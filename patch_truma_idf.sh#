#!/bin/bash

COMPONENT_DIR="./components/truma_inetbox"

echo "Starte ESP-IDF Patch für truma_inetbox..."

# Prüfen, ob Verzeichnis existiert
if [ ! -d "$COMPONENT_DIR" ]; then
  echo "Fehler: Verzeichnis $COMPONENT_DIR existiert nicht!"
  exit 1
fi

# --- 1. u_int32_t → uint32_t ---
echo "Ersetze u_int32_t → uint32_t ..."
find "$COMPONENT_DIR" -type f \( -name "*.cpp" -o -name "*.h" \) | while read -r file; do
  sed -i '' 's/\bu_int32_t\b/uint32_t/g' "$file"
done

# --- 2. u_int16_t → uint16_t ---
echo "Ersetze u_int16_t → uint16_t ..."
find "$COMPONENT_DIR" -type f \( -name "*.cpp" -o -name "*.h" \) | while read -r file; do
  sed -i '' 's/\bu_int16_t\b/uint16_t/g' "$file"
done

# --- 3. uart_intr_config Cast ---
UART_FILE="$COMPONENT_DIR/LinBusListener_esp_idf.cpp"
if [ -f "$UART_FILE" ]; then
  echo "Passe uart_intr_config Cast an ..."
  sed -i '' 's/uart_intr_config(uart_num, &uart_intr)/uart_intr_config(static_cast<uart_port_t>(uart_num), \&uart_intr)/' "$UART_FILE"
else
  echo "Warnung: $UART_FILE nicht gefunden!"
fi

# --- 4. NUMBER_SCHEMA Warnung (ESPHome 2025.11+) ---
echo "Passe number schema an (deprecated NUMBER_SCHEMA) ..."
find "$COMPONENT_DIR" -type f \( -name "*.py" -o -name "*.cpp" -o -name "*.h" \) | while read -r file; do
  sed -i '' 's/number\.NUMBER_SCHEMA/number.number_schema/g' "$file"
done

echo "Patch abgeschlossen!"