/* This program allows you to set DMX channels over the serial port.
**
** After uploading to Arduino, switch to Serial Monitor and set the baud rate
** to 9600. You can then set DMX channels using these commands:
**
** <number>c : Select DMX channel
** <number>v : Set DMX channel to new value
**
** These can be combined. For example:
** 100c355w : Set channel 100 to value 255.
**
** For more details, and compatible Processing sketch,
** visit http://code.google.com/p/tinkerit/wiki/SerialToDmx
**
** Help and support: http://groups.google.com/group/dmxsimple       */

#include <DmxSimple.h>
//Joystick
  const int SW_pin = 3; // digital pin connected to switch output
  const int X_pin = 0; // analog pin connected to X output
  const int Y_pin = 1; // analog pin connected to Y output
  
void setup() {
  Serial.begin(9600);
  Serial.println("SerialToDmx ready");
  Serial.println();
  Serial.println("Syntax:");
  Serial.println(" 123c : use DMX channel 123");
  Serial.println(" 45w  : set current channel to value 45");

  //Joystick
  pinMode(SW_pin, INPUT);
  digitalWrite(SW_pin, HIGH);
}

int value = 0;
int channel;

void loop() {
  int c;
  
  //while(!Serial.available());
  c = Serial.read();
  
  if ((c>='0') && (c<='9')) {
    value = 10*value + c - '0';
  } else {
    if (c=='c') channel = value;
    else if (c=='w') {
      DmxSimple.write(channel, value);
      Serial.println();
    }
    value = 0;
  }

  Serial.println(analogRead(X_pin));
}
