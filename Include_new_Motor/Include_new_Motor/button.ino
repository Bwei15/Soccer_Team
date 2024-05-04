const int btnPIN = 41;


void setupButton() {
  pinMode(btnPIN, INPUT_PULLUP);
}

void checkButton() {
  if (digitalRead(btnPIN) == LOW) {
    Motor(motorVRpin[0], motorVRpin[1], motorVRpin[2], 0);  //VR
    Motor(motorHRpin[0], motorHRpin[1], motorHRpin[2], 0);  //HR
    Motor(motorVLpin[0], motorVLpin[1], motorVLpin[2], 0);  //VL
    while (digitalRead(btnPIN) == LOW) {
      pause = false;
    }
  }

}
