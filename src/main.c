#include <asf.h>
#include <board.h>
#include <gpio.h>
#include <sysclk.h>
#include "busy_delay.h"

#define CONFIG_USART_IF (AVR32_USART2)

// defines for BRTT interface
#define TEST_A      AVR32_PIN_PA31
#define RESPONSE_A  AVR32_PIN_PA30
#define TEST_B      AVR32_PIN_PA29
#define RESPONSE_B  AVR32_PIN_PA28
#define TEST_C      AVR32_PIN_PA27
#define RESPONSE_C  AVR32_PIN_PB00

volatile uint8_t test_a_flag = 0;
volatile uint8_t test_b_flag = 0;
volatile uint8_t test_c_flag = 0;

__attribute__((__interrupt__)) static void interrupt_J3(void);

void init(){
    sysclk_init();
    board_init();
    busy_delay_init(BOARD_OSC0_HZ);
	
	gpio_configure_pin(TEST_A, GPIO_DIR_INPUT);
	gpio_configure_pin(RESPONSE_A, GPIO_DIR_OUTPUT | GPIO_INIT_HIGH);
	gpio_configure_pin(TEST_B, GPIO_DIR_INPUT);
	gpio_configure_pin(RESPONSE_B, GPIO_DIR_OUTPUT | GPIO_INIT_HIGH);
	gpio_configure_pin(TEST_C, GPIO_DIR_INPUT);
	gpio_configure_pin(RESPONSE_C, GPIO_DIR_OUTPUT | GPIO_INIT_HIGH);

    gpio_enable_pin_interrupt(TEST_A, GPIO_FALLING_EDGE);
    gpio_enable_pin_interrupt(TEST_B, GPIO_FALLING_EDGE);
    gpio_enable_pin_interrupt(TEST_C, GPIO_FALLING_EDGE);
    
    cpu_irq_disable();
    INTC_init_interrupts();
    INTC_register_interrupt(&interrupt_J3, AVR32_GPIO_IRQ_3, AVR32_INTC_INT1);
    cpu_irq_enable();
    
    stdio_usb_init(&CONFIG_USART_IF);
}

__attribute__((__interrupt__)) static void interrupt_J3(void){ 
    // Set flags for deferred processing in the main loop
    if(gpio_get_pin_interrupt_flag(TEST_A)){
        gpio_clear_pin_interrupt_flag(TEST_A);
        test_a_flag = 1;
    }

    if(gpio_get_pin_interrupt_flag(TEST_B)){
        gpio_clear_pin_interrupt_flag(TEST_B);
        test_b_flag = 1;
    }

    if(gpio_get_pin_interrupt_flag(TEST_C)){
        gpio_clear_pin_interrupt_flag(TEST_C);
        test_c_flag = 1;
    }
}

int main (void){
    init();

    while(1){
        // Process TEST_A response in the main loop
        if(test_a_flag){
            gpio_set_pin_low(RESPONSE_A);
            while(gpio_pin_is_low(TEST_A)){}
            gpio_set_pin_high(RESPONSE_A);
            test_a_flag = 0;
        }

        // Process TEST_B response in the main loop
        if(test_b_flag){
            gpio_set_pin_low(RESPONSE_B);
            while(gpio_pin_is_low(TEST_B)){}
				busy_delay_us(100);
            gpio_set_pin_high(RESPONSE_B);
            test_b_flag = 0;
        }

        // Process TEST_C response in the main loop
        if(test_c_flag){
            gpio_set_pin_low(RESPONSE_C);
            while(gpio_pin_is_low(TEST_C)){}
            gpio_set_pin_high(RESPONSE_C);
            test_c_flag = 0;
        }
    }

    return 0;
}
