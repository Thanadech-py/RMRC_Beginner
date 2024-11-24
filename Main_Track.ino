#include "RMRC_BreakoutV2.h"

//define Motors Port
RMRC_BreakoutV2 m2(M1M2_ADDR, MOTOR_2), m4(M1M2_ADDR, MOTOR_4);

#define MAXIMUM_SHAFT_RPM 294.0f
#define GEAR_RATIO        34.0f
                
void Setup_Main_track() {
  delay(400);
  Serial.begin(115200);

  m2.begin();
  m4.begin();

  // After change motor mode when uploading done you should turn off board 1 time.
  m2.setMotorMode(SPEED_AND_DUTY);
  m4.setMotorMode(SPEED_AND_DUTY);

  m2.setDuty(0);
  m4.setDuty(0);

  m2.setEncoderResolution(11.0f);
  m4.setEncoderResolution(11.0f);

  m2.setMaximumRPM(MAXIMUM_SHAFT_RPM * GEAR_RATIO);
  m4.setMaximumRPM(MAXIMUM_SHAFT_RPM * GEAR_RATIO);

  // if you want to using default value of kp, ki and kd just delete or comment function under this line.
  m2.setKp(0.0f);
  m2.setKi(0.0f);
  m2.setKd(0.0f);

  m4.setKp(0.0f);
  m4.setKi(0.0f);
  m4.setKd(0.0f);

}

void Control_Main_Track() {
  // setDuty: range -16999 to 16999.
  m2.setDuty(0);
  m4.setDuty(0);


  //setRPM: range -maximum_rpm to maximum_rpm
  m2.setRPM(0.0f);
  m4.setRPM(0.0f);
}
