/* TME 441
*  Milestone 3 - Object manipulation
*
*  This template includes example code to test your stepper and servo motors.
*  Once you are done testing, you should create your own Arduino sketch and
*  import the rest of your reusable code (motor_control.h and other *.ino files).
*
*  @author Matthew Ford
*  @version 2026
*/
#include <Stepper.h>
#include <Servo.h>

// Global variables for the stepper motor homing function (see below)
volatile bool swPressed = false;  // Flag to indicate if the limit switch is pressed

// Initialize stepper motor with 2048 steps per revolution
// Replace In1, In2, In3, In4 with the pin numbers connected to the stepper motor driver
Stepper stm(2048, In1, In3, In2, In4);

// Initialize servo motor
Servo svm;

int main() {
  init();

  // Initialize communication interfaces
  Serial.begin(115200);  // USB serial for debugging

  /* Initialize the servo motor
  Replace PIN with the pin number connected to the servo motor control signal.
  The second and third parameters specify the minimum and maximum pulse widths
  corresponding to 0 and 180 degrees. If your servo motor does not turn far
  enough, try adjusting these values.
   */
  svm.attach(PIN, 500, 2500);

  // Testing functions. Uncomment the one you want to test.
  testStepperMotor();
  //testServoMotor();
  //testRangeSensor();

}

/*******************************************************************************
 * Testing functions
 *******************************************************************************/

// Test the stepper motor by rotating it 1 revolution clockwise and counterclockwise
void testStepperMotor() {
  stm.setSpeed(10);  // Set speed to 10 RPM
  while (true) {
    stm.step(2048);  // Rotate 1 revolution clockwise
    delay(1000);     // Wait for 1 second
    stm.step(-2048); // Rotate 1 revolution counterclockwise
    delay(1000);     // Wait for 1 second
  }
}

// Test the servo motor by moving it between 0 and 180 degrees
void testServoMotor() {
  while (true) {
    svm.write(0);    // Move servo to 0 degrees
    delay(1000);     // Wait for servo to finish moving
    svm.write(180);  // Move servo to 180 degrees
    delay(1000);
  }
}

// Get range to target with ultrasonic sensor and print to serial monitor
void testRangeSensor() {
  while (true) {
    int range = rangeToTarget();
    Serial.print("Range to target: ");
    Serial.print(range);
    Serial.println(" cm");
    delay(1000);
  }
}

/*******************************************************************************
 * Helper functions
 *******************************************************************************/

/**
 * Home a stepper motor by rotating until the limit switch is triggered,
 * then backing off slightly to release the switch and establish a known start position.
 * 
 * @param s1 The stepper motor object to home
 * @param SWITCH_PIN The pin number connected to the limit switch
 */
void findStepperHome(Stepper s1, int SWITCH_PIN) {

  s1.setSpeed(10);  // Set speed to 10 RPM

  // Initialize the limit switch
  pinMode(SWITCH_PIN, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(SWITCH_PIN), toggleSwitch, FALLING);

  while (!swPressed) {
    s1.step(5);  // Rotate by a small increment
  }

  s1.step(-10);  // Reverse a small amount to unpress the switch

  // Detach the interrupt to prevent further triggering
  detachInterrupt(digitalPinToInterrupt(SWITCH_PIN));
}

// Interrupt to toggle limit switch
void toggleSwitch() {
  swPressed = true;
}

/**
 * Measure range to target with ultrasonic sensor
 * @return Range to target in centimeters
 */
int rangeToTarget() {
  const int TRIG = 2;  // Pin connected to the ultrasonic sensor TRIG
  const int ECHO = 3;  // Pin connected to the ultrasonic sensor ECHO

  pinMode(TRIG, OUTPUT);
  pinMode(ECHO, INPUT);

  // Send trigger pulse
  digitalWrite(TRIG, HIGH);
  delayMicroseconds(15);
  digitalWrite(TRIG, LOW);

  // Measure echo pulse
  long dt = pulseIn(ECHO, HIGH);

  return dt / 58;  // Estimate range to target
}