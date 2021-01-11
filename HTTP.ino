#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>


ESP8266WebServer server(80);


void handleRoot() {
  String message = "Number of args received:";
  message += server.args();            //Get number of parameters
  message += "\n";                            //Add a new line
  for (int i = 0; i < server.args(); i++) {
    message += "Arg nº" + (String)i + " –> ";   //Include the current iteration value
    message += server.argName(i) + ": ";     //Get the name of the parameter
    message += server.arg(i) + "\n";              //Get the value of the parameter
    if (server.argName(i) == "IR" && server.arg(i) != "") {
      String sendIRCommand =  server.arg(i);
      sendIR(sendIRCommand);
    }
    else if (server.argName(i) == "action") {
      if (server.arg(i) == "Power") {    sendIRPower(); }
      if (server.arg(i) == "Clear") {    receivedCommands = ""; }
      if (server.arg(i) == "V-") {       sendIRVolumeDonw(); }
      if (server.arg(i) == "V+") {       sendIRVolumeUp(); }
      if (server.arg(i) == "P-") {       sendIRProgramDown(); }
      if (server.arg(i) == "P+") {       sendIRProgramUp(); }          
    }
  } 
  Serial.println("HTTP Root");
  Serial.println(message);
  String htmlResponse = R"rawliteral(<html><body><form action='/' method='POST'><br><br><button type="submit" name="action" value="Power">TV Power</button><br>Volume<br><button type="submit" name="action" value="V-" >-</button><button type="submit" name="action" value="V+" >+</button><br>Programm<br><button type="submit" name="action" value="P-" >-</button><button type="submit" name="action" value="P+" >+</button><br><br>IR Command<br><input type='text' name='IR' placeholder=''><input type='submit' name='SUBMIT' value='Submit'><br><br>$MESSAGES$<br><button type="submit" name="action" value="Clear">Clear</button></form><br>You also can go <a href='/inline'>here</a></body></html>)rawliteral";
  htmlResponse.replace("$MESSAGES$",receivedCommands); 
  server.send(200, "text/html", htmlResponse);
  delay(100);
  
}



void handleNotFound() {
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET) ? "GET" : "POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";

  for (uint8_t i = 0; i < server.args(); i++) {
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }

  server.send(404, "text/plain", message);
  
}


void initHTTP(void) {
  server.on("/", handleRoot);
  server.on("/inline", []() {
    server.send(200, "text/plain", "this works as well");
  });
  server.onNotFound(handleNotFound);
  server.begin();
  Serial.println("HTTP server started");
}

void doHTTP(void) {
  server.handleClient();
}
