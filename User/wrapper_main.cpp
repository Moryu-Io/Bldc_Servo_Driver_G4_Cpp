#include "wrapper_main.hpp"
#include "main.h"
#include "servo_driver_model.hpp"
#include "wrapper_interrupt.hpp"

void cpp_wrapper_main_setup(void) {
  initialize_servo_driver_model();
}

void cpp_wrapper_main_loop(void) {
  volatile uint32_t enc_cnt   = get_bldc_if()->get_hall_count();
  volatile uint32_t enc_state = get_bldc_if()->get_hall_state();
  LL_mDelay(100);
}
