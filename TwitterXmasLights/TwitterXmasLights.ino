#include <SPI.h>         
#include <Ethernet.h>
#include <PubSubClient.h>
#include <EEPROM.h> 

#define P_NONE    0
#define P_ONOFF    1
#define P_ALTERNATE  2
#define P_FAST      3
#define P_FADE      4
#define P_SKIP  5
#define P_DANCE  6
#define P_PAT  7
#define P_LIGHTNING  8

#define PIN_1  5 
#define PIN_2  1 


int brightness = 0;    // how bright the LED is
int fadeAmount = 70;    // how many points to fade the LED by
int lightPattern = P_NONE;

byte server[] = {130,102,129,175}; //winter.ceit.uq.edu.au
char connectName[25];
char IPstring[80];
static char mqttClientID[80];
int greenLEDpin = 13;

EthernetClient ethClient;
PubSubClient client(server, 1883, callback, ethClient);

void processmessage(byte* payload);
void playPattern(void);


void callback(char* topic, byte* payload, unsigned int length){
  payload[length] = '\0';
  Serial.println( (char*) payload);
  processmessage(payload);
}


void setup_MQTT() {
  // EEPROM locations:
  // Unique MAC address can be stored in EEPROM using 'setMACaddress' sketch, which sets the last 3 bytes of this address.
  const int eepromMacFlag = 0; // 1 byte
  int eepromMacAddress = 1; // 3 bytes
  // Default MAC address for the ethernet controller.
  static byte mac[] = {0x90, 0xA2, 0xDA, 0x00, 0xFF, 0xFF}; 

  // Retrieve MAC address from EEPROM if present
  if (EEPROM.read(eepromMacFlag) == '#'){
    Serial.println("Retrieving MAC address from EEPROM");
    for (int i = 3; i < 6; i++)
      mac[i] = EEPROM.read(eepromMacAddress++);
  } 
  else {
    Serial.println("No MAC address stored in EEPROM");
    Serial.println("Using default MAC address");
  }  

  // Publish the MAC address to serial port
  {
    const int outputStringLen = 25;
    static char outputString[outputStringLen];  // buffer used for constructing output strings

    snprintf(outputString, outputStringLen, "%02x:%02x:%02x:%02x:%02x:%02x", mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
    Serial.print("MAC Address: "); 
    Serial.println(outputString);
  }

  // Use DHCP to determine my IP address
  Serial.println("DHCP...");
  while (Ethernet.begin(mac) == 0) {
    digitalWrite(greenLEDpin, HIGH);         
    Serial.println("No DHCP");
    delay(1000);
    digitalWrite(greenLEDpin, LOW);         
    Serial.println("DHCP...");
  }  
  Serial.println("IP address: ");
  sprintf(IPstring, "%u.%u.%u.%u", Ethernet.localIP()[0], Ethernet.localIP()[1], Ethernet.localIP()[2], Ethernet.localIP()[3]);
  Serial.println(IPstring);

  // Craft the MQTT client ID, which must be unique.  Use the IP address of this machine to make it so.
  sprintf(mqttClientID, "%u", Ethernet.localIP()[3]);
  strcat(mqttClientID, "-arduino");
}





void setup() {
  pinMode(6,OUTPUT);
  pinMode(5,OUTPUT);
  //Ethernet.begin(mac,ip);
  Serial.begin(9600);
  setup_MQTT();
  if (client.connect(mqttClientID)) {
    client.subscribe("ChristmasLights");
  } 
  lightPattern = P_NONE;
}


void loop() {
  if (!client.connected()) {
    client.connect(mqttClientID);
    client.subscribe("ChristmasLights");
    return;
  }
  client.loop();
  playPattern();
}


void processmessage( byte* payload) { 
  if( strcmp((char*)payload, "alternating")==0) {
    lightPattern = P_ALTERNATE;
  } 
  else if (strcmp((char*)payload, "onoff")==0) {
    lightPattern = P_ONOFF;
  } 
  else if (strcmp((char*)payload, "fade")==0) {
    lightPattern = P_FADE; 
  }
  
  else if (strcmp((char*)payload, "fast")==0) {
    lightPattern = P_FAST; 
  }
  
  else if (strcmp((char*)payload, "skip")==0) {
    lightPattern = P_SKIP; 
  }
  
  else if (strcmp((char*)payload, "dance")==0) {
    lightPattern = P_DANCE; 
  }
  
  else if (strcmp((char*)payload, "pat")==0) {
    lightPattern = P_PAT; 
  }
  
  else if (strcmp((char*)payload, "lightning")==0) {
    lightPattern = P_LIGHTNING;
  }
}


void playPattern(void) {
  switch (lightPattern) {
  case P_NONE:
    break;

  case P_ALTERNATE:
    alternating();
    break;

  case P_ONOFF:
    onoff();
    break; 
  
  case P_FADE:
    fade();
    break;
  
  case P_FAST:
    fast();
    break;
  
  case P_SKIP:
    skip();
    break;
    
  case P_DANCE:
    dance();
    break;
    
  case P_PAT:
    pat();
    break;
  
  case P_LIGHTNING:
    lightning();
    break;
  }
}


//_______ PATTTERNS _________

void onoff(){ 
  digitalWrite(PIN_2,HIGH);
  digitalWrite(PIN_1,HIGH);
  delay(500);
  digitalWrite(PIN_2,LOW);
  digitalWrite(PIN_1,LOW);
  delay(500);
}


void alternating(){
  digitalWrite(PIN_2,HIGH);
  digitalWrite(PIN_1,LOW);
  delay(1000);
  digitalWrite(PIN_2,LOW);
  digitalWrite(PIN_1,HIGH);
  delay(1000);
}


void fade(){
  analogWrite(6, brightness); analogWrite(PIN_1, brightness);  brightness = brightness + fadeAmount;if (brightness == 0 || brightness == 255) {fadeAmount = -fadeAmount ; } delay(500);digitalWrite(PIN_1,HIGH); delay(50);digitalWrite(PIN_1,LOW); delay(50);digitalWrite(PIN_1,HIGH); delay(50);digitalWrite(PIN_1,LOW); delay(500);
}


void fast(){
  digitalWrite(PIN_1,HIGH); delay(100);digitalWrite(PIN_1,LOW); delay(100);
}


void skip(){
 analogWrite(PIN_2, brightness); digitalWrite(PIN_1, HIGH);  brightness = brightness + fadeAmount;if (brightness == 0 || brightness == 255) {fadeAmount = -fadeAmount ; } delay(200); digitalWrite(PIN_1,LOW); delay(200);
}


void dance(){
  digitalWrite(PIN_2,HIGH);
  digitalWrite(PIN_1,LOW);
  delay(200);
  digitalWrite(PIN_2,LOW);
  digitalWrite(PIN_1,HIGH);
  delay(500);
  digitalWrite(PIN_2,HIGH);
  digitalWrite(PIN_1,HIGH);
  delay(250);
  digitalWrite(PIN_2,LOW);
  digitalWrite(PIN_1,LOW);
  delay(250);
}

void pat(){
  digitalWrite(PIN_2,HIGH);
  digitalWrite(PIN_1,LOW);
  delay(200);
  digitalWrite(PIN_2,LOW);
  delay(300);
  digitalWrite(PIN_2,HIGH);
  delay(200);
  digitalWrite(PIN_2,LOW);
  delay(250);
  digitalWrite(PIN_1,HIGH);
  delay(300);  
  digitalWrite(PIN_1,LOW);
  digitalWrite(PIN_2,LOW);
  delay(450);  
}


void lightning(){
  digitalWrite(PIN_2,HIGH);
  digitalWrite(PIN_1,LOW);
  delay(200);
  digitalWrite(PIN_2,LOW);
  delay(300);
  digitalWrite(PIN_2,HIGH);
  delay(200);
  digitalWrite(PIN_2,LOW);
  delay(250);
  digitalWrite(PIN_1,HIGH);
  delay(300);  
  digitalWrite(PIN_1,LOW);
  digitalWrite(PIN_2,LOW);
  delay(900); 
  digitalWrite(PIN_1,HIGH);
  digitalWrite(PIN_2,HIGH);
  delay(50);  
  digitalWrite(PIN_1,LOW);
  digitalWrite(PIN_2,LOW);
  delay(50);    
  digitalWrite(PIN_1,HIGH);
  digitalWrite(PIN_2,HIGH);
  delay(50);  
  digitalWrite(PIN_1,LOW);
  digitalWrite(PIN_2,LOW);
  delay(50);  
  digitalWrite(PIN_1,HIGH);
  digitalWrite(PIN_2,HIGH);
  delay(50);  
  digitalWrite(PIN_1,LOW);
  digitalWrite(PIN_2,LOW);
  delay(50);   
}
