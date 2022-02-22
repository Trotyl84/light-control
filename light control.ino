/*
Thangs:
http://davidjwatts.com/youtube/esp8266/esp-convertHTM.html# 
*/
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>

/* Create a WiFi access point and provide a web server on it. */
/* You can remove the password parameter if you want the AP to be open. */
#ifndef APSSID
#define APSSID "ESPap" 
#define APPSK  "thereisnospoon" 
#endif
const char *ssid = APSSID;
const char *password = APPSK;
ESP8266WebServer server(80);

const int pin_light = 1;//D1 // 
byte light_status=0;




const String goBack ="<html> <head> <script> function goBack() { window.history.go(-1); } </script> </head> <body onload=\"goBack()\"> <button onclick=\"goBack()\">Go Back</button>  </script> </body> </html>";

void ON(){ 
  digitalWrite(pin_light, HIGH);
  light_status=1;
}

void OFF(){ 
  digitalWrite(pin_light, LOW);  
  light_status=0;
}

void handleNotFound(){
	server.send(440, "text/html","Page is not faund");
	Serial.println("HTTP 440");
}

void handlesend(){ Serial.println("HTTP handlesend");
  String message ;
  for (uint8_t i=0; i<server.args(); i++){
		message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
		if(server.arg(i)=="ON"){
			ON(); 
			//Serial.println("ON");
		}
		if(server.arg(i)=="OFF"){
			//Serial.println("OFF");
			OFF();
		}
	}
	//server.send(200, "text/html", message); // dev
	server.send(200, "text/html", goBack); // 
	//serial.println("handlesend");
}

void handle_status(){
	String light_status_js;
	light_status_js ="var light_status= " + String(light_status)+" ;" ;
	server.send(200, "text/html", light_status_js);
}

void setup() {
  delay(1000);
  Serial.begin(115200);
  pinMode(pin_light, OUTPUT);
  
  WiFi.softAP(ssid, password);
  IPAddress myIP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(myIP);
  
  server.on("/send.html", handlesend );
  server.on("/status.js", handle_status);
  //server.on("/", handlesend);
  server.onNotFound(handleNotFound);
  server.begin();
  Serial.println("HTTP server started");
}

void loop() {
  server.handleClient(); 
}
