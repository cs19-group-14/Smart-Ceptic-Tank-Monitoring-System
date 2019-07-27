#include<SoftwareSerial.h>

SoftwareSerial GPRS(13, 12);

int led1 = 2, led2 = 3, led3 = 4;

int echoPin = 9;                           // Echo Pin of the ultrasonic sensor

int trigPin = 8;                          // Trigger Pin of the ultrasonic sensor

long distance;                              //Distance of the object from sensorlong echoTime Time taken for the echo

int triggerContentDistance = 10;              //GSM will be triggered to send a message if the distance is less than this value. (cm)

int warningDistance = 20;

int echoTime;


void setup()

{

  pinMode(led1, OUTPUT);

  pinMode(led2, OUTPUT);

  pinMode(led3, OUTPUT);
 
  pinMode(echoPin, INPUT);

  pinMode(trigPin, OUTPUT);

  GPRS.begin(9600);                         // the GPRS baud rate

  Serial.begin(9600);                  // the Serial port of Arduino baud rate.

  delay(500);

}

void loop()

{

   int SMSSent;
 
 if(SMSSent == 0)
 {
  sendSMS();
  SMSSent = 1;
 }

  digitalWrite(trigPin, LOW);                   //Set the trigger pin of the SR04 sensor to low

  delayMicroseconds(2);

  digitalWrite(trigPin, HIGH);                //Set the trigger pin of the SR04 sensor to high which will which will send a pulse.

  delayMicroseconds(10);

  digitalWrite(trigPin, LOW);                 //Set the trigger pin of the SR04 sensor to low

  echoTime = pulseIn(echoPin, HIGH);        //Reads a pulse (either HIGH or LOW) on a pin. For example, if value is HIGH, pulseIn() waits for the pin to go HIGH, starts timing, then waits for the pin to go LOW and stops timing. Returns the length of the pulse in microseconds or 0 if no complete pulse was received within the timeout https://www.arduino.cc/en/Reference/PulseIn

  distance = echoTime * 0.0340 / 2;

  if(distance < triggerContentDistance)

  {

    digitalWrite(led2, LOW);
    digitalWrite(led3, LOW);

   // digitalWrite(led1, HIGH);
   // delay(250);
    digitalWrite(led1, HIGH);
    delay(250);
    digitalWrite(led1, LOW);
    delay(500);
    digitalWrite(led1, HIGH);
    delay(250);
    digitalWrite(led1, LOW);
    delay(500);

    sendSMS();

    Serial.println("Sent SMS!");

    Serial.println( "Septic Tank has reached maximum level");
    Serial.print(distance);
    Serial.println("cm");
    Serial.println();

    delay(10);

     }

    else if(distance > triggerContentDistance && distance < warningDistance)
      {

        digitalWrite(led1, LOW);
        digitalWrite(led2, LOW);
        digitalWrite(led3, HIGH);
        delay(200);
        digitalWrite(led3, LOW);
        delay(200);
        digitalWrite(led3, HIGH);
        sendSMS2();  

        Serial.println("Sent Warning SMS!");

        Serial.println( "Septic Tank almost reaching maximum level");
        Serial.print(distance);
        Serial.println("cm");
        Serial.println();
    
        delay(10);
      }


   else
    { 
      digitalWrite(led1, LOW);
      digitalWrite(led3, LOW);
      Serial.println( "Septic Tank level is Okay");
      
      digitalWrite(led2, HIGH);
      
      Serial.print(distance);
      
      Serial.println("cm");
      Serial.println();
    }

  delay(1000);                    //Delay 40ms before next reading.

}


void sendSMS()

{
  
  GPRS.println("AT+CMGF=1");      // Set modem to text mode

  delay(1200);

  GPRS.write("AT+CMGS=");           // Start composing message

  GPRS.write(0x22);             // hex equivalent of double-quote '"'

  GPRS.write("+256788816187");           // the number to send the SMS to

  GPRS.write(0x22);
  GPRS.write(0x0D);              // hex equivalent of Carraige return

  GPRS.write(0x0A);                 // hex equivalent of newline

  delay(1200);

  GPRS.print("ID:=1 LOCATION:=MAKERERE UNIVERSITY, COLLEGEE OF ICT,  THE ABOVE SEPTIC TANK HAS REACHED ITS MAXIMUM CAPACITY PLEASE COME AND DRAIN IT TO KEEP THE ENVIRONMENT CLEAN"); // Send the text message to the GSM module

  GPRS.write(0x0D);            // hex equivalent of Carraige return

  GPRS.write(0x0A);        // hex equivalent of newline

  delay(700);

  GPRS.write(26);            // equivalent of CTRL-Z delay(3000);

}

//SEND WARNING SMS
void sendSMS2()

{
  
  GPRS.println("AT+CMGF=1");      // Set modem to text mode

  delay(1200);

  GPRS.write("AT+CMGS=");           // Start composing message

  GPRS.write(0x22);             // hex equivalent of double-quote '"'

  GPRS.write("+256788816187");           // the number to send the SMS to

  GPRS.write(0x22);
  GPRS.write(0x0D);              // hex equivalent of Carraige return

  GPRS.write(0x0A);                 // hex equivalent of newline

  delay(1200);

  GPRS.print("ID:=1 LOCATION:=MAKERERE UNIVERSITY, COLLEGEE OF ICT,  THE ABOVE SEPTIC TANK IS ALMOST REACHING ITS MAXIMUM CAPACITY PLEASE."); // Send the text message to the GSM module

  GPRS.write(0x0D);            // hex equivalent of Carraige return

  GPRS.write(0x0A);        // hex equivalent of newline

  delay(700);

  GPRS.write(26);            // equivalent of CTRL-Z delay(3000);

}
