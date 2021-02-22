
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

const int pinCE = PA9;
const int pinCSN = PA10;

int Array[4];
int counter = 0;

RF24 radio(pinCE, pinCSN);

const byte slaveAddress[5] = {'R', 'x', 'A', 'A', 'A'};
const byte masterAddress[5] = {'T','x','A','A','A'};

long int msg_1[10];
char sendID[1];
volatile bool rslt;

void setup() 
{
 Serial.begin(9600);       
 radio.begin();         
 radio.setAutoAck(1);               
 radio.enableAckPayload();         
 radio.setRetries(5,15);           
    radio.openWritingPipe(masterAddress);        // NB these are swapped compared to the master
    radio.openReadingPipe(1, slaveAddress); 
    radio.setChannel(108);
 radio.stopListening();
 radio.printDetails(); 
}

void loop() 
{
  radio.startListening();                 //This sets the module as transmitter
  delay(50);                              //Mandatory delay to switch state
  if (radio.available())
  {
    radio.read(&sendID, 1);
    if (sendID[0] == 'A')                  // Client ID [A/B/C/D]
    {
      Serial.print("Received Request ");
      Serial.println(sendID[0]);           // Display Client ID
      send();
    }
  }
  Serial.println("Waiting for request");
}

void send()
{
   for (int i=0; i<4; i++){
     Array[i] = map (analogRead(PA0), 0, 255, 0, 179); 
     counter++;
   }
    
   if (!radio.write( &Array, sizeof(Array))){
      Serial.println("delivery failed");
      Serial.println();     
  }
  else {
      Serial.println(Array[0]);
      Serial.println();
  }
}
