void PD(float value, float *valueOld, float *valuePD, unsigned long *lastTime, int timeStep, float p, float d) {
  if (*lastTime < (millis() - timeStep)) {
    float delta_time = millis() - *lastTime;
    float out_p = value * p;
    float out_d = (value - *valueOld) * d / delta_time;

    *valueOld = value;
    *valuePD = (out_p + out_d);
    *lastTime = millis();
    }
  }