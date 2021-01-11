#include <ESP8266WiFi.h>
#include <Credentials.h>



String publisher[] = {"IRSend"};
String receivedCommands = "";



void setup()
{
  Serial.begin(115200);
  delay(50);
  initWIFI();
  initIR();
  delay(50);
  initMQTT();
  initHTTP();
  
}




void loop()
{
    handleMQTT();
    handleIR();
    doHTTP();
    delay(20);
}
