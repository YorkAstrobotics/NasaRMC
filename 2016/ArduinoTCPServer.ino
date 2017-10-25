#include <SPI.h>
#include <Ethernet.h>
#include <stdint.h>
#include <SoftwareSerial.h>

#define G_LED A0
#define R_LED A1
#define Y_LED A2
#define B_LED A3
#define W_LED A4

int t=0;


// Enter a MAC address and IP address for your controller below.
// The IP address will be dependent on your local network.
// gateway and subnet are optional:

byte mac[] = {0x90, 0xA2, 0xDA, 0x0F, 0xEA, 0x76};
IPAddress ip    (192,168,1,42);
IPAddress gateway (192,168,1,1);
IPAddress subnet  (255,255,255,0);
EthernetServer server(5050);

SoftwareSerial Actuators(2,3);
SoftwareSerial Track(4,5);  //******THIS IS FULL 24V, DO NOT CONNECT A 12 V MOTOR ON THIS CHANNEL*****  ******PINS 4 AND 5*****




byte Motor_R ;
byte Motor_L;

byte Short_Actuators;
byte Long_Actuator;

byte Track_Actuator;


void setup() {
  allStop();
  pinMode(G_LED,OUTPUT);
  pinMode(R_LED, OUTPUT);
  pinMode(B_LED, OUTPUT);
  pinMode(Y_LED, OUTPUT);
  pinMode(W_LED, OUTPUT);


  digitalWrite(G_LED,HIGH);

  // Open serial communications and wait for port to open:
  Serial.begin(115200);
  Actuators.begin(115200);
  Track.begin(115200);


  // start the Ethernet connection
  // initialize the Ethernet device not using DHCP:
  Ethernet.begin(mac,ip);
  //  Ethernet.begin(mac);
  delay(1000);delay(1000);delay(1000);
  flashLEDs();
  digitalWrite(R_LED,LOW);
  // print your local IP address:
  Serial.print("My IP address: ");
  ip = Ethernet.localIP();

  for (byte thisByte = 0; thisByte < 4; thisByte++)
  {
    // print the value of each byte of the IP address:
    Serial.print(ip[thisByte], DEC);
    Serial.print(".");
  }
  Serial.println();
  // start listening for clients
  server.begin();

}

void loop() {
  // wait for a new client:
  EthernetClient client = server.available();

  //Debug code to let us know if we acutually connected
  if (client) {

    digitalWrite(R_LED,LOW);



    // read the bytes incoming from the client:
    byte Motor_R = client.read();
    byte Motor_L = client.read();

    byte Short_Actuators=client.read();
    byte Long_Actuator = client.read();
    byte Track_Actuator = client.read();


    //    Serial.print("[");  Serial.print(Motor_R, DEC);   Serial.print("]");
    //  Serial.print("[");  Serial.print(Motor_L, DEC);   Serial.print("]");
    //  Serial.print("[");  Serial.print(Short_Actuators, DEC);    Serial.print("]");
    //Serial.print("[");  Serial.print(Long_Actuator, DEC;   Serial.print("]");
    //  Serial.print("[");  Serial.print(Track_Actuator, DEC);    Serial.print("]");



    processMotors(Motor_R, Motor_L);
    processActuators(Short_Actuators, Long_Actuator, Track_Actuator);
    t=0;
  }
  else 
  {

    digitalWrite(B_LED,LOW);
    digitalWrite(Y_LED,LOW);
    digitalWrite(W_LED,LOW);
    Timeout();
  }

}

void processMotors(byte R, byte L)
{
  int driveSpeedRight; int driveSpeedLeft;  

  driveSpeedRight=(R);
  driveSpeedLeft = (L);


  //Serial.println(R);Serial.println(L);


  // Shift down to center on 0
  driveSpeedRight = driveSpeedRight - 127;
  driveSpeedLeft = driveSpeedLeft - 127;


  // Range is now -127 to 127, we need to pull this out


  // to the full range of -1000 to 1000
  driveSpeedRight = driveSpeedRight * -8;
  driveSpeedLeft = driveSpeedLeft * 8;
  if (driveSpeedRight | driveSpeedLeft != 0)
    digitalWrite(Y_LED, HIGH);


  if(driveSpeedRight > 1000)
    driveSpeedRight = 1000;
  if(driveSpeedRight < -1000)
    driveSpeedRight = -1000;


  if(driveSpeedLeft > 1000)
    driveSpeedLeft = 1000;
  if(driveSpeedLeft < -1000)
    driveSpeedLeft = -1000;



  //Sends the command to the motor cotroller on the serial port

  Serial.print("!G");               // Motor GO command
  Serial.print(" ");                //   Space delimiter
  Serial.print("1");                // Channel Number
  Serial.print(" ");                //   Space delimiter
  Serial.println(driveSpeedRight);  // Motor Power Value

  Serial.print("!G");               // Motor GO command
  Serial.print(" ");                //   Space delimiter
  Serial.print("2");                // Channel Number
  Serial.print(" ");                //   Space delimiter
  Serial.println(driveSpeedLeft);   // Motor Power Value
}

void processActuators(byte ShortA, byte LongA, byte TrackA)
{
  int ShortSpeed; int LongSpeed; int TrackSpeed;  


  ShortSpeed=(ShortA);
  LongSpeed = (LongA);
  TrackSpeed=(TrackA);

  //Serial.println(ShortA);Serial.println(LongA);Serial.println(TrackSpeed);


  // Shift down to center on 0

  ShortSpeed = ShortSpeed - 127;
  LongSpeed = LongSpeed - 127;
  TrackSpeed = TrackSpeed -127;



  // Range is now -127 to 127, we need to pull this out
  // to the full range of -1000 to 1000
  ShortSpeed = ShortSpeed * -4;
  LongSpeed = LongSpeed * 4;
  TrackSpeed = TrackSpeed * 8 ;                   //******THIS IS FULL 24V, DO NOT CONNECT A 12 V MOTOR ON THIS CHANNEL*****  ******PINS 4 AND 5*****

  if  (TrackSpeed > 10)
    TrackSpeed=1000;
  if (TrackSpeed < -10)
    TrackSpeed = -1000;

  if(TrackSpeed != 0)
    digitalWrite(W_LED, HIGH);
  else digitalWrite(W_LED,LOW);
  if(ShortSpeed | LongSpeed !=0)
    digitalWrite(B_LED,HIGH);
  else digitalWrite (B_LED,LOW);

  if(ShortSpeed > 500)
    ShortSpeed = 500;
  if(ShortSpeed < -500)
    ShortSpeed = -500;


  if(LongSpeed > 500)
    LongSpeed = 500;
  if(LongSpeed < -500)
    LongSpeed = -500;



  //Sends the command to the motor cotroller on the serial port

  Actuators.print("!G");               // Motor GO command
  Actuators.print(" ");                //   Space delimiter
  Actuators.print("1");                // Channel Number
  Actuators.print(" ");                //   Space delimiter
  Actuators.println(ShortSpeed);  // Motor Power Value

  Actuators.print("!G");               // Motor GO command
  Actuators.print(" ");                //   Space delimiter
  Actuators.print("2");                // Channel Number
  Actuators.print(" ");                //   Space delimiter
  Actuators.println(LongSpeed);   // Motor Power Value

  Track.print("!G");               // Motor GO command
  Track.print(" ");                //   Space delimiter
  Track.print("2");                // Channel Number
  Track.print(" ");                //   Space delimiter
  Track.println(TrackSpeed);   // Motor Power Value

  /*
     Serial.print("!G");               // Motor GO command
     Serial.print(" ");                //   Space delimiter
     Serial.print("1");                // Channel Number
     Serial.print(" ");                //   Space delimiter
     Serial.println(ShortSpeed);  // Motor Power Value

     Serial.print("!G");               // Motor GO command
     Serial.print(" ");                //   Space delimiter
     Serial.print("1");                // Channel Number
     Serial.print(" ");                //   Space delimiter
     Serial.println(LongSpeed);  // Motor Power Value

     Serial.print("!G");               // Motor GO command
     Serial.print(" ");                //   Space delimiter
     Serial.print("2");                // Channel Number
     Serial.print(" ");                //   Space delimiter
     Serial.println(TrackSpeed);  // Motor Power Value

     Serial.print ('\n');Serial.print ('\n');Serial.print ('\n');Serial.print ('\n');
   */
}
void flashLEDs()
{  
  flashLED(R_LED, 125, 25);
  flashLED(W_LED, 125, 25);
  flashLED(Y_LED, 125, 25);
  flashLED(B_LED, 125, 25);

}
void flashLED(int LED, int durationOn, int durationOff)
{
  digitalWrite(LED, HIGH);
  delay(durationOn);
  digitalWrite(LED, LOW);
  delay(durationOff);
}
void Timeout()
{
  t=t+1;
  if (t > 1000)
  {
    allStop();
    digitalWrite(R_LED, HIGH);
    t=0;
  }
}
void allStop()
{
  Serial.print("!G");  // Motor GO command
  Serial.print(" ");   //   Space
  Serial.print(1);     // Channel Number
  Serial.print(" ");   //   Space
  Serial.println(0);   // Motor Power Value

  Serial.print("!G");  // Motor GO command
  Serial.print(" ");   //   Space
  Serial.print(2);     // Channel Number
  Serial.print(" ");   //   Space
  Serial.println(0);   // Motor Power Value  

  Track.print("!G");  // Motor GO command
  Track.print(" ");   //   Space
  Track.print(1);     // Channel Number
  Track.print(" ");   //   Space
  Track.println(0);   // Motor Power Value

  Track.print("!G");  // Motor GO command
  Track.print(" ");   //   Space
  Track.print(2);     // Channel Number
  Track.print(" ");   //   Space
  Track.println(0);   // Motor Power Value  

  Actuators.print("!G");  // Motor GO command
  Actuators.print(" ");   //   Space
  Actuators.print(1);     // Channel Number
  Actuators.print(" ");   //   Space
  Actuators.println(0);   // Motor Power Value

  Actuators.print("!G");  // Motor GO command
  Actuators.print(" ");   //   Space
  Actuators.print(2);     // Channel Number
  Actuators.print(" ");   //   Space
  Actuators.println(0);   // Motor Power Value  
}






