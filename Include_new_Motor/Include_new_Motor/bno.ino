void calibBno055() {
  Serial.println("Orientation Sensor Test");
  Serial.println("");
  /* Initialise the sensor */
  if (!bno.begin(8)) {
    /* There was a problem detecting the BNO055 ... check your connections */
    Serial.print("Ooops, no BNO055 detected ... Check your wiring or I2C ADDR!");
    // while (1);
  }
  delay(1000);
  /* Use external crystal for better accuracy */
  bno.setExtCrystalUse(true);
  /* Display some basic information on this sensor */
}

float getDegrees() {
  sensors_event_t event;
  bno.getEvent(&event);
  gyroDegrees = (float)event.orientation.x - ausrichten;
  // Serial.print("evet: ");
  // Serial.println((float)event.orientation.x);

  if (gyroDegrees < -180) gyroDegrees += 360;
  if (gyroDegrees > 180) gyroDegrees -= 360;

  gyroDegrees = gyroDegrees / 0.9;
  return (gyroDegrees);
  Serial.println(gyroDegrees);
}

float hochheben() {
  sensors_event_t gravityData;
  bno.getEvent(&gravityData, Adafruit_BNO055::VECTOR_GRAVITY);
  hoch = (float)gravityData.acceleration.z;
  return hoch;
}


