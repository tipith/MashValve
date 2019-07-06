# MashValve
Arduino control for a mash valve

# Features
- DC motor control with a tachometer feedback
- Limit switch and position calibration during startup
- Two inputs which set the target valve position
   * PWM input
   * Manual control with buttons
   * Manual control fallback to PWM after a fixed delay
- Modular architecture for easier prototyping

# Dependencies
- https://github.com/PaulStoffregen/Encoder
- https://github.com/Saeterncj/MX1508
- https://github.com/br3ttb/Arduino-PID-Library
 
Download master as a zip and Sketch -> Include Library -> Add .ZIP Library
