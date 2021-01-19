#define BLYNK_PRINT Serial
#include <ESP8266_Lib.h>
#include <BlynkSimpleShieldEsp8266.h>
#include <Servo.h>
#include <SoftwareSerial.h>
#define ESP8266_BAUD 9600

char auth[] = "dIIl3-EC24JcWlrHPGMJHsBYrIacF9Ht";
char ssid[] = "FISIO ORANGE";
char pass[] = "FS151515";

Servo servo1;
int pos = 0;
int data = 1000;

unsigned long currentTime = 0;
unsigned long previousTime = 0;
unsigned long inputTime = 3000;

bool hidup = false;

SoftwareSerial EspSerial(2, 3); // RX, TX

ESP8266 wifi(&EspSerial);

BlynkTimer timer;

void myTimerEvent()
{
  Blynk.virtualWrite(V5, millis() / 1000);
}

BLYNK_WRITE(V4){
  int angka = param.asInt();
  Serial.println(angka);

  inputTime = angka * 1000;
}

BLYNK_WRITE(V9){
  if(hidup){
    Serial.println("Tombol ditekan");
  } else {
    Serial.println("Tombol ditekan");
  }
  hidup = !hidup;
}

void setup()
{
  Serial.begin(9600);

  EspSerial.begin(ESP8266_BAUD);
  delay(10);

  Blynk.begin(auth, wifi, ssid, pass);
  
  timer.setInterval(1000L, myTimerEvent);

  servo1.attach(9);
}

void loop()
{
  Blynk.run();
  timer.run(); // Initiates BlynkTimer

  if(hidup){
    servoMotor();
  }
}

void servoMotor(){
  currentTime = millis();
  
  while(Serial.available()){
    data = Serial.parseInt() * 1000;
    inputTime = data;
  }

  if (currentTime - previousTime >= inputTime){
    Serial.print(inputTime/1000);
    Serial.println(" detik");

    for (pos = 0; pos <= 180; pos += 1) { // goes from 0 degrees to 180 degrees
      // in steps of 1 degree
      servo1.write(pos);              // tell servo to go to position in variable 'pos'
      delay(5.5);                       // waits 15ms for the servo to reach the position
    }
    for (pos = 180; pos >= 0; pos -= 1) { // goes from 180 degrees to 0 degrees
      servo1.write(pos);              // tell servo to go to position in variable 'pos'
      delay(5.5);                       // waits 15ms for the servo to reach the position
    }
    
    previousTime = currentTime;
  }
}
