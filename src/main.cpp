#include <Arduino.h>
#include <Ethernet.h>

#define REDPIN 3
#define GREENPIN 5
#define BLUEPIN 6

byte mac[] = {0x56, 0xA9, 0x71, 0x00, 0x18, 0x4E};
byte ip[] = {129, 21, 50, 27};
byte gateway[] = {129,21,76,254};
byte subnet[] = {255,255,255,0};


EthernetServer server = EthernetServer(80);

String readString = String(100);

String testString = String(100);
String finalString = String(100);

String flag = String(2);

void setColor(uint8_t red, uint8_t green, uint8_t blue){
  analogWrite(REDPIN, red);
  analogWrite(GREENPIN, green);
  analogWrite(BLUEPIN, blue);
}

void setColor(String hexColor){
    hexColor.toUpperCase();
    long number = (long) strtol( &hexColor[0], NULL, 16);
    int r = number >> 16;
    int g = number >> 8 & 0xFF;
    int b = number & 0xFF;

    analogWrite(REDPIN, r);
    analogWrite(GREENPIN, g);
    analogWrite(BLUEPIN, b);
}

void setup(){
  // initialize the ethernet device
  Ethernet.begin(mac, ip, gateway, subnet);

  // start listening for clients
  server.begin();

  Serial.begin(19200);

  pinMode(REDPIN, OUTPUT);
  pinMode(GREENPIN, OUTPUT);
  pinMode(BLUEPIN, OUTPUT);

  setColor(0, 0, 0);

  Serial.print("Server is at: ");
  Serial.println(Ethernet.localIP());
}

void loop(){
  // listen for incoming clients
  EthernetClient client = server.available();
  if (client) {
    Serial.println("new client");
    readString = "";
    // an http request ends with a blank line
    boolean currentLineIsBlank = true;
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        if (readString.length() < 99) {
          readString += c;
        }
        if (c == '\n' && currentLineIsBlank) {
          String color = "";
          Serial.println(readString);
          Serial.println(readString.indexOf('x'));
          int pos = readString.indexOf('x');
          color = readString.substring(pos+1, pos+7);
          Serial.println(color);


          setColor(color);

          client.println("Color set to: " + color);
          break;
        }
        if (c == '\n') {
          // you're starting a new line
          currentLineIsBlank = true;
        } else if (c != '\r') {
          // you've gotten a character on the current line
          currentLineIsBlank = false;
        }
      }
    }
    // give the web browser time to receive the data
    delay(1);
    // close the connection:
    client.stop();
    Serial.println();
    //Serial.println(readString);
    Serial.println("client disconnected");
  }
}
