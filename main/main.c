#include <stdio.h>
#include "hal/adc_types.h"
#include "esp_err.h"
#include "esp_adc/adc_oneshot.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "driver/gpio.h"
#define TAG "LDR"
#define LED_PIN 13
void readLDR(void *);

void app_main(void)
{
    TaskHandle_t task_handle = NULL;
    xTaskCreate(readLDR,
                "LDR",2048,NULL,1,&task_handle);
    gpio_reset_pin(LED_PIN);
    gpio_set_direction(LED_PIN,GPIO_MODE_OUTPUT);
   
}

void readLDR(void * pvParameters){
    adc_oneshot_unit_handle_t adc_handle;
adc_oneshot_unit_init_cfg_t adc_cfg = {
    .clk_src = ADC_RTC_CLK_SRC_DEFAULT,
    .ulp_mode = ADC_ULP_MODE_DISABLE,
    .unit_id = ADC_UNIT_1
};
ESP_ERROR_CHECK(adc_oneshot_new_unit(&adc_cfg,&adc_handle));
adc_oneshot_chan_cfg_t channel_cfg = {
    .atten = ADC_ATTEN_DB_12,
    .bitwidth = ADC_BITWIDTH_12
};
ESP_ERROR_CHECK(adc_oneshot_config_channel(adc_handle,ADC_CHANNEL_4,&channel_cfg));
int adc_raw;
int voltage_mv;
while(1) {
        
        ESP_ERROR_CHECK(adc_oneshot_read(adc_handle, ADC_CHANNEL_4, &adc_raw));
        
       
        voltage_mv = adc_raw * 3300 / 4095;  
        ESP_LOGI(TAG, "Voltage: %d mV", voltage_mv);
        if(voltage_mv < 500){
            gpio_set_level(LED_PIN,1);
        }else{
            gpio_set_level(LED_PIN,0);
        }
        vTaskDelay(pdMS_TO_TICKS(2000));
    }
}