#ifdef USE_ESP32_FRAMEWORK_ARDUINO
#include "LinBusListener.h"
#include "esphome/core/log.h"
#include "driver/uart.h"
#include "soc/uart_struct.h"
#include "soc/uart_reg.h"
#ifdef CUSTOM_ESPHOME_UART
#include "esphome/components/uart/truma_uart_component_esp32_arduino.h"
#define ESPHOME_UART uart::truma_ESP32ArduinoUARTComponent
#else
#define ESPHOME_UART uart::ESP32ArduinoUARTComponent
#endif // CUSTOM_ESPHOME_UART
#include "esphome/components/uart/uart_component_esp32_arduino.h"

namespace esphome {
namespace truma_inetbox {

static const char *const TAG = "truma_inetbox.LinBusListener";

#define QUEUE_WAIT_BLOCKING (portTickType) portMAX_DELAY

void LinBusListener::setup_framework() {
  auto uartComp = static_cast<ESPHOME_UART *>(this->parent_);

  // Explicit cast workaround to fix invalid conversion error
  uint8_t uart_num_raw = uartComp->get_hw_serial_number();
  uart_port_t uart_num = static_cast<uart_port_t>(static_cast<int>(uart_num_raw));

  auto hw_serial = uartComp->get_hw_serial();

  // Configure UART interrupt settings
  uart_intr_config_t uart_intr;
  uart_intr.intr_enable_mask = UART_RXFIFO_FULL_INT_ENA_M | UART_RXFIFO_TOUT_INT_ENA_M;
  uart_intr.rxfifo_full_thresh = 1;
  uart_intr.rx_timeout_thresh = 10;
  uart_intr.txfifo_empty_intr_thresh = 10;

  uart_intr_config(uart_num, &uart_intr);

  hw_serial->onReceive([this]() { this->onReceive_(); }, false);
  hw_serial->onReceiveError([this](hardwareSerial_error_t val) {
    // Clear buffer on error
    this->clear_uart_buffer_();
    if (val == UART_BREAK_ERROR) {
      if (this->current_state_ != READ_STATE_SYNC) {
        this->current_state_ = READ_STATE_BREAK;
      }
      return;
    }
  });

  // Create a FreeRTOS task pinned to core 0 for processing LIN bus messages
  xTaskCreatePinnedToCore(LinBusListener::eventTask_,
                          "lin_event_task",    // task name
                          4096,                // stack size in words
                          this,                // parameter
                          2,                   // priority
                          &this->eventTaskHandle_,
                          0);                  // core ID

  if (this->eventTaskHandle_ == NULL) {
    ESP_LOGE(TAG, " -- LIN message Task not created!");
  }
}

void LinBusListener::eventTask_(void *args) {
  LinBusListener *instance = (LinBusListener *)args;
  for (;;) {
    instance->process_lin_msg_queue(QUEUE_WAIT_BLOCKING);
  }
}

}  // namespace truma_inetbox
}  // namespace esphome

#undef QUEUE_WAIT_BLOCKING
#undef ESPHOME_UART

#endif  // USE_ESP32_FRAMEWORK_ARDUINO
