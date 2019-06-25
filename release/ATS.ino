#include <SPI.h>
#include <Ethernet.h>

// Enter a MAC address and IP address for your controller below.
// The IP address will be dependent on your local network:
byte mac[] = {
  0x00, 0x01, 0x02, 0x00, 0x00, 0x02 //Specify the client's MAC
};
IPAddress ip(10, 0, 0, 41); //Specify the client's IP

// Enter the IP address of the server you're connecting to:
IPAddress server(10, 0, 0, 21); //Specify the server's IP

// Initialize the Ethernet client library
// with the IP address and port of the server
// that you want to connect to (port 23 is default for telnet;
EthernetClient client;


#define sensorIn A0
#define ledRed  9
#define ledGreen  8
#define ledYellow  7
#define port 5555 //Specify the server's PORT
char incomingSerial; 
unsigned long time = millis();
float dataMin = 15;   //Min Threesold
float dataMax = 30;   //Max Threesold
int attStat = 0;    //0=idle 1=Cooling 2=Heating;
char mode = 'C';
char temp = 'C';
char incomingEth;
bool connection = false;
int dataMinTemp = 0;
int dataMaxTemp = 0;
long long tempTime = 0;
int DataMinTemp = 0;
int DataMaxTemp =0;


void readSerial(){
    if (Serial.available() > 0) {
             incomingSerial = Serial.read();

             if (incomingSerial == '/'){
                mode = toupper(Serial.read());
              }
    }
}

void readEth(){
      if (client.available() > 0) {
             incomingEth = client.read();

             if (incomingEth == '/'){
                mode = toupper(client.read());
              }
     }
}

//Spegni tutti i LED
void ledReset() {
    digitalWrite(ledRed, HIGH);
    digitalWrite(ledGreen, HIGH);
    digitalWrite(ledYellow, HIGH);
}

//Output dei dati via seriale
void serialOut(float dataTemperature, float dataMin, float dataMax, int attStat) {
  Serial.println((String) "/" + dataTemperature + " " + dataMin + " " + dataMax + " " + attStat + "#");
}

void ethOut(float dataTemperature, float dataMin, float dataMax, int attStat) {
  client.println((String) "/" + dataTemperature + " " + dataMin + " " + dataMax + " " + attStat + "#");
}

void setup() {
    Serial.begin(9600);
    // You can use Ethernet.init(pin) to configure the CS pin
    Ethernet.init(10);  // Most Arduino shields
    //Ethernet.init(5);   // MKR ETH shield
    //Ethernet.init(0);   // Teensy 2.0
    //Ethernet.init(20);  // Teensy++ 2.0
    //Ethernet.init(15);  // ESP8266 with Adafruit Featherwing Ethernet
    //Ethernet.init(33);  // ESP32 with Adafruit Featherwing Ethernet
    Ethernet.begin(mac, ip);

    // give the Ethernet shield a second to initialize:
    delay(1000);
      
    Serial.println("Connecting...");
    // if you get a connection, report back via serial:
    if (client.connect(server, port)) {
     connection=true;
     Serial.println("Connected");
    } else {
      // if you didn't get a connection to the server:
      Serial.println("Connection failed");
  }

    

    pinMode(7, OUTPUT);
    pinMode(8, OUTPUT);
    pinMode(9, OUTPUT);
    long long time = millis();
    

}

void loop() {

    //Reset stato LED
    
    ledReset();

    float dataTemperature = 5.0+(analogRead(sensorIn)/1024.0*35.0); //temp attuale;

    // if there are incoming bytes available
    // from the server, read them and print them:
    //Serial.print(incomingEth);
    

    //Verifica di dati in arrivo

    readSerial();
    readEth();

    //Switch operazionale per le funzioni implementate

    switch (mode)
    {
      case 'D': //Output manuale, ritorna automaticamente alla funzione precedente
      serialOut(dataTemperature,dataMin,dataMax,attStat);
      ethOut(dataTemperature,dataMin,dataMax,attStat);
      mode = temp;
      break;


      case 'L': //Attuatore manualmente in modalitÃ¡ di riscaldamento
      attStat=2;
      digitalWrite(ledYellow, LOW);
      temp = mode;
      break;


      case 'H': //Attuatore manualmente in modalitÃ¡ di raffreddamento
      attStat=1;
      digitalWrite(ledRed, LOW);
      temp = mode;
      break;


      case 'M': //Dispositivo in idle
      attStat=0;
      digitalWrite(ledGreen, LOW);
      temp = mode;
      break;


      case 'C':     //Determinazione stato attuatore
      tempTime = millis();

      if (dataTemperature<dataMin){
        attStat=2;
        digitalWrite(ledYellow, LOW);
      }

      else if (dataTemperature>dataMax){
        attStat=1;
        digitalWrite(ledRed, LOW);
      }

      else{
        attStat=0;
        digitalWrite(ledGreen, LOW);
      }

      
     //Condizione standard di print (ogni 5 minuti)
      if (tempTime >= time + 300000)
      {
        serialOut(dataTemperature,dataMin,dataMax,attStat);
        ethOut(dataTemperature,dataMin,dataMax,attStat);
        time = millis();
      }
      temp = mode;
      break;


      case 'S': //Input di nuove temperature di minima e massima

      
      
      dataMinTemp = Serial.parseFloat();
      dataMaxTemp = Serial.parseFloat();           

      if(connection)
      {
        dataMinTemp = client.parseFloat();
        dataMaxTemp = client.parseFloat();
      }

      if (dataMinTemp >= 5.0 && dataMinTemp <= 40.0 && dataMinTemp < dataMaxTemp )
      {
        dataMin = dataMinTemp;
      }

      if (dataMaxTemp >= 5.0 && dataMaxTemp <= 40.0 && dataMinTemp < dataMaxTemp)
      {
        dataMax = dataMaxTemp;
      }


      
      mode = temp;
      break;

        
      }
    delay(100);
}
