#include "RMRC_BreakoutV2.h"
/*Instructions for setting up the flipper:
1. Rotate the flipper to be parallel with the floor.
2. Read the offset value of the current position of the flipper by printing the value of "flipper.getOffset();".
   Then, set this value as the first parameter of the function "flipper.setOffset();", ensuring the value falls between 1300 and 2700.
3. Rotate the flipper upwards by 180 degrees, then print the value of "getKAngle();"
   and set this value as the second parameter of the function "flipper.setOffset();".
4. Rotate the flipper downwards by 360 degrees, then print the value of "getKAngle();"
   and set this value as the final parameter of the function "flipper.setOffset();".*/

RMRC_BreakoutV2 m1(M1M2_ADDR, MOTOR_1), m3(M1M2_ADDR, MOTOR_3);

void Setup_Flipper() {
  delay(400);
  Serial.begin(115200);

  m1.begin();
  m3.begin();

  // Only M1 and M3 can use POSITION_AND_DUTY mode.
  // After change motor mode when uploading done you should turn off board 1 time.
  m1.setMotorMode(POSITION_AND_DUTY);
  m3.setMotorMode(POSITION_AND_DUTY);

  m1.setDuty(0);
  m3.setDuty(0);

  m1.setOffset(0, 0, 0);
  m3.setOffset(0, 0, 0);

}

void control_Flipper() {
  // setDuty: range -16999 to 16999.
  m1.setDuty(0);
  m3.setDuty(0);

  Serial.print("m1->Offset = ");  Serial.print(m1.getOffset()); Serial.print("\t");
  Serial.print("m1->k_angle = "); Serial.print(m1.getKAngle()); Serial.print("\t");
  Serial.print("m1->POSITION = "); Serial.println(m1.position);

  Serial.print("m3->Offset = ");  Serial.print(m3.getOffset()); Serial.print("\t");
  Serial.print("m3->k_angle = "); Serial.print(m3.getKAngle()); Serial.print("\t");
  Serial.print("m3->POSITION = "); Serial.println(m3.position);
  delay(10);
}
