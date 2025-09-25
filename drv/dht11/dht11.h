#pragma once

#include "driver/gpio.h"
#include "esp_err.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief DHT11 sensor data structure
 */
typedef struct {
    int temperature;  /**< Temperature in °C */
    int humidity;     /**< Relative Humidity in % */
} dht11_data_t;

/**
 * @brief Initialize the DHT11 sensor
 *
 * Configures the specified GPIO pin for communication with the DHT11 sensor.
 *
 * @param[in] gpio_num  GPIO number connected to the DHT11 data pin
 * @return
 *      - ESP_OK: Initialization successful  
 *      - ESP_ERR_INVALID_ARG: Invalid GPIO number  
 */
esp_err_t dht11_init(gpio_num_t gpio_num);

/**
 * @brief Read temperature and humidity from the DHT11 sensor
 *
 * Attempts to read data from the DHT11 sensor and stores the results
 * in the provided data structure.
 *
 * @param[out] out_data  Pointer to a dht11_data_t struct to store results
 * @return
 *      - ESP_OK: Data read successfully  
 *      - ESP_FAIL: Checksum mismatch or timing error  
 *      - ESP_ERR_INVALID_ARG: Null pointer passed  
 */
esp_err_t dht11_read(dht11_data_t *out_data);

#ifdef __cplusplus
}
#endif
