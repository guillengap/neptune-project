#include <Notecard.h>
#include <HardwareSerial.h>
#include <math.h>

#define RED_LED D5 //Connect the LED Grove module to Pin 5
#define FLAME_SENSOR D2 //connect FLAME SENSOR to digital pin 2
#define BUZZER D0 //connect Grove - BUZZER to pin 0

#define NOTE_PRODUCT_UID "com.your-company.your-name:your_product"

#define usbSerial Serial

HardwareSerial txRxPinsSerial(0);

int flameValue;
const int thresholdvalue=10;         //The threshold for which the LED should turn on. 
float Rsensor; //Resistance of sensor in K
int getSensorInterval();

Notecard notecard;

void setup() {
  pinMode(FLAME_SENSOR, INPUT);
  pinMode(RED_LED,OUTPUT);            
  pinMode(BUZZER,OUTPUT);
  digitalWrite(RED_LED,LOW);
  digitalWrite(BUZZER,LOW);

  // And configure txRxPinsSerial on pins RX=D6, TX=D7
  txRxPinsSerial.begin(9600, SERIAL_8N1, 8, 9);
  txRxPinsSerial.print("MySerial1");

  usbSerial.begin(115200);
  while (!usbSerial) {
      ; // wait for serial port to connect. Needed for native USB
  }
  usbSerial.println("Starting...");

  notecard.begin(txRxPinsSerial, 9600);
  notecard.setDebugOutputStream(usbSerial);

  J *req = notecard.newRequest("hub.set");
  if (req != NULL) {
      JAddStringToObject(req, "product", NOTE_PRODUCT_UID);
      JAddStringToObject(req, "mode", "continuous");
      JAddBoolToObject(req, "sync", true); 
      notecard.sendRequest(req);
  }
}

void loop() {
  int sensorValue = analogRead(A9); 
  Rsensor=(float)(4095-sensorValue)*10/sensorValue;
  if(Rsensor>thresholdvalue)
  {
    digitalWrite(RED_LED, HIGH);
  }
  else
  {
    digitalWrite(RED_LED, LOW);
  }

  if(isFlameDetected())
  turnOnBUZZER();
  else turnOffBUZZER();

  usbSerial.print("Flame = ");
  usbSerial.println(flameValue);
  usbSerial.print("Resistance = ");
  usbSerial.println(Rsensor);

  J *req = notecard.newRequest("note.add");
  if (req != NULL) {
    JAddStringToObject(req, "file", "sensors.qo");
    JAddBoolToObject(req, "sync", true);
    J *body = JAddObjectToObject(req, "body");
    if (body) {
        JAddNumberToObject(body, "flame", flameValue);
        JAddNumberToObject(body, "resistance", Rsensor);
    }
    notecard.sendRequest(req);
  }
  //delay(15000);
  int sensorIntervalSeconds = getSensorInterval();
  usbSerial.print("Delaying ");
  usbSerial.print(sensorIntervalSeconds);
  usbSerial.println(" seconds");
  delay(sensorIntervalSeconds * 1000);
}

void turnOnBUZZER()
{
    flameValue=1;
    analogWrite(BUZZER,150);
}

void turnOffBUZZER()
{
    flameValue=0;
    analogWrite(BUZZER,0);
}

boolean isFlameDetected()
{
    if(digitalRead(FLAME_SENSOR))
    return false;
    else return true;
}

// This function assumes you’ll set the reading_interval environment variable to
// a positive integer. If the variable is not set, set to 0, or set to an invalid
// type, this function returns a default value of 60.
int getSensorInterval() {
  int sensorIntervalSeconds = 60;
  J *req = notecard.newRequest("env.get");
  if (req != NULL) {
      JAddStringToObject(req, "name", "reading_interval");
      J* rsp = notecard.requestAndResponse(req);
      int readingIntervalEnvVar = atoi(JGetString(rsp, "text"));
      if (readingIntervalEnvVar > 0) {
        sensorIntervalSeconds = readingIntervalEnvVar;
      }
      notecard.deleteResponse(rsp);
  }
  return sensorIntervalSeconds;
}

