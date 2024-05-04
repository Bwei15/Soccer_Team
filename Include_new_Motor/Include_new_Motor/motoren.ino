void Motor(int A, int B, int PWM, int speed) {
  if (speed == 0) {
    digitalWrite(A, LOW);
    digitalWrite(B, LOW);
  }
  if (speed >= minSpeed) {
    analogWrite(PWM, speed);
    digitalWrite(A, HIGH);
    digitalWrite(B, LOW);
  }
  if (speed <= (minSpeed * -1)) {
    analogWrite(PWM, abs(speed));
    digitalWrite(A, LOW);
    digitalWrite(B, HIGH);
  }
}




//allgemeine Fahrfunktion mit Winkel
void degreeDrive(float degrees, float speedy, float addieren) {
  float gyroscope = pdOutput;
  degrees = degrees + gyroscope;
  
  //Motorversatz
  motorVR = sin((degrees - 60) / 180 * PI) * speedy;
  motorHR = sin((degrees - 180) / 180 * PI) * (speedy*0.5);
  motorVL = sin((degrees - 300) / 180 * PI) * speedy;


  // Der Maximale Motorwert wird bestimmt, um zu testen, ob ein Motor schneller dreht als der maximale Wert es zulässt
  int motorMAX = abs(motorVR);
  if (abs(motorHR) > motorMAX) motorMAX = abs(motorHR);
  if (abs(motorVL) > motorMAX) motorMAX = abs(motorVL);

  // Falls ein Motor höher liegt als der maximale Wert, so werden alle Motoren mit maxSpeed mutlipliziert und mit dem höchsten Motorwert geteilt
  // Dadurch entspricht der höchste Motorwert "maxSpeed" und das Verhältnis aller Motoren bleibt gleich
  if (motorMAX > maxSpeed) {
    motorVR = motorVR * maxSpeed / motorMAX;
    motorHR = motorHR * maxSpeed / motorMAX;
    motorVL = motorVL * maxSpeed / motorMAX;
  }

  gyroscope = gyroscope + addieren;
  //wir könnten vllt versuchen mithilfe von so einem system die Ausrichtung des Gyros zu verändern.
  //Also zB um 30 Grad nach links gucken um zum tor zu schauen

  motorVR += gyroscope;
  motorHR += gyroscope;
  motorVL += gyroscope;

  // Falls ein Motor höher liegt als der maximale Wert, so werden alle Motoren mit maxSpeed mutlipliziert und mit dem höchsten Motorwert geteilt
  // Dadurch entspricht der höchste Motorwert "maxSpeed" und das Verhältnis aller Motoren bleibt gleich
  if (motorMAX > maxSpeed) {
    motorVR = motorVR * maxSpeed / motorMAX;
    motorHR = motorHR * maxSpeed / motorMAX;
    motorVL = motorVL * maxSpeed / motorMAX;
  }

  // Die Rotation wird im Nachhinein aufaddiert
  // Da die Motortreiber sich erst ab 10% der Spannung aktivieren muss ein Wert von 25 (ca 10% von 255) addiert werden, um die Motoren am "Nullpunkt" zu haben
  if (motorVR > 0) motorVR += minSpeed;
  else motorVR -= minSpeed;
  if (motorHR > 0) motorHR += minSpeed;
  else motorHR -= minSpeed;
  if (motorVL > 0) motorVL += minSpeed;
  else motorVL -= minSpeed;

  max_speed = max(abs(motorVR), max(abs(motorHR), abs(motorVL)));
  if (max_speed > 255) {
    motorVR = (float)motorVR / (float)max_speed * 255;
    motorHR = (float)motorHR / (float)max_speed * 255;
    motorVL = (float)motorVL / (float)max_speed * 255;
  }


  // Ein Rad dreht sich immer mit speedy (nicht langsamer als die festgelegte geschwindigkeit)
  if (max_speed < speedy) {
    motorVR = (float)motorVR / (float)max_speed * speedy;
    motorHR = (float)motorHR / (float)max_speed * speedy;
    motorVL = (float)motorVL / (float)max_speed * speedy;
  }
  //Zum Schluss sollen zu kleine Motorwerte auf 0 gesetzt werden, um zu schwache Ansteuerungen zu verhindern, welche eher nur zu Zuckungen und weniger zum sauberen Fahren führen würden
  if (abs(motorVR) < (minSpeed + 2)) motorVR = 0;
  if (abs(motorHR) < (minSpeed + 2)) motorHR = 0;
  if (abs(motorVL) < (minSpeed + 2)) motorVL = 0;
  
   Serial.println(motorVR);
   Serial.println(motorVL);
  Serial.println(motorHR);
  
  if (degrees == 90 || degrees == 270) {
    motorVR = motorVR * 0.3;
    motorHR = motorHR * 0.3;
    motorVL = motorVL * 0.3;
  }
}



void Drive() {
  Motor(motorVRpin[0], motorVRpin[1], motorVRpin[2], motorVR);  //VR
  Motor(motorHRpin[0], motorHRpin[1], motorHRpin[2], motorHR);  //HR
  Motor(motorVLpin[0], motorVLpin[1], motorVLpin[2], motorVL);  //VL
}
