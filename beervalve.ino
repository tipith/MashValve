#include <Encoder.h> /* https://github.com/PaulStoffregen/Encoder */
#include <MX1508.h> /* https://github.com/Saeterncj/MX1508 */
#include "motor.h"

#define PIN_HALL_A    2
#define PIN_HALL_B    3
#define PIN_MOTOR_A   9
#define PIN_MOTOR_B   10


bool ledState = true;

Encoder tacho(PIN_HALL_A, PIN_HALL_B);
MX1508 motor(PIN_MOTOR_A, PIN_MOTOR_B, FAST_DECAY, PWM_2PIN);

void setup()
{
  Serial.begin(115200);
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, ledState);

  /* Drive motor until limit switch triggers */

  /* Set known position */
  tacho.write(0);
}

void loop()
{
  ledState = !ledState;
  digitalWrite(LED_BUILTIN, ledState);
  delay(100);

  char text[64];

  static unsigned long next_run = 0;
  static bool cwDirection = true;
  static int pwm = 1;

  if (next_run < millis())
  {
    if (cwDirection && pwm++ > 100 )
    {
      cwDirection = false;
    }
    else if (!cwDirection && pwm-- < -100)
    {
      cwDirection =  true;
    }

    motor.motorGo(pwm);
    next_run = millis() + 50;
  }

  sprintf(text, "led %u, encoder %u, motor %i %i\n", ledState, tacho.read(), pwm, motor.getPWM());
  Serial.print(text);
}
