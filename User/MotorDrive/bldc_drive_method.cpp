#include "bldc_drive_method.hpp"

void BldcDriveMethod6Step::update() {
  drive_6step();
}

void BldcDriveMethod6Step::itr_callback(){
  drive_6step();
}

void BldcDriveMethod6Step::drive_6step() {
  uint8_t _h_state = p_bldc_->get_hall_state();

  BLDC::DriveDuty _duty = {};

  if(InRef_.Vq >= 0) {
    switch(_h_state) {
    case 1:
      /* hall_UがHigh、U→Wに電流 */
      _duty.u8_U_out_enable = DRIVE_OUT_BOTH_ENABLE;
      _duty.u8_V_out_enable = DRIVE_OUT_DISABLE;
      _duty.u8_W_out_enable = DRIVE_OUT_LOW_ENABLE;
      _duty.Duty.U         = InRef_.Vq;
      _duty.Duty.V         = 0.0f;
      _duty.Duty.W         = 0.0f;
      break;
    case 3:
      /* hall_U,VがHigh、V→Wに電流 */
      _duty.u8_U_out_enable = DRIVE_OUT_DISABLE;
      _duty.u8_V_out_enable = DRIVE_OUT_BOTH_ENABLE;
      _duty.u8_W_out_enable = DRIVE_OUT_LOW_ENABLE;
      _duty.Duty.U         = 0.0f;
      _duty.Duty.V         = InRef_.Vq;
      _duty.Duty.W         = 0.0f;
      break;
    case 2:
      /* hall_VがHigh、V→Uに電流 */
      _duty.u8_U_out_enable = DRIVE_OUT_LOW_ENABLE;
      _duty.u8_V_out_enable = DRIVE_OUT_BOTH_ENABLE;
      _duty.u8_W_out_enable = DRIVE_OUT_DISABLE;
      _duty.Duty.U         = 0.0f;
      _duty.Duty.V         = InRef_.Vq;
      _duty.Duty.W         = 0.0f;
      break;
    case 6:
      /* hall_V,WがHigh、W→Uに電流 */
      _duty.u8_U_out_enable = DRIVE_OUT_LOW_ENABLE;
      _duty.u8_V_out_enable = DRIVE_OUT_DISABLE;
      _duty.u8_W_out_enable = DRIVE_OUT_BOTH_ENABLE;
      _duty.Duty.U         = 0.0f;
      _duty.Duty.V         = 0.0f;
      _duty.Duty.W         = InRef_.Vq;
      break;
    case 4:
      /* hall_WがHigh、W→Vに電流 */
      _duty.u8_U_out_enable = DRIVE_OUT_DISABLE;
      _duty.u8_V_out_enable = DRIVE_OUT_LOW_ENABLE;
      _duty.u8_W_out_enable = DRIVE_OUT_BOTH_ENABLE;
      _duty.Duty.U         = 0.0f;
      _duty.Duty.V         = 0.0f;
      _duty.Duty.W         = InRef_.Vq;
      break;
    case 5:
      /* hall_U,WがHigh、U→Vに電流 */
      _duty.u8_U_out_enable = DRIVE_OUT_BOTH_ENABLE;
      _duty.u8_V_out_enable = DRIVE_OUT_LOW_ENABLE;
      _duty.u8_W_out_enable = DRIVE_OUT_DISABLE;
      _duty.Duty.U         = InRef_.Vq;
      _duty.Duty.V         = 0.0f;
      _duty.Duty.W         = 0.0f;
      break;
    default:
      break;
    }
  }else{
    switch(_h_state) {
    case 1:
      /* hall_UがHigh、W→Uに電流 */
      _duty.u8_U_out_enable = DRIVE_OUT_LOW_ENABLE;
      _duty.u8_V_out_enable = DRIVE_OUT_DISABLE;
      _duty.u8_W_out_enable = DRIVE_OUT_BOTH_ENABLE;
      _duty.Duty.U         = 0.0f;
      _duty.Duty.V         = 0.0f;
      _duty.Duty.W         = -InRef_.Vq;
      break;
    case 3:
      /* hall_U,VがHigh、W→Vに電流 */
      _duty.u8_U_out_enable = DRIVE_OUT_DISABLE;
      _duty.u8_V_out_enable = DRIVE_OUT_LOW_ENABLE;
      _duty.u8_W_out_enable = DRIVE_OUT_BOTH_ENABLE;
      _duty.Duty.U         = 0.0f;
      _duty.Duty.V         = 0.0f;
      _duty.Duty.W         = -InRef_.Vq;
      break;
    case 2:
      /* hall_VがHigh、U→Vに電流 */
      _duty.u8_U_out_enable = DRIVE_OUT_BOTH_ENABLE;
      _duty.u8_V_out_enable = DRIVE_OUT_LOW_ENABLE;
      _duty.u8_W_out_enable = DRIVE_OUT_DISABLE;
      _duty.Duty.U         = -InRef_.Vq;
      _duty.Duty.V         = 0.0f;
      _duty.Duty.W         = 0.0f;
      break;
    case 6:
      /* hall_V,WがHigh、U→Wに電流 */
      _duty.u8_U_out_enable = DRIVE_OUT_BOTH_ENABLE;
      _duty.u8_V_out_enable = DRIVE_OUT_DISABLE;
      _duty.u8_W_out_enable = DRIVE_OUT_LOW_ENABLE;
      _duty.Duty.U         = -InRef_.Vq;
      _duty.Duty.V         = 0.0f;
      _duty.Duty.W         = 0.0f;
      break;
    case 4:
      /* hall_WがHigh、V→Wに電流 */
      _duty.u8_U_out_enable = DRIVE_OUT_DISABLE;
      _duty.u8_V_out_enable = DRIVE_OUT_BOTH_ENABLE;
      _duty.u8_W_out_enable = DRIVE_OUT_LOW_ENABLE;
      _duty.Duty.U         = 0.0f;
      _duty.Duty.V         = -InRef_.Vq;
      _duty.Duty.W         = 0.0f;
      break;
    case 5:
      /* hall_U,WがHigh、V→Uに電流 */
      _duty.u8_U_out_enable = DRIVE_OUT_LOW_ENABLE;
      _duty.u8_V_out_enable = DRIVE_OUT_BOTH_ENABLE;
      _duty.u8_W_out_enable = DRIVE_OUT_DISABLE;
      _duty.Duty.U         = 0.0f;
      _duty.Duty.V         = -InRef_.Vq;
      _duty.Duty.W         = 0.0f;
      break;
    default:
      break;
    }
  }

  p_bldc_->set_drive_duty(_duty);

}

void BldcDriveMethodVector::update() {
}
