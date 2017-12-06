#include <Arduino.h>
#include <Ethernet.h>

#define REDPIN 3
#define GREENPIN 5
#define BLUEPIN 6

byte mac[] = {0x56, 0xA9, 0x71, 0x00, 0x18, 0x4E};
byte ip[] = {129, 21, 50, 27};
byte gateway[] = {129,21,76,254};
byte subnet[] = {255,255,255,0};

//String colors[] =
String currentColor = "";
String default_color = "0xC1007C";

EthernetServer server = EthernetServer(80);

String readString = String(100);

String testString = String(100);
String finalString = String(100);

// Color Definitions
String colorNames[] = {"white", "red", "yellow", "green", "aqua", "blue", "purple", "csh"};
String colorValues[] = {"0xFFFFFF", "0xFF0000", "0xFFFF00", "0x00FF00", "0x00FFFF", "0x0000FF", "0x800080", "0xC1007C"};
// Color Definitions

int indexOf(String array[], String pattern){
  for (int i = 0; i < 8; i++) {
    if (array[i].equals(pattern)) {
      return i;
    }
  }
  return -1;
}

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

String parseColor(String color){
  color.toLowerCase();
  if (color.substring(0, 2).equals("0x")) {
    return setColor(color);
  }else if(indexOf(colorNames, color) != -1){
    return setColor(colorValues[indexOf(colorNames, color)]);
  }else return "None";
}

String parseRequest(String request){
  String type = request.substring(0, request.indexOf(' '));
  String color = "";
  Serial.println(type);
  if (type == "POST") {
    int pos = request.indexOf('/');
    Serial.println(color = request.substring(pos+1, request.indexOf(' ', pos)));
    parseColor(color);
    return color;
  }else if (type == "GET"){
    return currentColor;
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
