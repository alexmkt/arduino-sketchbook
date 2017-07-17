int sensorValue;
int x;
long sensorTotal;

void setup()
{
  Serial.begin(9600);  // sets the serial port to 9600
  pinMode(9, OUTPUT); 
}

void loop()
{
  
  sensorTotal=0;
  for (int x = 0; x < 500; x++) {
  Serial.println("here");
  sensorValue = analogRead(5);       // read analog input pin 0
  Serial.println(sensorValue, DEC);  // prints the value read
  delay(100);                        // wait 100ms for next reading
  
  sensorValue = sensorValue-512;
  if (sensorValue<0){
      sensorValue=sensorValue*-1;
  }
  //sensorValue = abs(sensorValue);
   //Serial.println(sensorValue, DEC);
   
 
 
    sensorTotal = sensorTotal+sensorValue;
   
   
 }
  
  
  
  
  sensorTotal = sensorTotal/2000;
    Serial.println(sensorTotal, DEC);
  analogWrite(9, sensorTotal );
  
  

}





