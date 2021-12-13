#include "bldc.hpp"
#include "mymath.hpp"
#include "servo_driver_model.hpp"
#include "uart_dmac.hpp"

class Maxon_BLDC : public BLDC {
public:
  Maxon_BLDC(){};

  void init() override {
    LL_TIM_EnableIT_CC1(TIM4);
    LL_TIM_CC_EnableChannel(TIM4, LL_TIM_CHANNEL_CH1);
    LL_TIM_EnableCounter(TIM4);
  };

  void hall_itr_callback() override {
    uint32_t portb               = LL_GPIO_ReadInputPort(GPIOB);
    uint8_t  _u8_next_hall_state = (uint8_t)((portb >> 6) & 0b111);
    switch(_u8_next_hall_state) {
    case 1:
      if(u8_now_hall_state_ == 5)
        s8_now_motor_dir_ = 1;
      else if(u8_now_hall_state_ == 3)
        s8_now_motor_dir_ = -1;
      break;
    case 3:
      if(u8_now_hall_state_ == 1)
        s8_now_motor_dir_ = 1;
      else if(u8_now_hall_state_ == 2)
        s8_now_motor_dir_ = -1;
      break;
    case 2:
      if(u8_now_hall_state_ == 3)
        s8_now_motor_dir_ = 1;
      else if(u8_now_hall_state_ == 6)
        s8_now_motor_dir_ = -1;
      break;
    case 6:
      if(u8_now_hall_state_ == 2)
        s8_now_motor_dir_ = 1;
      else if(u8_now_hall_state_ == 4)
        s8_now_motor_dir_ = -1;
      break;
    case 4:
      if(u8_now_hall_state_ == 6)
        s8_now_motor_dir_ = 1;
      else if(u8_now_hall_state_ == 5)
        s8_now_motor_dir_ = -1;
      break;
    case 5:
      if(u8_now_hall_state_ == 4)
        s8_now_motor_dir_ = 1;
      else if(u8_now_hall_state_ == 1)
        s8_now_motor_dir_ = -1;
      break;
    }

    u16_hall_counter_ += s8_now_motor_dir_;

    u8_now_hall_state_ = _u8_next_hall_state;
  };

  void set_drive_duty(DriveDuty &_Vol) override {
    uint32_t u_duty = (uint32_t)(mymath::satf(_Vol.Duty.U * Vm_inv, 1.0f, 0.0f) * (float)TIM1->ARR);
    uint32_t v_duty = (uint32_t)(mymath::satf(_Vol.Duty.V * Vm_inv, 1.0f, 0.0f) * (float)TIM1->ARR);
    uint32_t w_duty = (uint32_t)(mymath::satf(_Vol.Duty.W * Vm_inv, 1.0f, 0.0f) * (float)TIM1->ARR);
    TIM1->CCR1      = u_duty;
    TIM1->CCR2      = v_duty;
    TIM1->CCR3      = w_duty;
    TIM1->CCER      = _Vol.u32_Out_Enable;
  };

private:
  const float Vm_inv = 1.0f / 12.0f;
};

static Maxon_BLDC MxnBldc;

BLDC *get_bldc_if() { return &MxnBldc; };

class FSD_RS485 : public UART_DMAC {
public:
  FSD_RS485(const USART_TypeDef *_u, const DMA_TypeDef *_d,
            const uint32_t _dch_r, const uint32_t _dch_t)
      : UART_DMAC(_u, _d, _dch_r, _dch_t){};

protected:
  void enable_tx() override { LL_GPIO_SetOutputPin(GPIOC, LL_GPIO_PIN_14); };
  void disable_tx() override { LL_GPIO_ResetOutputPin(GPIOC, LL_GPIO_PIN_14); };
};

constexpr uint16_t RS485_RXBUF_LENGTH = 512;
constexpr uint16_t RS485_TXBUF_LENGTH = 256;
static uint8_t     u8_RS485_RXBUF[RS485_RXBUF_LENGTH];
static uint8_t     u8_RS485_TXBUF[RS485_TXBUF_LENGTH];
static FSD_RS485   Rs485Com(USART1, DMA1, LL_DMA_CHANNEL_3, LL_DMA_CHANNEL_4);

COM_BASE *get_debug_com() { return &Rs485Com; };

void initialize_servo_driver_model() {
  Rs485Com.init_constparam(u8_RS485_RXBUF, RS485_RXBUF_LENGTH,
                           u8_RS485_TXBUF, RS485_TXBUF_LENGTH);
  Rs485Com.init_rxtx();

  MxnBldc.init();
}
