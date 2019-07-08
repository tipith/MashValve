#include "src/ValveMotor.h"
#include "src/ValveMotorBuilder.h"
#include "src/InputSourceManualPoller.h"
#include "src/InputSourcePWM.h"
#include "src/MotorDriverMX1508.h"
#include "src/MotorEncoder.h"
#include "src/ControlStrategyBasic.h"
#include "src/ControlStrategyPID.h"
#include "src/LimitSwitch.h"

#define PIN_MOTOR_A 9
#define PIN_MOTOR_B 10
#define PIN_HALL_A 2
#define PIN_HALL_B 3
#define PIN_INPUT_PWM 4
#define PIN_BUTTON_OPEN 5
#define PIN_BUTTON_CLOSE 6
#define PIN_LIMIT_SWITCH 7

// JGY-371, 40rpm
#define GEAR_REDUCTION 150
#define PULSES_PER_REV_PRE 11
#define PULSES_PER_REV ((long)PULSES_PER_REV_PRE * (long)GEAR_REDUCTION) // after gearbox
#define FULLSCALE_MM   18  // assuming 1 mm rise on threaded rod
#define POSITIONS_NUM ((long)PULSES_PER_REV * (long)FULLSCALE_MM)

ValveMotor* motor;
MotorDriverMX1508 driver(PIN_MOTOR_A, PIN_MOTOR_B);
MotorEncoder encoder(PIN_HALL_A, PIN_HALL_B, PULSES_PER_REV);
InputSourceManualPoller input1(PIN_BUTTON_CLOSE, PIN_BUTTON_OPEN);
InputSourcePWM input2(PIN_INPUT_PWM);
ControlStrategyBasic controller(POSITIONS_NUM);
ControlStrategyPID controllerpid(0.4, 0.1, 0.01, POSITIONS_NUM);
LimitSwitch limit(PIN_LIMIT_SWITCH, 200);

void setup()
{
  Serial.begin(115200);
  motor = &ValveMotorBuilder()
              .withDriver(driver)
              .withEncoder(encoder)
              .addInput(input1)
              .addInput(input2)
              .withController(controllerpid)
              .withLimitSwitch(limit)
              .build();
  motor->calibrate();
}

void loop()
{
  motor->do_work();
  motor->print_state();
  delay(10);
}
