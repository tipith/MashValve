#include "src/ValveMotor.h"
#include "src/ValveMotorBuilder.h"
#include "src/InputSourceManual.h"
#include "src/InputSourceManualPoller.h"
#include "src/InputSourcePWM.h"
#include "src/MotorDriverMX1508.h"
#include "src/MotorEncoder.h"
#include "src/ControlStrategyBasic.h"

#define PIN_MOTOR_A 9
#define PIN_MOTOR_B 10
#define PIN_HALL_A 2
#define PIN_HALL_B 3
#define PIN_INPUT_PWM 4
#define PIN_BUTTON_OPEN 5
#define PIN_BUTTON_CLOSE 6

#define PULSER_PER_REV 5
#define POSITIONS_NUM 1024
#define MANUAL_CONTROL_ACTIVE_FOR_MS 30*1000

bool ledState = true;
ValveMotor* motor;
MotorDriverMX1508 driver(PIN_MOTOR_A, PIN_MOTOR_B);
MotorEncoder encoder(PIN_HALL_A, PIN_HALL_B, PULSER_PER_REV);
InputSourceManualPoller input1(PIN_BUTTON_CLOSE, PIN_BUTTON_OPEN, MANUAL_CONTROL_ACTIVE_FOR_MS);
InputSourcePWM input2(PIN_INPUT_PWM);
ControlStrategyBasic controller(POSITIONS_NUM);


void setup()
{
  Serial.begin(115200);
  pinMode(LED_BUILTIN, OUTPUT);

  motor = &ValveMotorBuilder()
              .withDriver(driver)
              .withEncoder(encoder)
              .usingInput(input1, PRIO_HIGH)
              .usingInput(input2, PRIO_LOW)
              .withController(controller)
              .build();
  motor->calibrate();
}

void loop()
{
  ledState = !ledState;
  digitalWrite(LED_BUILTIN, ledState);
  motor->do_work();
  motor->print_state();
  delay(100);
}
