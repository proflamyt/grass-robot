#define sensorPower 5
#define sensorPin A0
#define soilPin A1
#define RELAY 11
#include <SoftwareSerial.h>
const int AirValue = 620;   //you need to replace this value with Value_1
const int WaterValue = 310;  //you need to replace this value with Value_2
int soilMoistureValue = 0;
int soilmoisturepercent=0;
char state=0;
SoftwareSerial s(2,3);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
   s.begin(9600);
    // Initialize the output variables as outputs
  pinMode(RELAY, OUTPUT);

  // Set outputs to LOW
  pinMode(sensorPower, OUTPUT);
  
  // Set to LOW so no power flows through the sensor
  digitalWrite(sensorPower, LOW);
  
  digitalWrite(RELAY, LOW);
  digitalWrite(soilPin, LOW);

}


int moisture(){
  
  
soilMoistureValue = analogRead(soilPin );  //put Sensor insert into soil

soilmoisturepercent = map(soilMoistureValue, AirValue, WaterValue, 0, 100);
if(soilmoisturepercent >= 100)
{
    return 100;
}
else if(soilmoisturepercent <=0)
{
    return 0;
}
else if(soilmoisturepercent >0 && soilmoisturepercent < 100)
{
  return soilmoisturepercent;

  
}
  delay(250);
  }






int readSensor() {
  digitalWrite(sensorPower, HIGH);  // Turn the sensor ON
  delay(10);              // wait 10 milliseconds
  int  val = analogRead(sensorPin);    // Read the analog value form sensor
  digitalWrite(sensorPower, LOW);   // Turn the sensor OFF
  return val;             // send current reading
}


void loop() {
  
while(s.available()){
state = s.read();

Serial.print(state);
}
  
  if(state == 1){
   

    
    digitalWrite(RELAY, HIGH);

  }
  else if (state == 2 ){
   
    digitalWrite(RELAY, LOW);
   
    }



int sensor = readSensor();

s.write(sensor);
 int soil_moisture = moisture();

 s.write(soil_moisture);


} 
