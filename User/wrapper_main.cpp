#include "main.h"
#include "wrapper_main.hpp"
#include "wrapper_interrupt.hpp"
#include "servo_driver_model.hpp"

void cpp_wrapper_main_setup(void){
    initialize_servo_driver_model();
}

void cpp_wrapper_main_loop(void){
    LL_mDelay(100);
}

