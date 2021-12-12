#include "servo_driver_model.hpp"
#include "uart_dmac.hpp"

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

COM_BASE* get_debug_com(){ return &Rs485Com; };

void initialize_servo_driver_model() {
  Rs485Com.init_constparam(u8_RS485_RXBUF, RS485_RXBUF_LENGTH,
                           u8_RS485_TXBUF, RS485_TXBUF_LENGTH);
  Rs485Com.init_rxtx();
}
