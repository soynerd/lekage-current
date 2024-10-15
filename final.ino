#include <Wire.h>
#include <Adafruit_ADS1X15.h>

Adafruit_ADS1115 ads;

const float FACTOR = 30; //20A/1V from teh CT

const float multiplier = 0.00005;

const int redLed=5;
const int greenLed=7;
const int blueLed=8;


void setup() {
  Serial.begin(9600);

  ads.setGain(GAIN_FOUR);      // +/- 1.024V 1bit = 0.5mV
  ads.begin();
  pinMode(redLed, OUTPUT);
  pinMode(greenLed, OUTPUT);
  pinMode(blueLed, OUTPUT);
  

}

void printMeasure(String prefix, float value, String postfix)
{
  Serial.print(prefix);
  Serial.print(value, 3);
  Serial.println(postfix);
}

void loop() {
  float currentRMS = getcurrent();
  if(currentRMS > 0.05){
    digitalWrite(blueLed, HIGH);
  }
  if(currentRMS > 0.3){
    digitalWrite(redLed, HIGH);
    digitalWrite(greenLed, LOW);
  }else{
    digitalWrite(redLed, LOW);
    digitalWrite(greenLed, HIGH);
  }

  printMeasure("Irms: ", currentRMS, "A");
  delay(1000);

}

float getcurrent()
{
  float voltage;
  float current;
  float sum = 0;
  long time_check = millis();
  int counter = 0;

  while (millis() - time_check < 1000)
  {
    voltage = ads.readADC_Differential_0_1() * multiplier;
    current = voltage * FACTOR;
    //current /= 1000.0;

    sum += sq(current);
    counter = counter + 1;
  }

  current = sqrt(sum / counter);
  return (current);
}
