#include <stdio.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/i2c.h"
#include "esp_log.h"

#define I2C_MASTER_SCL_IO    22
#define I2C_MASTER_SDA_IO    21
#define I2C_MASTER_NUM       I2C_NUM_0
#define I2C_MASTER_FREQ_HZ   400000
#define PCA9685_ADDR         0x40

static const char *TAG = "PCA9685";

esp_err_t i2c_master_init(void) {
    i2c_config_t conf = {
        .mode = I2C_MODE_MASTER,
        .sda_io_num = I2C_MASTER_SDA_IO,
        .sda_pullup_en = GPIO_PULLUP_ENABLE,
        .scl_io_num = I2C_MASTER_SCL_IO,
        .scl_pullup_en = GPIO_PULLUP_ENABLE,
        .master.clk_speed = I2C_MASTER_FREQ_HZ,
    };
    esp_err_t err = i2c_param_config(I2C_MASTER_NUM, &conf);
    if (err != ESP_OK) return err;
    return i2c_driver_install(I2C_MASTER_NUM, conf.mode, 0, 0, 0);
}

esp_err_t pca9685_write_byte(uint8_t reg, uint8_t data) {
    i2c_cmd_handle_t cmd = i2c_cmd_link_create();
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, (PCA9685_ADDR << 1) | I2C_MASTER_WRITE, true);
    i2c_master_write_byte(cmd, reg, true);
    i2c_master_write_byte(cmd, data, true);
    i2c_master_stop(cmd);
    esp_err_t ret = i2c_master_cmd_begin(I2C_MASTER_NUM, cmd, pdMS_TO_TICKS(1000));
    i2c_cmd_link_delete(cmd);
    return ret;
}

void pca9685_set_pwm(uint8_t channel, uint16_t on, uint16_t off) {
    pca9685_write_byte(0x06 + 4 * channel, on & 0xFF);
    pca9685_write_byte(0x07 + 4 * channel, on >> 8);
    pca9685_write_byte(0x08 + 4 * channel, off & 0xFF);
    pca9685_write_byte(0x09 + 4 * channel, off >> 8);
}

void app_main(void) {
    ESP_ERROR_CHECK(i2c_master_init());
    
    // Initialize PCA9685
    pca9685_write_byte(0x00, 0x00); // Mode1
    pca9685_write_byte(0x01, 0x04); // Mode2
    pca9685_write_byte(0xFE, 0x79); // 50Hz prescale
    
    printf("Servo Control - Enter angle (-50 to +50):\n");
    printf("Pulse widths: -50°=1000us, 0°=1500us, +50°=2000us\n");
    
    char input[10];
    while (1) {
        if (fgets(input, sizeof(input), stdin)) {
            int angle = atoi(input);
            if (angle >= -50 && angle <= 50) {
                // Map angle to pulse width (1000-2000us)
                uint16_t pulse = 205 + (angle + 50) * 2.05; // ~205-410 for 1000-2000us
                pca9685_set_pwm(0, 0, pulse);
                
                printf("Angle: %d° -> Pulse: %dus\n", angle, 1000 + (angle + 50) * 10);
            } else {
                printf("Angle out of range (-50 to +50)\n");
            }
        }
        vTaskDelay(pdMS_TO_TICKS(100));
    }
}