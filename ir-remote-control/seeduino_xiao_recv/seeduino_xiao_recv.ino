#include <IRremote.h>			
#define Button_1 0x10 // key_1 hex value
#define Button_2 0x810 //key_2 hex value
#define Button_3 0x410 // key_3 hex value

int SENSOR = 10; 			// IR receiver sensor to pin 10
IRrecv irrecv(SENSOR);			
decode_results codigo;			// decode_results class

int LEDLAMP_PIN1 = 1;
int LEDLAMP_PIN2 = 2;
int SENSOR_GND = 9;

void setup() {
  Serial.begin(9600);			
  irrecv.enableIRIn();			// initialize data reception
  pinMode(LEDLAMP_PIN1, OUTPUT);		// pin 1 as output
  pinMode(LEDLAMP_PIN2, OUTPUT);		// pin 2 as output
  pinMode(SENSOR_GND, OUTPUT);		// pin 3 used as gnd
} 

void loop() {
  digitalWrite(SENSOR_GND, LOW);
  digitalWrite(LEDLAMP_PIN2, LOW);  
  if (irrecv.decode(&codigo)) {			// if there is already decoded data
    Serial.println(codigo.value, HEX);		// print the value on monitor - hex
      if (codigo.value == Button_1){
      analogWrite(LEDLAMP_PIN1, 255);
      }			// if code is button 1

      if (codigo.value == Button_2){
      analogWrite(LEDLAMP_PIN1, 0);
      }			// if code is button 2

    if (codigo.value == Button_3){
      for(int i=0; i<=2; i++){
        for(int i=0; i<=255; i++) {
            analogWrite(LEDLAMP_PIN1, i);
            delay(10);
        }
        for(int i=255; i>=0; i--) {
            analogWrite(LEDLAMP_PIN1, i);
            delay(10);
        }
      }  
    }			// if code is button 3
    irrecv.resume();				// summarizes data acquisition
  }
  delay (100);					
}

