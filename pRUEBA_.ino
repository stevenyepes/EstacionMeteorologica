#include <dht.h>
#include <math.h>
#include <SoftwareSerial.h>

//use mega Serial 2 for serial monitor; Serial 1 on pins 19 (RX) and 18 (TX);// Serial2 on pins 17 (RX) and 16 (TX), Serial3 on pins 15 (RX) and 14 (TX).
#define SSID "Telematica [Clase]"
#define PASS "Telem4tic5"
#define DST_IP "220.181.111.85" //baidu.com
SoftwareSerial dbgSerial(8,9); // RX, TX
const int ledPin = 7;
const int thresholdvalue = 10;
float Rsensor;
dht DHT;


#define DHT22_PIN 5

using namespace std;

float Temperatura = 56;
float Humedad = 59;
float Luz = 87;

void setup()
{
  Serial.begin(9600);
  dbgSerial.begin(9600); //can't be faster than 19200 for softserial
  dbgSerial.println("ESP8266 Demo");

  while (!Serial);

  Serial.println("ESP8266 Demo on Mega2560");

  while (dbgSerial.available() > 0)
    dbgSerial.read();

  delay(1000);
  //test if the module is ready
  dbgSerial.println("AT+RST");
  //delay(1000);
  //delay(1000);
  Serial.println("Resetting module");
  dbgSerial.flush();

  //if(Serial2.find("ready"))
  if (dbgSerial.find("Ready") || dbgSerial.find("ready"))
  {
    dbgSerial.println("Module is ready");
    Serial.println("Module is ready");
  }
  else
  {
    dbgSerial.println("Module have no response.");
    Serial.println("Module have no response.");
    while (1);
  }
  delay(1000);
  //connect to the wifi
  boolean connected = false;
  for (int i = 0; i < 5; i++)
  {
    if (connectWiFi())
    {
      connected = true;
      break;
    }
  }
  if (!connected) {
    while (1);
  }
  delay(1000);
  


  
  
  pinMode(ledPin, OUTPUT);  
  
  //leer_Temperatura_humedad();
  //lectura_Luz();
  send_Data(Temperatura, Humedad, Luz);


  Serial.println();
}

void loop()
{
  
  
}


void send_Data(float temperatura, float humedad, float luz)
{
  
  //set the single connection mode
  dbgSerial.println("AT+CIPMUX=1");
  Serial.println("AT+CIPMUX=1");
  dbgSerial.println("AT+CIPSTART=4,\"TCP\",\"184.106.153.149\",80");
  Serial.println("AT+CIPSTART=4,\"TCP\",\"184.106.153.149\",80");
  
  if (dbgSerial.find("Linked") || dbgSerial.find("LINKED"))
  {
    delay(1000);
    Serial.println("Module is Linked");
    
  }
  else
  {
    Serial.println("Module have no response.");
    while (1);
  }
  
  String cmd = "GET /update?key=XDY7EDG6OSAY8ILN&field1=";
  cmd += temperatura;
  cmd += "&field2=";
  cmd += humedad;
  cmd += "&field3=";
  cmd += luz;

  

  String cmd2 = "AT+CIPSEND=4,";
  cmd2 += cmd2.length;
  
  
  dbgSerial.println(cmd2);
  Serial.println(cmd2);
  //char *obtener = "GET /update?key=XDY7EDG6OSAY8ILN&field1="+25+"&field2="+25+"&field3="+25;
  dbgSerial.println(cmd);
  Serial.println(cmd);
  
  
}


boolean connectWiFi()
{
  dbgSerial.println("AT+CWMODE=1");
  String cmd = "AT+CWJAP=\"";
  cmd += SSID;
  cmd += "\",\"";
  cmd += PASS;
  cmd += "\"";
  //dbgSerial.println(cmd);
  dbgSerial.println(cmd);
  Serial.println(cmd);
  delay(2000);
  if (dbgSerial.find("OK"))
  {
    dbgSerial.println("OK, Connected to WiFi.");
    Serial.println("OK, Connected to WiFi.");
    return true;
  } else
  {
    dbgSerial.println("Can not connect to the WiFi.");
    Serial.println("Can not connect to the WiFi.");
    return false;
  }
}


void leer_Temperatura_humedad() {
  Serial.print("DHT22, \t");
  int chk = DHT.read22(DHT22_PIN);

  switch (chk)
  {
    case DHTLIB_OK:
      Serial.print("OK,\t");
      break;
    case DHTLIB_ERROR_CHECKSUM:
      Serial.print("Checksum error,\t");
      break;
    case DHTLIB_ERROR_TIMEOUT:
      Serial.print("Time out error,\t");
      break;
    default:
      Serial.print("Unknown error,\t");
      break;
  }

  Temperatura = DHT.temperature;
  Humedad = DHT.humidity;

  Serial.println("Type,\tstatus,\tHumidity (%),\tTemperature (C)");
  Serial.print(DHT.humidity, 1);
  Serial.print(",\t");
  Serial.println(DHT.temperature, 1);
  Serial.print(",\t");

}

void lectura_Luz() {

  // READ DATA

  int sensorValue = analogRead(0);
  Rsensor = (float)(1023 - sensorValue) * 10 / sensorValue;
  if (Rsensor > thresholdvalue)
  {
    digitalWrite(ledPin, HIGH);
  }
  else
  {
    digitalWrite(ledPin, LOW);
  }
  // DISPLAY DATA

  Serial.println("the analog read data is ");
  Serial.print(",\t");
  Serial.println(sensorValue);
  Serial.print(",\t");
  Serial.println("the sensor resistance is ");
  Serial.print(",\t");
  Serial.println(Rsensor, DEC); //show the light intensity on the serial monitor;
  Luz = Rsensor;
  delay(5000);
}

