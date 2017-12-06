/*
An Arduino sketch that receives POST requests to change the color of an LED strip.

Author: Seth Gower
sethgower.com
*/

#include <Arduino.h>
#include <Ethernet.h>

#define REDPIN 3
#define GREENPIN 5
#define BLUEPIN 6

//defines the internet interface properties
byte mac[] = {0x56, 0xA9, 0x71, 0x00, 0x18, 0x4E}; //MAC address
byte ip[] = {129, 21, 50, 27}; //IPv4 address.
byte gateway[] = {129,21,76,254};
byte subnet[] = {255,255,255,0};

String default_color = "0xC1007C"; //the CSH pink
String currentColor = default_color;

EthernetServer server = EthernetServer(80); //opens a server on port 80, the default HTTP port

//Strings used when reading the HTTP request
String readString = String(100);
String finalString = String(100);

//Set the color of the strip to a certain hex color
String setColor(String hexColor){
    hexColor.toUpperCase();
    String tempColor = hexColor.substring(2);
    Serial.println(tempColor);
    long number = (long) strtol( &tempColor[0], NULL, 16);
    int r = number >> 16;
    int g = number >> 8 & 0xFF;
    int b = number & 0xFF;

    analogWrite(REDPIN, r);
    analogWrite(GREENPIN, g);
    analogWrite(BLUEPIN, b);

    currentColor = hexColor;
    return tempColor;
}

//Parses the HTTP request embedded in the string passed to the method. The format is [POST,GET] /[color] HTTP/1.1
String parseRequest(String request){
  String type = request.substring(0, request.indexOf(' '));
  String color = "";
  Serial.println(type);
  if (type == "POST") {
    int pos = request.indexOf('/');
    Serial.println(color = request.substring(pos+1, request.indexOf(' ', pos)));
    setColor(color);
    return color;
  }
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

  setColor(default_color);

  Serial.print("Server is at: ");
  Serial.println(Ethernet.localIP());

}

void loop(){
  // listen for incoming clients
  EthernetClient client = server.available();
  if (client) {
    Serial.println("new client");
    readString = "";
    finalString = "";
    // an http request ends with a blank line
    boolean currentLineIsBlank = true;
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        if (readString.length() < 100) {
          readString += c;
        }
        //if there isn't more info being transmitted from the client
        if (c == '\n' && currentLineIsBlank) {
          finalString = readString.substring(0, readString.indexOf('\n'));
          parseRequest(finalString);
          Serial.println(finalString);
          client.println("Color set to: " + currentColor);
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
