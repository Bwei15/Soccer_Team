#include <Wire.h>
#include <Adafruit_BNO055.h>
#include <sys/types.h>
#include <HTInfraredSeeker.h>
#include <Arduino.h>
#include <cmath>
#include <math.h>




Adafruit_BNO055 bno = Adafruit_BNO055(55, 0x28);


//ir
HTInfraredSeeker seeker;
InfraredInput input;
#define TCAADDR 0x70

void tcaselect(uint8_t i) {
  if (i > 7) return;

  Wire.beginTransmission(TCAADDR);
  Wire.write(1 << i);
  Wire.endTransmission();
}

int buttonPin = 0;
int dir1;
int dir0;
float strength0;
float strength1;
int backDirection;
int frontDirection;
int frontPower = input.strengthAll;
int backPower = input.strengthAll;
float ausrichten;
struct Vector2D {
  float x;
  float y;
};

int botX = 0;
int botY = 0;

int ad_drehung = 0;


///////////////////////////
int botspeed = 120;  // Geschwindigkeit des Roboters Original: (300)
//////////////////////////


const int srf08AddressFront = 0x70;
const int srf08AddressRight = 0x72;
const int srf08AddressBall = 0x74;
const int srf08AddressHinten = 0x73;


float currentInput = 0.0;
float previousInput = 0.0;
float pdOutput = 0.0;
unsigned long lastUpdateTime = 0;
int timeStep = 0;  // Zeitintervall von 100 ms zwischen den Aktualisierungen
float proportionalGain = 0.8;
float differentialGain = 50;

bool def_s_rechts = false;
bool def_s_vorne = false;
bool def_s_hinten = false;
bool def_s_ball = false;

bool def_s_oben = false;
bool def_s_unten = false;






int max_speed = 0;
float gyroDegrees;
float hoch;
float x_speed;
bool merken = true;
;
float y_speed;
float gyroRadiants;
//------------------------motoren------------------------
const uint motorVRpin[] = { 10, 5, 2 };  // CCW1, CW1, PWM1 //VR  HR
const uint motorVLpin[] = { 6, 11, 3 };  // CCW2, CW2, PWM2 //VL
const uint motorHRpin[] = { 9, 12, 4 };  // CCW3, CW3, PWM3 //hinten
const int motorOnOffPin = 30;            // ON/OFF-Pin for motor control
                                         
int SrfFront = 0;
int SrfRight = 0;
int SrfBall = 0;
int SrfHinten = 0;

int motorVL = 0;  //Speedwerte für die jeweiligen Motoren
int motorVR = 0;
int motorHR = 0;


#define maxSpeed 120
#define minSpeed 0
#define maxRotation 80

int gyro_addieren = 0;

//Ultraschall
int heading = 0;

void startMeasurement(int address) {
  Wire2.beginTransmission(address);
  Wire2.write(byte(0x00));  // Befehlsregister
  Wire2.write(byte(0x51));  // Starten der Entfernungsmessung in Zentimeter
  Wire2.endTransmission();
}

int readMeasurement(int address) {
  Wire2.beginTransmission(address);
  Wire2.write(byte(0x02));  // Zeiger auf das Register für Entfernungsmessungen setzen
  Wire2.endTransmission();

  Wire2.requestFrom(address, 2);
  if (Wire2.available() >= 2) {
    int highByte = Wire2.read();
    int lowByte = Wire2.read();
    return (highByte << 8) + lowByte;
  }
  return -1;
}

void readAllDistances() {
  // Starten der Messung für alle Sensoren
  startMeasurement(srf08AddressFront);
  startMeasurement(srf08AddressRight);
  startMeasurement(srf08AddressBall);
  startMeasurement(srf08AddressHinten);

  // Warte 70 ms für die Messungen
  //  delay(70);

  // Lesen der Ergebnisse
  SrfFront = readMeasurement(srf08AddressFront);
  SrfRight = readMeasurement(srf08AddressRight);
  SrfBall = readMeasurement(srf08AddressBall);
  SrfHinten = readMeasurement(srf08AddressHinten);

  // Verarbeiten Sie hier die Ergebnisse
}


//feld


void setup() {
  Serial.begin(115200);
  calibBno055();
  Wire2.begin();
  Wire1.begin();
  Wire.begin();
  seeker.init();
  pinMode(A17, INPUT_PULLUP);



  pinMode(motorOnOffPin, OUTPUT);
  digitalWrite(motorOnOffPin, HIGH);
  delay(1000);
  sensors_event_t event;
  bno.getEvent(&event);  // BNO daten sammeln
  gyroDegrees = event.orientation.x;
  ausrichten = gyroDegrees;
}



//----------------------------------loop----------------------------------------
bool pause = true;
void loop() {
  readAllDistances();
  checkButton();
  seeker.init();

  Serial.println(SrfBall);

  int angle = ballPosition();


  int ball = ballerkennung(SrfBall, strength0, def_s_ball);
  Serial.println(ball);
  float speed;


  
  if (angle == 180) {
    speed = botspeed + 15;
  } else {
    speed = botspeed;
  }



   float hoch = hochheben();
   if (hoch < 9.55) {
     pause = true;


  currentInput = getDegrees();  // Lese den aktuellen Gyro-Wert
  PD(currentInput, &previousInput, &pdOutput, &lastUpdateTime, timeStep, proportionalGain, differentialGain);



  //------ansteuern der Motoren------
  // Serial.println(speed)
  degreeDrive(angle, speed, ad_drehung);
  // gyro_addieren = 10;
  //degreeDrive(270,100);
  if (pause) {
    Motor(motorVRpin[0], motorVRpin[1], motorVRpin[2], 0);  //VR
    Motor(motorHRpin[0], motorHRpin[1], motorHRpin[2], 0);  //HR
    Motor(motorVLpin[0], motorVLpin[1], motorVLpin[2], 0);  //VL
  } else {
    Drive();
  }
  //----------------------------
  //Defekt Erkennen
  fehler(SrfRight, SrfFront, SrfHinten, SrfBall);
}


//------------------------------------------------------------------------------


//------ballerkennung-----

int ballerkennung(float entfernung, float strength, int def_s_ball) {
  if ((entfernung <= 8) && (strength > 142) && (def_s_ball = false)) {
    return 1;
  } else if ((entfernung >= 20) && (strength > 144)) {
    return 1;
  } else {
    return 0;
  }
}



//------------------------
