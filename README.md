# ESPHome truma_inetbox component

ESPHome component to remote control Truma CP Plus Heater by simulating a Truma iNet box.

See [1](https://github.com/danielfett/inetbox.py) and [2](https://github.com/mc0110/inetbox2mqtt) for great documentation about how to connect an CP Plus to an ESP32 or RP2040.

## Acknowledgements

This project is based on the work of the [WomoLIN project](https://github.com/muccc/WomoLIN) and [mc0110 inetbox.py](https://github.com/danielfett/inetbox.py), especially the initial protocol decod[...] 

## Example configuation

This example is just for connecting ESPHome to the CP Plus. See [truma.yaml](/truma.yaml) for an example config with all possible things configured.

```yaml
esphome:
  name: "esphome-truma"

external_components:
  - source: github://Fabian-Schmidt/esphome-truma_inetbox
    components: ["truma_inetbox"]

esp32:
  board: mhetesp32devkit

uart:
  - id: lin_uart_bus
    baud_rate: 9600
    stop_bits: 2

truma_inetbox:
  uart_id: lin_uart_bus

binary_sensor:
  - platform: truma_inetbox
    name: "CP Plus alive"
    type: CP_PLUS_CONNECTED

sensor:
  - platform: truma_inetbox
    name: "Current Room Temperature"
    type: CURRENT_ROOM_TEMPERATURE
  - platform: truma_inetbox
    name: "Current Water Temperature"
    type: CURRENT_WATER_TEMPERATURE
  - platform: truma_inetbox
    name: "Target Room Temperature"
    type: TARGET_ROOM_TEMPERATURE
  - platform: truma_inetbox
    name: "Target Water Temperature"
    type: TARGET_WATER_TEMPERATURE
```

## ESPHome components

This project contains the following ESPHome components:

- `truma_inetbox` has the following settings:
  - `cs_pin` (optional) if you connect the pin of your lin driver chip.
  - `fault_pin` (optional) if you connect the pin of your lin driver chip.
  - `on_heater_message` (optional) [ESPHome Trigger](https://esphome.io/guides/automations.html) when a message from CP Plus is recieved.

Requires ESP Home 2023.4 or higher.

[truncated...]