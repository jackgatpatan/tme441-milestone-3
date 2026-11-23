/* navigation.ino
*  Functions for navigating the robot to April tags using the HuskyLens camera
*
*  @author Matthew Ford
*/


/* Find and approach the specified April tag
*
*  @param tag_id ID number of the desired tag
*  @return 
*     1 = Success
*    -1 = Searched unsuccessfully for a tag for 20 sec
*    -2 = Other error, or lost connection with HuskyLens
*/
int driveToTag(int tag_id) {

  int status;
  int range;

  status = pointToTag(tag_id);  // First search
  if (status == -2) { return -2; }
  if (status == -1) { return -1; }

  // Check range to tag
  range = rangeToTag(tag_id);

  // Drive to tag, stopping 10 cm short
  driveDistance(range-10);
Serial.print(range);
  return 1;
}

/* Rotate robot to point to the specified April tag
*
*  @param tag_id ID number of the desired tag
*  @return
*     1 = Success
*    -1 = Search unsuccessfully for 20 sec
*    -2 = Other error, or lost connection with HuskyLens
*/
int pointToTag(int tag_id) {
  int x;
  long start_time = millis();
  float range;
  
   range = rangeToTag(tag_id);
  while ((millis() - start_time) < 30000) {
    x = getObjCenterX(tag_id);

    if (x == -2) {  // HuskyLens error
      return -2;
    } else if (x == -1) {  // Tag not found
      rotateCCW(35);
      Serial.print(enc_l.read());
      Serial.print(enc_r.read());
      delay(300);
    } else if (abs(x - 160) < 13) {  // Tag is centered
      return 1;
     }else {  // Tag not centered. Minor adjustment
      float angle = ((160-x)/5.55)*(PI/180);
      float theta = atan((range*sin(angle))/(.12+range*cos(angle)));
      theta = theta / (PI/180);
      float fangle= angle/(PI/180);
      //delay(500);
      rotateCCW(theta);
      Serial.print("angle");
      Serial.print(fangle);
      Serial.print("theta");
      Serial.print(theta);
      Serial.print("range");
      Serial.print(range);
       Serial.print(enc_l.read());
      Serial.print(enc_r.read());
      delay(100); 
      
    }
  }
  
  return -1;  // Timeout
}
