#include <Adafruit_DotStar.h>

#define NUMPIXELS 1 // Number of LEDs in strip

// Here's how to control the LEDs from any two pins:
#define DATAPIN   7
#define CLOCKPIN   8

Adafruit_DotStar strip = Adafruit_DotStar(
  NUMPIXELS, DATAPIN, CLOCKPIN, DOTSTAR_BGR);

//  Det ovan behövs för att kunna styra dotstar leddioden på kretskortet.


//
// röda ledlampa på kretskortet
const int ledPin = 13; 
const int pwmPin = A2;  // A2 = utgång 0 på trinket kretskort
const int tankSensor = A3;

// the setup function runs once when you press reset or power the board
void setup() {
  strip.begin(); // Initialize pins for output
  strip.show();  // Turn all LEDs off ASAP

  // initialize digital pin 13 as an output.
  pinMode(ledPin, OUTPUT);

  pinMode(pwmPin, OUTPUT);  // sets the pin as output
  pinMode(tankSensor, INPUT);       // läser tanksensor

  digitalWrite(ledPin, LOW);    // turn the LED off by making the voltage LOW
}

// the loop function runs over and over again forever
int sensorValue = 0;
int sensorValueInverted = 0;
int val = 0;

int valMin = 130;
int valMax = 255;

// sensorValue     pwmValue      Tankmätare
//                      232             3/4
//                      192             1/2
//                      163             1/4
//                      132               0
 
void loop() {
  sensorValue = analogRead(tankSensor);

  // om tanksensor går sönder eller kabel lossnar kommer mätvärde över 545 att läsas.
  // sätt då värde till 545 så att inte mappningsfunktionen inte ska ge högt maxvärde.
  // tänd även röda dioder så att det syns att något inte är rätt.
  if (sensorValue > 545) {
    sensorValue = 545;

    digitalWrite(ledPin, HIGH);    // turn the LED on
    strip.setPixelColor(0, 0xFF0000); // red
    strip.show(); 
  } else {
    digitalWrite(ledPin, LOW);    // turn the LED off by making the voltage LOW
    strip.setPixelColor(0, 0x000000); // off
    strip.show();
  }

  // Invertera mätvärdet, kompensera för att sensorn ger högre ohm vid tom tank än vid full.
  sensorValueInverted = 545 - sensorValue;

  val = map(sensorValueInverted,0,545,valMin,valMax);
  analogWrite(pwmPin, val);

  Serial.print("sensorValue = ");
  Serial.print(sensorValue);
  Serial.print(", DAC value = ");
  Serial.print(val);
  Serial.println("");

  delay(1000);
}
