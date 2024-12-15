#include <Bluepad32.h>
#include "RMRC_BreakoutV2.h"

//Define Controller
ControllerPtr myControllers[BP32_MAX_CONTROLLERS];

//Define Motors
RMRC_BreakoutV2 m1(M1M2_ADDR, MOTOR_1), m2(M1M2_ADDR, MOTOR_2),
  m3(M3M4_ADDR, MOTOR_3), m4(M3M4_ADDR, MOTOR_4);

#define MAXIMUM_SHAFT_RPM 294.0f
#define GEAR_RATIO 34.0f

void setup() {
  // Initialize serial
  Serial.begin(115200);
  while (!Serial) {
    ;
  }

  String fv = BP32.firmwareVersion();

  // To get the BD Address (MAC address) call:
  const uint8_t* addr = BP32.localBdAddress();
  for (int i = 0; i < 6; i++) {
    Serial.print(addr[i], HEX);
    if (i < 5)
      Serial.print(":");
    else
      Serial.println();
  }
  // This call is mandatory. It sets up Bluepad32 and creates the callbacks.
  BP32.setup(&onConnectedController, &onDisconnectedController);

  BP32.forgetBluetoothKeys();

  //Setup Motors
  m1.begin();
  m2.begin();
  m3.begin();
  m4.begin();

  // Only M1 and M3 can use POSITION_AND_DUTY mode.
  // After change motor mode when uploading done you should turn off board 1 time.
  m1.setMotorMode(SPEED_AND_DUTY);
  m2.setMotorMode(SPEED_AND_DUTY);
  m3.setMotorMode(POSITION_AND_DUTY);
  m4.setMotorMode(SPEED_AND_DUTY);
  //m4 motor port is broke
  //Fixing inprogress

  m1.setDuty(0);
  m2.setDuty(0);
  m3.setDuty(0);
  m4.setDuty(0);

  m1.setEncoderResolution(11.0f);
  m2.setEncoderResolution(11.0f);

  m1.setMaximumRPM(MAXIMUM_SHAFT_RPM * GEAR_RATIO);
  m2.setMaximumRPM(MAXIMUM_SHAFT_RPM * GEAR_RATIO);

  m2.setKp(0.0f);
  m2.setKi(0.0f);
  m2.setKd(0.0f);

  m1.setKp(0.0f);
  m1.setKi(0.0f);
  m1.setKd(0.0f);
}

// This callback gets called any time a new gamepad is connected.
// Up to 4 gamepads can be connected at the same time.
void onConnectedController(ControllerPtr ctl) {
  bool foundEmptySlot = false;
  for (int i = 0; i < BP32_MAX_GAMEPADS; i++) {
    if (myControllers[i] == nullptr) {
      Serial.print("CALLBACK: Controller is connected, index=");
      Serial.println(i);
      myControllers[i] = ctl;
      foundEmptySlot = true;

      // Optional, once the gamepad is connected, request further info about the
      // gamepad.
      ControllerProperties properties = ctl->getProperties();
      char buf[80];
      sprintf(buf,
              "BTAddr: %02x:%02x:%02x:%02x:%02x:%02x, VID/PID: %04x:%04x, "
              "flags: 0x%02x",
              properties.btaddr[0], properties.btaddr[1], properties.btaddr[2],
              properties.btaddr[3], properties.btaddr[4], properties.btaddr[5],
              properties.vendor_id, properties.product_id, properties.flags);
      Serial.println(buf);
      break;
    }
  }
  if (!foundEmptySlot) {
    Serial.println(
      "CALLBACK: Controller connected, but could not found empty slot");
  }
}

void onDisconnectedController(ControllerPtr ctl) {
  bool foundGamepad = false;

  for (int i = 0; i < BP32_MAX_GAMEPADS; i++) {
    if (myControllers[i] == ctl) {
      Serial.print("CALLBACK: Controller is disconnected from index=");
      Serial.println(i);
      myControllers[i] = nullptr;
      foundGamepad = true;
      break;
    }
  }

  if (!foundGamepad) {
    Serial.println(
      "CALLBACK: Controller disconnected, but not found in myControllers");
  }
}

void processGamepad(ControllerPtr gamepad) {
  char buf[256];
  snprintf(buf, sizeof(buf) - 1,
           "idx=%d, dpad: 0x%02x, buttons: 0x%04x, "
           "axis L: %4li, %4li, axis R: %4li, %4li, "
           "brake: %4ld, throttle: %4li, misc: 0x%02x, "
           "gyro x:%6d y:%6d z:%6d, accel x:%6d y:%6d z:%6d, "
           "battery: %d",
           gamepad->index(),        // Gamepad Index
           gamepad->dpad(),         // DPad
           gamepad->buttons(),      // bitmask of pressed buttons
           gamepad->axisX(),        // (-511 - 512) left X Axis
           gamepad->axisY(),        // (-511 - 512) left Y axis
           gamepad->axisRX(),       // (-511 - 512) right X axis
           gamepad->axisRY(),       // (-511 - 512) right Y axis
           gamepad->brake(),        // (0 - 1023): brake button
           gamepad->throttle(),     // (0 - 1023): throttle (AKA gas) button
           gamepad->miscButtons(),  // bitmask of pressed "misc" buttons
           gamepad->gyroX(),        // Gyro X
           gamepad->gyroY(),        // Gyro Y
           gamepad->gyroZ(),        // Gyro Z
           gamepad->accelX(),       // Accelerometer X
           gamepad->accelY(),       // Accelerometer Y
           gamepad->accelZ(),       // Accelerometer Z
           gamepad->battery()       // 0=Unknown, 1=empty, 255=full
  );
  Serial.println(buf);

  //Control Flipper
  //== Xbox A button = 0x0001 ==//
  if (gamepad->buttons() == 0x0001) {
    m3.setDuty(-16999);
  }

  //== Xbox X button = 0x0008 ==//
  if (gamepad->buttons() == 0x0008) {
    m3.setDuty(16999);
  }
  if (gamepad->buttons() == 0x0000) {
    m3.setDuty(0);
  }

  //Control Movement
  //== Xbox Dpad UP button = 0x01 ==//
  if (gamepad->dpad() == 0x01) {
    m2.setDuty(-16999);
    m1.setDuty(-16999);
  }
  //==Xbox Dpad DOWN button = 0x02==//
  if (gamepad->dpad() == 0x02) {
    m2.setDuty(16999);
    m1.setDuty(16999);
  }
  //== Xbox Dpad LEFT button = 0x08 ==//
  if (gamepad->dpad() == 0x08) {
    m2.setDuty(16999);
    m1.setDuty(-16999);
  }
  //== Xbox Dpad RIGHT button = 0x04 ==//
  if (gamepad->dpad() == 0x04) {
    m2.setDuty(-16999);
    m1.setDuty(16999);
  }
  if (gamepad->dpad() == 0x00) {
    m2.setDuty(0);
    m1.setDuty(0);
  }
}


void loop() {
  m2.setRPM(10000.0f);
  m1.setRPM(10000.0f);
  BP32.update();
  
  for (int i = 0; i < BP32_MAX_CONTROLLERS; i++) {
    ControllerPtr myController = myControllers[i];

    if (myController && myController->isConnected()) {
      processGamepad(myController);
    }
  }
}
