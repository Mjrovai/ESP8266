/**************************************************************************************************
* ES8266 Arduino Uno GPIO Control
* LED ON/OFF
* Sketch adaptada do original https://youtu.be/xa6dJvd7wHg?list=PLbADmeIeQARotrEv9bTuOVyJLOYOD-pPM
*
* Marcelo Jose Rovai - 19Jan2016
***************************************************************************************************/

//#define esp8266 Serial2 <===== In case of using MEGA, take out the comment
#include <SoftwareSerial.h>   // In case of using MEGA, mark the line as comment
SoftwareSerial esp8266(2,3);  // In case of using MEGA, mark the line as comment
//Rx ==> Pin 2; TX ==> Pin3 

#define speed8266 9600  // <========= Change the Speed for the one used at Module
#define CH_PD 4 
#define DEBUG true

int red =12;
int green =13;
int blue =11;


void setup()

{
  pinMode(red,OUTPUT);
  pinMode(green,OUTPUT);
  pinMode(blue,OUTPUT);
  
  digitalWrite(red,LOW);
  digitalWrite(green,LOW);
  digitalWrite(blue,HIGH);

  Serial.begin(9600);
  esp8266.begin(speed8266);

  reset8266(); 
  InitWifiModule();

  digitalWrite(blue,LOW);
  digitalWrite(green,HIGH);
  delay(2000);
  digitalWrite(green,LOW);
}

void loop()
{

  if(esp8266.available())
  {
    if(esp8266.find("+IPD,"))
    {
      delay(1000);  
      int connectionId = esp8266.read()-48;  
      esp8266.find("pin="); 
      int pinNumber = (esp8266.read()-48)*10; 
      pinNumber += (esp8266.read()-48); 
      int statusLed =(esp8266.read()-48);

      digitalWrite(pinNumber, statusLed);

      String closeCommand = "AT+CIPCLOSE="; 
      closeCommand+=connectionId; 
      closeCommand+="\r\n";
      sendData(closeCommand,1000,DEBUG); 
      
    }
  }
}

/*****************************************************************************************
* Name: sendData
* Description: Function used to send data to ESP8266.
* Params: command - the data/command to send; timeout - the time to wait for a response; debug - print to Serial window?(true = yes, false = no)
* Returns: The response from the esp8266 (if there is a reponse)
*/
String sendData(String command, const int timeout, boolean debug)
{
    String response = "";
    
    esp8266.print(command); // send the read character to the esp8266
    long int time = millis();
    while( (time+timeout) > millis())
    {
      while(esp8266.available())
      {
        char c = esp8266.read(); // read the next character.
        response+=c;
      }  
    }
    
    if(debug)
    {
      Serial.print(response);
    }    
    return response;
}




/***********************************************************************/  
// This initializes the Wifi Module as a server 
void InitWifiModule()
{
  sendData("AT+RST\r\n", 2000, DEBUG); // reset
  sendData("AT+CWMODE=1\r\n", 1000, DEBUG);
  //sendData("AT+CWJAP=\"Your_WiFi_Network_Name\",\"password\"\r\n", 2000, DEBUG); //Connect network
  delay (1000);
  sendData("AT+CIFSR\r\n", 1000, DEBUG); // Show IP Adress
  sendData("AT+CIPMUX=1\r\n", 1000, DEBUG); // Multiple conexions
  sendData("AT+CIPSERVER=1,80\r\n", 1000, DEBUG); // start comm port 80
}

/*************************************************/
// Reset funtion to accept communication
void reset8266 ()
{
  pinMode(CH_PD, OUTPUT);
  digitalWrite(CH_PD, LOW);
  delay(300);
  digitalWrite(CH_PD, HIGH);
}
