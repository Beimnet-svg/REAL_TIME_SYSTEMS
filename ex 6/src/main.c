#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <gpio.h>
#include <power_clocks_lib.h>
#include <sysclk.h>
#include <compiler.h>
#include <board.h>
#include <stdio_usb.h>


#define CONFIG_USART_IF (AVR32_USART2)
#define txskIDLE_PRIORITY 0

#include "FreeRTOS.h"
#include "task.h"

#define TEST_A      AVR32_PIN_PA31
#define RESPONSE_A  AVR32_PIN_PA30
#define TEST_B      AVR32_PIN_PA29
#define RESPONSE_B  AVR32_PIN_PA28
#define TEST_C      AVR32_PIN_PA27
#define RESPONSE_C  AVR32_PIN_PB00


void busy_delay_ms(int delay){
    for(; delay != 0; delay--){
        for(int i = 0; i < 2108; i++){
            asm volatile ("" ::: "memory");
        }
    }
}

void busy_delay_short(void){
    for(int i = 0; i < 10; i++){
        asm volatile ("" ::: "memory");
    }
}

void init(){
	board_init();
	
    gpio_configure_pin(TEST_A, GPIO_DIR_INPUT);
    gpio_configure_pin(TEST_B, GPIO_DIR_INPUT);
    gpio_configure_pin(TEST_C, GPIO_DIR_INPUT);
    gpio_configure_pin(RESPONSE_A, GPIO_DIR_OUTPUT | GPIO_INIT_HIGH);
    gpio_configure_pin(RESPONSE_B, GPIO_DIR_OUTPUT | GPIO_INIT_HIGH);
    gpio_configure_pin(RESPONSE_C, GPIO_DIR_OUTPUT | GPIO_INIT_HIGH);

	pcl_switch_to_osc(PCL_OSC0, FOSC0, OSC0_STARTUP);
	
	stdio_usb_init(&CONFIG_USART_IF);

    #if defined(__GNUC__) && defined(__AVR32__)
	    setbuf(stdout, NULL);
	    setbuf(stdin,  NULL);
    #endif
}

static void taskFn(void* args){
	const portTickType delay = 200 / portTICK_RATE_MS;
	
    int iter = 0;

	while(1){
		gpio_toggle_pin(LED0_GPIO);
		printf("tickLED0 %d\r\n", iter++);
		
		vTaskDelay(delay);
	}
}

static void taskFn1(void* args){
	const portTickType delay = 500 / portTICK_RATE_MS;
	
    int iter = 0;

	while(1){
		gpio_toggle_pin(LED1_GPIO);
		printf("tickLED1 %d\r\n", iter++);
		
		vTaskDelay(delay);
	}
}

//TASK B
struct responseTaskArgs {
	struct {
		uint32_t test;
		uint32_t response;
	} pin;
};

static void responseTask(void* args){
	struct responseTaskArgs a = *(struct responseTaskArgs*)args;
	while(1){
		if(gpio_pin_is_low(a.pin.test)){
			gpio_set_pin_low(a.pin.response);
			}
			else{
			gpio_set_pin_high(a.pin.response);vTaskDelay(0);	}
	}
}

//TASK C
struct responseTaskArgsC {
	struct {
		uint32_t test;
		uint32_t response;
		uint8_t value;
	} pin;
};

static void responseTaskC(void* args){
	struct responseTaskArgsC a = *(struct responseTaskArgsC*)args;
	while(1){
		if(gpio_pin_is_low(a.pin.test)){
			busy_delay_ms(a.pin.value);
			gpio_set_pin_low(a.pin.response);
		}
		else{
		gpio_set_pin_high(a.pin.response);vTaskDelay(0);	}
	}
}

//TASK D
/*struct responseTaskArgsD {
	struct {
		uint32_t test;
		uint32_t response;
		uint8_t value;
	} pin;
};

static void responseTaskD(void* args){
	struct responseTaskArgsD a = *(struct responseTaskArgsD*)args;
	while(1){
		if(gpio_pin_is_low(a.pin.test)){
			busy_delay_ms(a.pin.value);
			gpio_set_pin_low(a.pin.response);
		}
		else
		{
			gpio_set_pin_high(a.pin.response);
			vTaskDelay(1);	
		}
	}*/
	
	
	//TASK E
	struct responseTaskArgsE {
		struct {
			uint32_t test;
			uint32_t response;
			uint8_t value;
		} pin;
	};

	static void responseTaskE(void* args){
		struct responseTaskArgsE a = *(struct responseTaskArgsE*)args;
		while(1){
			if(gpio_pin_is_low(a.pin.test)){
				busy_delay_ms(a.pin.value);
				gpio_set_pin_low(a.pin.response);
			}
			else
			{
				gpio_set_pin_high(a.pin.response);
				vTaskDelay(1);
			}
		}
}
int main(){
	init();
	//xTaskCreate(taskFn, "", 1024, NULL, txskIDLE_PRIORITY + 1, NULL);
	//xTaskCreate(taskFn1, "", 1024, NULL, txskIDLE_PRIORITY + 1, NULL);
	
	//TASKB
	/*xTaskCreate(responseTask, "", 1024,(&(struct responseTaskArgs){{TEST_A, RESPONSE_A}}),tskIDLE_PRIORITY + 1, NULL);
	xTaskCreate(responseTask, "", 1024,(&(struct responseTaskArgs){{TEST_B, RESPONSE_B}}),tskIDLE_PRIORITY + 1, NULL);
	xTaskCreate(responseTask, "", 1024,(&(struct responseTaskArgs){{TEST_C, RESPONSE_C}}),tskIDLE_PRIORITY + 1, NULL);*/

	//TASKC		
	//xTaskCreate(responseTaskC, "", 1024,(&(struct responseTaskArgsC){{TEST_A, RESPONSE_A, 0}}),tskIDLE_PRIORITY + 1, NULL);
	//xTaskCreate(responseTaskC, "", 1024,(&(struct responseTaskArgsC){{TEST_B, RESPONSE_B, 0}}),tskIDLE_PRIORITY + 1, NULL);
	//xTaskCreate(responseTaskC, "", 1024,(&(struct responseTaskArgsC){{TEST_C, RESPONSE_C, 3}}),tskIDLE_PRIORITY + 1, NULL);
		
	//TASKD
		//xTaskCreate(responseTaskD, "", 1024,(&(struct responseTaskArgsD){{TEST_A, RESPONSE_A, 0}}),tskIDLE_PRIORITY + 1, NULL);
		//xTaskCreate(responseTaskD, "", 1024,(&(struct responseTaskArgsD){{TEST_B, RESPONSE_B, 0}}),tskIDLE_PRIORITY + 1, NULL);
		//xTaskCreate(responseTaskD, "", 1024,(&(struct responseTaskArgsD){{TEST_C, RESPONSE_C, 3}}),tskIDLE_PRIORITY + 1, NULL);
			
	//TASKE
		xTaskCreate(responseTaskE, "", 1024,(&(struct responseTaskArgsE){{TEST_A, RESPONSE_A, 0}}),tskIDLE_PRIORITY + 2, NULL);
		xTaskCreate(responseTaskE, "", 1024,(&(struct responseTaskArgsE){{TEST_B, RESPONSE_B, 0}}),tskIDLE_PRIORITY + 2, NULL);
		xTaskCreate(responseTaskE, "", 1024,(&(struct responseTaskArgsE){{TEST_C, RESPONSE_C, 3}}),tskIDLE_PRIORITY + 1, NULL);
	// Start the scheduler, anything after this will not run.
	vTaskStartScheduler();
    
}

