#include "wrapper_interrupt.hpp"
#include "servo_driver_model.hpp"

void USART1TC_ITR(){
    get_debug_com()->tx_callback();
}

void TIM4_ITR() {
    get_bldc_if()->hall_itr_callback();
}

void TIM6_ITR() {
}
void TIM7_ITR() {
}