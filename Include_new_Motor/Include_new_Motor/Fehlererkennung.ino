void fehler(int SrfRechts, int SrfVorne, int SrfBack, int SrfKugel) {
  if (SrfRechts == -1) {
    def_s_rechts = true;
    // Serial.println("FEHLER - SENSOR RECHTS AUSGEFALLEN !");
  } else {
    def_s_rechts = false;
  }

  if (SrfVorne == -1) {
    def_s_vorne = true;
    // Serial.println("FEHLER - SENSOR VORNE AUSGEFALLEN !");
  } else {
    def_s_vorne = false;
  }

  if (SrfBack == -1) {
    def_s_hinten = true;
    // Serial.println("FEHLER - SENSOR HINTEN AUSGEFALLEN !");
  } else {
    def_s_hinten = false;
  }

  if (SrfKugel == -1) {
    def_s_ball = true;
    // Serial.println("FEHLER - BALL AUSGEFALLEN !");
  } else {
    def_s_ball = false;
  }

  if (dir0 == 255) {
    def_s_unten = true;
    Serial.println("FEHLER - SENSOR UNTEN [IR] AUSGEFALLEN !");
  } else {
    def_s_unten = false;
  }

  if (dir1 == 255) {
    def_s_oben = true;
    Serial.println("FEHLER - SENSOR OBEN [IR] AUSGEFALLEN !");
  } else {
    def_s_oben = false;
  }

  if(def_s_ball && def_s_hinten && def_s_rechts && def_s_vorne){
    Serial.println("FEHLER    -   US AUSGEFALLEN");
  }
}