#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

extern "C" {
    void app_main(void);
}

void app_main(void) {
    // Initialize components
    xTaskCreate(MPU6050_Task, "MPU6050_Task", 2048, NULL, 5, NULL);
    xTaskCreate(PCA9685_Task, "PCA9685_Task", 2048, NULL, 5, NULL);
    xTaskCreate(HX711_Task, "HX711_Task", 2048, NULL, 5, NULL);
    
    // Start the scheduler
    vTaskStartScheduler();
}