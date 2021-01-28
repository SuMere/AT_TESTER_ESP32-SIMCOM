#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "driver/gpio.h"
#include "driver/uart.h"
#include "esp_log.h"


//#include "esp_modem_dce_service.h"

#define UART_NUMBER 1
#define BUF_SIZE (2048)
#define RD_BUF_SIZE (BUF_SIZE)
#define MODEM_UART_TX 26
#define MODEM_UART_RX 34

/* TODO BE ABLE TO CONFIGURE IT
    GSM_ENABLE_GPIO on D47 is IO23
    GSM_ENABLE_GPIO on LiLyGO is IO4

    On D47 GSM IS POWERED BY PULLING UP AS OUTPUT IO5

    TX and RX are 27 and 26 respectively on LiLyGO
    TX and RX are 25 and 26 respectively on D47
*/

#define GSM_ENABLE_GPIO 23
#define GSM_VLTE_GPIO 27
#define GPIO_OUTPUT_PIN_SEL  ((1ULL<<GSM_ENABLE_GPIO) | (1ULL<<GSM_VLTE_GPIO))

static QueueHandle_t uart0_queue;

esp_err_t modem_init();
void test_at(const char* command);

#ifdef __cplusplus
}
#endif