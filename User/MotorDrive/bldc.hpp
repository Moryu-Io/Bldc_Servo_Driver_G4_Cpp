#ifndef BLDC_HPP_
#define BLDC_HPP_

#include "main.h"
#include "stm32g4xx.h"

class BLDC {
public:
  BLDC(){};

  struct DrivePhase {
    float U;
    float V;
    float W;
  };

  struct DriveDuty {
    uint32_t   u32_Out_Enable;
    DrivePhase Duty;
  };

  virtual void init() = 0;

  virtual void hall_itr_callback(){};

  virtual void set_drive_duty(DriveDuty &_Vol) = 0;

  virtual uint8_t  get_hall_state() { return u8_now_hall_state_; };
  virtual uint32_t get_hall_count() { return u16_hall_counter_; };

protected:
  uint8_t    u8_now_hall_state_;
  int8_t     s8_now_motor_dir_;
  uint16_t   u16_hall_counter_;
  float      fl_elec_angle_;
  DrivePhase fl_now_current_;
  DrivePhase fl_now_bev_;
};

#endif