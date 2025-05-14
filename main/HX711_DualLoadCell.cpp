void hx711_init(int dout_pin, int sck_pin) {
    gpio_config_t io_conf = {
        .intr_type = GPIO_INTR_DISABLE,
        .mode = GPIO_MODE_OUTPUT,
        .pin_bit_mask = (1ULL << sck_pin),
        .pull_down_en = 0,
        .pull_up_en = 0,
    };
    gpio_config(&io_conf);
    
    io_conf.mode = GPIO_MODE_INPUT;
    io_conf.pin_bit_mask = (1ULL << dout_pin);
    gpio_config(&io_conf);
    
    gpio_set_level(sck_pin, 0);
}

long hx711_read(int dout_pin, int sck_pin) {
    long count = 0;
    
    // Wait for data ready
    while (gpio_get_level(dout_pin));
    
    // Read 24 bits
    for (int i = 0; i < 24; i++) {
        gpio_set_level(sck_pin, 1);
        count = count << 1;
        gpio_set_level(sck_pin, 0);
        if (gpio_get_level(dout_pin)) {
            count++;
        }
    }
    
    // 25th pulse to set gain (128)
    gpio_set_level(sck_pin, 1);
    gpio_set_level(sck_pin, 0);
    
    // Convert to signed
    if (count & 0x800000) {
        count = count | 0xFF000000;
    }
    
    return count;
}

void app_main(void) {
    hx711_init(HX711_DOUT_1, HX711_SCK_1);
    hx711_init(HX711_DOUT_2, HX711_SCK_2);
    
    printf("HX711 Dual Load Cell Reader\n");
    printf("Time(ms),LoadCell1,LoadCell2\n");
    
    while (1) {
        long reading1 = hx711_read(HX711_DOUT_1, HX711_SCK_1);
        long reading2 = hx711_read(HX711_DOUT_2, HX711_SCK_2);
        
        printf("%ld,%ld,%ld\n", esp_timer_get_time()/1000, reading1, reading2);
        
        vTaskDelay(pdMS_TO_TICKS(100));
    }
}