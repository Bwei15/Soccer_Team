




int ballPosition() {
   tcaselect(1);
    input=seeker.readSensorValues();
     dir1 = input.direction;
       Serial.print("ObenSeeker:");
    Serial.print(dir1);
  Serial.print("  /  ");
  Serial.print("seeker strength:");
  Serial.println(strength1);
  tcaselect(0);
  input = seeker.readSensorValues();
  dir0 = input.direction;
  strength0 = input.strengthAll;
  Serial.print("UntenSeeker:");
  Serial.print(dir0);
  Serial.print("  /  ");
  Serial.print("seeker strength:");
  Serial.println(strength0);
  Serial.println("------------------------------");
  strength1 = input.strengthAll;
  //---------------ballpositionen-------------------------
  if((dir0 != 255) && (dir1 != 255)){
    if((strength0 >= 20)||(strength1 >= 20)){
     if((dir0 == 5)&&(dir1 == 5)){
      return 0; 
     }
     else if((dir1 == 5)&&(dir0 == 6)){
      return 0;
     }
     else if((dir1 == 6)&&(dir0 == 5)){
      return 270;
     }
     else if((dir0 == 4)&&(strength0 >= 100)){
      return 270;
     }
     else if(dir0 == 7){
      return 90;
     }
     else if(dir1 == 4){
      return 90;
     }
//     else if(dir0 == 3){
//      return 270;
//     }
     else if(dir1 == 7){
      return 270;
     }
     else if((dir0 <= 2)&&(dir0 >= 8)){
      return 180;
     }
     else{
      return 180;
     }
  }} else if((dir1 == 255)&&(dir0 != 255)){
  if (strength0 >= 20) {
    if (dir0 == 1) {
      return 180;
    } else if (dir0 == 2) {
      return 180;
    } else if (dir0 == 3) {
      return 270;
    } else if (dir0 == 4) {
      return 270;
    } else if (dir0 == 5) {
      return 0;
    } else if ((dir0 == 6) && (strength0 > 144)) {
      return 0;
    } else if ((dir0 == 6) && (strength0 < 144)) {
      return 90;
    } else if (dir0 == 7) {
      return 90;
    } else if (dir0 == 8) {
      return 180;
    } else if (dir0 == 9) {
      return 180;
    } else {
      return 180;
    }
  } else {
    return 180;
  }
  }
}


//------------------------------------------------------

