/* driving.ino
 *  This code allows the robot to drive in a straight line and then turn in place.
 *  Distances and angles are converted to encoder ticks, and the robot
 *  runs the PID controller until the average of both encoders reaches
 *  that many ticks.
 */

int ticks_per_cm = 61;     // encoder ticks per cm of travel (measured)
float track_width_cm = 17.2;   // distance between the two wheels (measured)

int drive_speed = 1500;    // straight driving speed [ticks/sec]
int turn_speed = 400;      // turning speed [ticks/sec]

/* Drive straight a set distance
 *  distance_cm: how far to drive in cm (negative = backwards)
 */
void driveDistance(int distance_cm) {
  long target = abs(ticks_per_cm * distance_cm);  // ticks to travel (always positive)

  // Forward if distance is positive, backward if negative
  int speed = (distance_cm > 0) ? drive_speed : -drive_speed;

  long start_l = enc_l.read();   // encoder counts at the start
  long start_r = enc_r.read();
  long traveled = 0;             // average ticks traveled so far

  resetPID();

  while (traveled < target) {
    updatePID(speed, speed, false);

    long moved_l = enc_l.read() - start_l;
    long moved_r = enc_r.read() - start_r;
    traveled = (abs(moved_l) + abs(moved_r)) / 2;  // average of both wheels
  }

  motorHalt(LMOTOR);
  motorHalt(RMOTOR);
  delay(500);  
}

/* Spin in place
 *  angle_deg: degrees to turn (positive = counterclockwise, negative = clockwise)
 */
void rotateCCW(float angle_deg) {
  // Each wheel travels along a circle with diameter = track width
  long target = PI * track_width_cm * ticks_per_cm * abs(angle_deg) / 360;
  Serial.print("target");
Serial.print(target);
  // CCW if angle is positive, CW if negative
  int speed = (angle_deg > 0) ? turn_speed : -turn_speed;

  long start_l = enc_l.read();
  long start_r = enc_r.read();
  long traveled = 0;

  resetPID();

  while (traveled < target) {
    updatePID(-speed, speed, false);   // wheels spin in opposite directions

    long moved_l = enc_l.read() - start_l;
    long moved_r = enc_r.read() - start_r;
    traveled = (abs(moved_l) + abs(moved_r)) / 2;
  }

  motorHalt(LMOTOR);
  motorHalt(RMOTOR);
  delay(500);
}