#include <IRremoteESP8266.h>
#include <IRsend.h>
#include <IRrecv.h>
#include <IRutils.h>
const uint16_t kIrLed = D6;  // ESP8266 GPIO pin to use. Recommended: 4 (D2).
const uint16_t kRecvPin = D5;

IRrecv irrecv(kRecvPin);
IRsend irsend(kIrLed);  // Set the GPIO to be used to sending the message.
decode_results results;
String sendIRCommand = "";


void initIR(){
  irsend.begin();
  irrecv.enableIRIn();  // Start the receiver
}

void sendIRPower(){
  Serial.println("sendIRPower");
  irsend.sendNEC(0x00E0E040BF);  
}

void sendIRVolumeUp(){
  Serial.println("sendIRVolumeUp");
  irsend.sendNEC(0x00E0E0E01F);  
}

void sendIRVolumeDonw(){
  Serial.println("sendIRVolumeDonw");
  irsend.sendNEC(0x00E0E0D02F);
}

void sendIRProgramUp(){
  Serial.println("sendIRProgramUp");
  irsend.sendNEC(0x00E0E048B7);  
}

void sendIRProgramDown(){
  Serial.println("sendIRProgramDown");
  irsend.sendNEC(0x00E0E008F7);
}

void sendIR(String irCommand){
  Serial.print("sendIR:");
  Serial.println(irCommand);
  char irChar[50];
  irCommand.toCharArray(irChar, irCommand.length()+1); 
  long unsigned int b = strtoul(irChar, 0, 16);
  irsend.sendNEC(b);
}


void handleIR(){
      if (irrecv.decode(&results)) {
    // print() & println() can't handle printing long longs. (uint64_t)
      serialPrintUint64(results.value, HEX);
      long code = results.value;
      char out[20];
      sprintf(out,"%08X",code);
      String IRmessage = String(out);
      Serial.print("send MQTT IRRecive/Signal:");
      Serial.println(IRmessage);
      sendMQTT("IRRecive/Signal",IRmessage);
      receivedCommands += "<br>"+IRmessage;
      irrecv.resume();  // Receive the next value
    }

}
