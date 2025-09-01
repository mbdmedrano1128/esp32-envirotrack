#pragma once

#include "esp_err.h"

typedef struct {
    int temperature;  // in °C
    int humidity;     // in %
} dht11_data_t;

/**
 * @brief Initialize the DHT11 sensor GPIO
 * 
 * @param gpio_num GPIO connected to DHT11 data pin
 * @return esp_err_t ESP_OK on success, ESP_ERR_INVALID_ARG if GPIO is invalid
 */
esp_err_t dht11_init(gpio_num_t gpio_num);

/**
 * @brief Read temperature and humidity from DHT11
 * 
 * @param out_data Pointer to dht11_data_t struct to store results
 * @return esp_err_t ESP_OK on success, ESP_FAIL on checksum or timing error
 */
esp_err_t dht11_read(dht11_data_t *out_data);