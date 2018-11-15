#include <DmxSimple.h>
#include <math.h>
//Joystick
  const int JoystickClick = 7; // digital pin connected to switch output
  const int JoystickX = 0; // analog pin connected to X output
  const int JoystickY = 1; // analog pin connected to Y output
  int JoystickCenterY;
  int JoystickCenterX;
//Heat Sensor
//Used to detect a change in heat which will alter the resistive value of the Joystick
  const int therm = 2;
  int temp, lastTemp;
  double Thermistor(int RawADC) {
    double Temp;
    Temp = log(10000.0*((1024.0/RawADC-1))); 
    //         =log(10000.0/(1024.0/RawADC-1)) // for pull-up configuration
    Temp = 1 / (0.001129148 + (0.000234125 + (0.0000000876741 * Temp * Temp ))* Temp );
    Temp = Temp - 273.15;            // Convert Kelvin to Celcius
    return Temp;
  }
//DMX
  int masterChannel = 120;
  //American DJ Spot 6 Channels
  int panChannel = masterChannel + 1; //0 LEFT - 255 RIGHT
  int tiltChannel = panChannel + 1; //0 UP - 255 DOWN
  int PTZ_SpeedChannel = masterChannel + 6; //0 FAST - 255 SLOW
void setup() {

  Serial.begin(9600);
  Serial.println();
  Serial.print("Master DMX Channel:");
  Serial.println(masterChannel);

  //Read Temp
  temp = Thermistor(analogRead(therm));
  lastTemp = temp;
  Serial.print("Temperature");
  Serial.println(temp);

  //JOYSTICK - INIT
  pinMode(JoystickClick, INPUT);
  digitalWrite(JoystickClick, HIGH);
  
  //JOYSTICK - Get dead center
  JoystickCenterX = analogRead(JoystickX); // Calibrate X Axis
  JoystickCenterY = analogRead(JoystickY); // Calibrate Y Axis
  Serial.print("Joystick Dead-Center X:  ");
  Serial.println(JoystickCenterX);
  Serial.print("Joystick Dead-Center Y:  ");
  Serial.println(JoystickCenterY);
}

int lastXValue = 500;
int lastYValue = 500;
int MAX_DMX = 255;
int MIN_DMX = 0;
int OFFSET = 20;
int Xvalue, Yvalue;
void loop() {
  //Read Temp
  temp = Thermistor(analogRead(therm));
  if(temp != lastTemp){
    Serial.print("Temperature Chaned to: ");
    Serial.println(temp);
  }

  int JoystickXValue = analogRead(JoystickX)-JoystickCenterX;
  int JoystickYValue = analogRead(JoystickY)-JoystickCenterY;
  if(abs(JoystickXValue) > OFFSET){ 
    
    //Serial.println("Joystick being moved");
    
    if(JoystickXValue > 0){
      //Increase
      //Serial.println("X Value Increasing");
      Xvalue++;
    }
    else{
      //Decrease
      //Serial.println("X Value Decreasing");
      Xvalue--;
    }
    if(Xvalue > MAX_DMX){
      Xvalue = MAX_DMX;
    }
    else if (Xvalue < MIN_DMX){
      Xvalue = MIN_DMX;
    }
    DmxSimple.write(panChannel, Xvalue);
    Serial.print("X: ");
    Serial.println(Xvalue);
    //Latch last known DMX value per axis
  }
  else{
    
    //Stop sending commands
    //Serial.println("Joystick not being moved");
  }

  if(abs(JoystickYValue) > OFFSET){ 
    
    //Serial.println("Joystick being moved");
    
    if(JoystickYValue > 0){
      //Increase
      //Serial.println("Y Value Increasing");
      Yvalue++;
    }
    else{
      //Decrease
      //Serial.println("Y Value Decreasing");
      Yvalue--;
    }
    if(Yvalue > MAX_DMX){
      Yvalue = MAX_DMX;
    }
    else if (Yvalue < MIN_DMX){
      Yvalue = MIN_DMX;
    }
    DmxSimple.write(tiltChannel, Yvalue);
    Serial.print("Y: ");
    Serial.println(Yvalue);
    //Latch last known DMX value per axis
  }
  else{
    
    //Stop sending commands
    //Serial.println("Joystick not being moved");
  }
  lastXValue = JoystickXValue;
  lastYValue = JoystickYValue;
  lastTemp = temp;
  delay(75);
}
