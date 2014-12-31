#include <aJSON.h>

#define LED 13
#define VREF 6
#define IN1 5
#define IN2 2
#define TENMP_SENSOR A3
aJsonStream serial_stream(&Serial);

void setup()
{
  Serial.begin(115200);
  pinMode(LED, OUTPUT);  //LED
  pinMode(VREF, OUTPUT);  //Vref
  pinMode(IN1, OUTPUT);  //IN1
  pinMode(IN2, OUTPUT);  //IN2
  pinMode(TENMP_SENSOR, INPUT);  //IN2
}

void loop()
{
  if (serial_stream.available()) {
    /* First, skip any accidental whitespace like newlines. */
    serial_stream.skip();
  }
  
  if (serial_stream.available()) {
    aJsonObject *msg = aJson.parse(&serial_stream);  
    if(aJson.getObjectItem(msg, "temp")->type == aJson_String){
      if("heat" == aJson.getObjectItem(msg, "temp")->valuestring){
        digitalWrite(13, HIGH);
        heat(255);
      }
      else if("cool" == aJson.getObjectItem(msg, "temp")->valuestring){
        digitalWrite(13, HIGH);
        cool(255);
      }
      else if("stop" == aJson.getObjectItem(msg, "temp")->valuestring){
        digitalWrite(13, HIGH);
        tempstop();
      }
    }
  }
}

float getTemperature(){
  int ar = analogRead(TENMP_SENSOR);
  ar += analogRead(TENMP_SENSOR);
  ar += analogRead(TENMP_SENSOR);
  return -0.39140856 * ar /3 + 207.73;
}

void heat(int d){
  analogWrite(VREF, d);
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
}

void cool(int d){
  analogWrite(VREF, d);
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
}

void tempstop(){
  analogWrite(VREF, 0);
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
}
