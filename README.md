# EnviroTrack: ESP32 Environmental Logger

EnviroTrack is a modular firmware project for the ESP32, designed to log environmental data using multiple sensors and transmit readings via UART and MQTT. Built with ESP-IDF and FreeRTOS, it emphasizes clean architecture, scalability, and low-level control.

## 📦 Features

- 🌡️ Temperature & Humidity via DHT11  
- 🌞 Light intensity via LDR  
- 🌡️ Analog temperature via LM35DZ  
- 📤 UART logging for debugging  
- 📡 MQTT publishing (planned)  
- 🔌 Modular driver structure  
- 🧵 FreeRTOS task-based design  

## 🚀 Getting Started
- Clone the repository
git clone https://github.com/mbdmedrano1128/esp32-envirotrack.git
cd esp32-envirotrack
- Activate the ESP-IDF environment
. $HOME/documents/projects/esp/esp-idf/export.sh
- Build and flash the firmware
idf.py build
idf.py -p /dev/ttyUSB0 flash monitor

🛠️ Development Notes
- Written in pure C using ESP-IDF 5.5
- Designed for WSL-based development
- Uses esp_log for structured logging
- Modular components registered via CMake
- UART output used for real-time debugging and sensor feedback
