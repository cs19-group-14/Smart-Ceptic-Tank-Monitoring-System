#include<SoftwareSerial.h>

SoftwareSerial GPRS(13, 12);

int led1 = 2, led2 = 3, led3 = 4, led4 = 5;

int echoPin = 8;                           // Echo Pin of the ultrasonic sensor

int trigPin = 9;                          // Trigger Pin of the ultrasonic sensor

long distance;                              //Distance of the object from sensorlong echoTime Time taken for the echo

int triggerContentDistance = 10;              //GSM will be triggered to send a message if the distance is less than this value. (cm)

int warningDistance = 20;

int echoTime;

void setup()

{
  
  pinMode(led1, OUTPUT);

  pinMode(led2, OUTPUT);

  pinMode(led3, OUTPUT);

  pinMode(led4, OUTPUT);
 
  pinMode(echoPin, INPUT);

  pinMode(trigPin, OUTPUT);

  GPRS.begin(9600);                         // the GPRS baud rate

  Serial.begin(9600);                  // the Serial port of Arduino baud rate.

  delay(500);

}

void loop()

{
//ACTIVATING SENSOR AND SETTING DISTANCE START

//ACTIVATING SENSOR AND SETTING DISTANCE END

  Sensor();

  if(distance < triggerContentDistance)

  {

    digitalWrite(led2, LOW);
    digitalWrite(led3, LOW);

    for(int i=0;i<=3;i++)
    {
      digitalWrite(led1, HIGH);
      delay(250);
      digitalWrite(led1, LOW);
      delay(250);
      digitalWrite(led4, HIGH);
      delay(250);
      digitalWrite(led4, LOW);
      delay(250);
    }

     Serial.println( "An obstacle encountered!");
     Serial.print(distance);
     Serial.println("cm");
     Serial.println();
    
    for(int c=0;c<=10;c++)
    {
      digitalWrite(led1, HIGH);
      delay(500);
      digitalWrite(led1,LOW);
      delay(500);
     }

     digitalWrite(led1, HIGH);
     digitalWrite(led4, LOW);
     

    //CHECKING WHETHER ITS AN OBSTRUCTION
    Sensor();
      if(distance < triggerContentDistance)
      {
        Serial.println("Sending Warning SMS...");
        sendWarningSMS();
        digitalWrite(led4, HIGH);
        delay(1000);
        digitalWrite(led4, LOW);
        
        toServer();
        digitalWrite(led4, HIGH);
        delay(1000);
        digitalWrite(led4, LOW);
        
          Sensor(); 
         if(distance < triggerContentDistance)//IF RETURNS TRUE, SYSTEM STOPS, AND A MANUAL RESET IS REQUIRED
         { 
          //All the LEDs turn on if the distance value is still below triggerContentDistance
          digitalWrite(led1, HIGH);
          digitalWrite(led4, HIGH);
          digitalWrite(led2, HIGH);
          digitalWrite(led3, HIGH);
  
          //Message sent to responsible person and contents updated to the ThingSpeak Server
          sendSMS();
          digitalWrite(led4, LOW);
          delay(1000);
          digitalWrite(led4, HIGH);
          
          toServer();
          digitalWrite(led4, LOW);
          delay(1000);
          digitalWrite(led4, HIGH);
          
          digitalWrite(led4, LOW);
     
          Serial.println( "Septic Tank has reached maximum level");
          Serial.print(distance);
          Serial.println("cm");
          Serial.println();
          Serial.println("Sent SMS!");
          Serial.println("Septic is full, System Shutting down!");
          
          while(1); //The loop is haulted/stopped.
        }
        
      }
      else
      {
        Serial.println("An interference cleared, good to go."); //prints to serial monitor
      }

     }
   //END OF CHECKING  

    else if(distance >= triggerContentDistance && distance <= warningDistance)
      {
        

        digitalWrite(led1, LOW);
        digitalWrite(led2, LOW);
        digitalWrite(led3, HIGH);
        delay(200);
        digitalWrite(led3, LOW);
        delay(200);
        digitalWrite(led3, HIGH);
                
        Serial.println( "Septic Tank almost reaching maximum level");
        Serial.print(distance);
        Serial.println("cm");
        Serial.println();
        toServer();
    
        delay(2000);
      }


   else
    { 
      digitalWrite(led1, LOW);
      digitalWrite(led3, LOW);
      Serial.println( "Septic Tank level is Okay");
      
      digitalWrite(led2, HIGH);
      delay(250);
      digitalWrite(led2, LOW);
      delay(250);
      digitalWrite(led2, HIGH);
      digitalWrite(led2, HIGH);
      delay(250);
      digitalWrite(led2, LOW);
      delay(250);
      digitalWrite(led2, HIGH);
      
      Serial.print(distance);
      
      Serial.println("cm");
      Serial.println();
      
      toServer();
    }

  delay(10);                    //Delay 10ms before next reading.

}

//SEND WARNING SMS
void sendWarningSMS()
{  
  GPRS.println("AT+CIPSHUT");
  updateSerial();
  delay(5000);
  
  GPRS.println("AT+CMGF=1");      // Set modem to text mode
  updateSerial();

  delay(1000);

  GPRS.write("AT+CMGS=");           // Start composing message
  updateSerial();

  GPRS.write(0x22);             // hex equivalent of double-quote '"'

  GPRS.write("+256776465777");           // the number to send the SMS to
  updateSerial();

  GPRS.write(0x22);
  GPRS.write(0x0D);              // hex equivalent of Carraige return

  GPRS.write(0x0A);                 // hex equivalent of newline

  delay(1000);

  GPRS.print("WARNING!!! \nTHE SEPTIC TANK IS ALMOST FULL.\n CUSTOMERID=0001 LOCATION:=MAKERERE UNIVERSITY, COLLEGEE OF ICT."); // Send the text message to the GSM module
  updateSerial();

  GPRS.write(0x0D);            // hex equivalent of Carraige return

  GPRS.write(0x0A);        // hex equivalent of newline

  delay(700);

  GPRS.write(26);            // equivalent of CTRL-Z delay(3000);

}

//SEND SMS TO NOTIFY RESPONSIBLE PARTIES TO EMPTY THE SEPTIC TANK
void sendSMS()

{
  GPRS.println("AT+CIPSHUT");
  updateSerial();

  delay(5000);

  GPRS.println("AT");
  updateSerial();
  
  GPRS.println("AT+CMGF=1");      // Set modem to text mode
  updateSerial();

  delay(1000);

  GPRS.write("AT+CMGS=");           // Start composing message
  updateSerial();

  GPRS.write(0x22);             // hex equivalent of double-quote '"'

  GPRS.write("+256776465777");           // the number to send the SMS to
  updateSerial();

  GPRS.write(0x22);
  GPRS.write(0x0D);              // hex equivalent of Carraige return

  GPRS.write(0x0A);                 // hex equivalent of newline

  delay(1000);

  GPRS.print("THE SEPTIC TANK BELOW HAS REACHED ITS MAXIMUM CAPACITY PLEASE COME AND DRAIN IT TO KEEP THE ENVIRONMENT CLEAN\n ID:=1 LOCATION:=MAKERERE UNIVERSITY, COLLEGEE OF ICT"); // Send the text message to the GSM module
  updateSerial();

  GPRS.write(0x0D);            // hex equivalent of Carraige return

  GPRS.write(0x0A);        // hex equivalent of newline

  delay(700);

  GPRS.write(26);            // equivalent of CTRL-Z delay(3000);

}

//SENSOR FUNCTION FOR SETTING UP THE TRIGGER AND ECHO PINS
void Sensor()
  {

    digitalWrite(trigPin, LOW);                   //Set the trigger pin of the SR04 sensor to low
  
    delayMicroseconds(2);
  
    digitalWrite(trigPin, HIGH);                //Set the trigger pin of the SR04 sensor to high which will which will send a pulse.
  
    delayMicroseconds(10);
  
    digitalWrite(trigPin, LOW);                 //Set the trigger pin of the SR04 sensor to low
  
    echoTime = pulseIn(echoPin, HIGH);        //Reads a pulse (either HIGH or LOW) on a pin. For example, if value is HIGH, pulseIn() waits for the pin to go HIGH, starts timing, then waits for the pin to go LOW and stops timing. Returns the length of the pulse in microseconds or 0 if no complete pulse was received within the timeout https://www.arduino.cc/en/Reference/PulseIn
  
    distance = echoTime * 0.0340 / 2;
  
  }

  //SENDING TO SERVER
  void toServer(){
  Serial.println("AT+CIPSHUT\r\n");
  GPRS.println("AT+CIPSHUT"); //Turn off GSM/
  updateSerial();
  delay(5000);
   //Print response on the serial monitor/
  delay(5000);
  
  Serial.println("HTTP post method: ");
  Serial.println("AT\r\n");
  GPRS.println("AT"); /*Check Communication*/
  updateSerial();
  delay(5000);
   /*Print response on the serial monitor*/
  delay(1000);

  Serial.println("CMD AT\r\n");
  GPRS.println("CMD AT"); /*Check Communication*/
  updateSerial();
  delay(5000);
   /*Print response on the serial monitor*/
  delay(1000);

  Serial.println("AT+CIPMUX=0\r\n");
  GPRS.println("AT+CIPMUX=0"); /*Check Communication*/
  updateSerial();
  delay(5000);
   /*Print response on the serial monitor*/
  delay(1000);

  Serial.println("AT+CGATT=1\r\n");
  GPRS.println("AT+CGATT=1"); /*Check Communication*/
  updateSerial();
  delay(5000);
   /*Print response on the serial monitor*/
  delay(1000);

//  Serial.print("\"AT+CSTT="INTERNET","",""\r\n");

  //tryStast
  Serial.print("AT+CSTT=");
  Serial.print("\"INTERNET\"");
  Serial.print(",");
  Serial.print("\"\"");
  Serial.print(",");
  Serial.print("\"\"");
  Serial.println("\r\n");
  //TryEnd
  /*Check Communication*/
  GPRS.print("AT+CSTT=");
  GPRS.print("\"INTERNET\"");
  GPRS.print(",");
  GPRS.print("\"\"");
  GPRS.print(",");
  GPRS.print("\"\"");
  GPRS.println("\r\n");
  updateSerial();
  delay(5000);
   /*Print response on the serial monitor*/
  delay(1000);

  Serial.println("AT+CIICR");
  GPRS.println("AT+CIICR"); /*Check Communication*/
  updateSerial();
  delay(5000);
   /*Print response on the serial monitor*/
  delay(1000);

  Serial.println("AT+CIFSR");
  GPRS.println("AT+CIFSR"); /*Check Communication*/
  updateSerial();
  delay(1000);
   /*Print response on the serial monitor*/
  delay(1000);

  Serial.println("AT+CIPSTART=\"TCP\",\"184.106.153.149\",80");
  GPRS.println("AT+CIPSTART=\"TCP\",\"184.106.153.149\",80");
  updateSerial();
  delay(5000);
  
  delay(1000);

  Serial.println("AT+CIPSEND=44");
  GPRS.println("AT+CIPSEND=44");
  updateSerial();
  delay(5000);
  
  delay(1000);

  Serial.println("GET https://api.thingspeak.com/update?api_key=U64QNZUHE8ARMSQ2&field1=distance\r\n"); 
  GPRS.println("GET https://api.thingspeak.com/update?api_key=U64QNZUHE8ARMSQ2&field1=distance\r\n");//Sends the output to the ThinkSpeak server
  delay(15000);
    }

    
 void updateSerial()
{
  while(GPRS.available()!=0) /*If data is available on Serial port*/
  Serial.write(char(GPRS.read())); /*Print character received on to the Serial monitor*/
}
