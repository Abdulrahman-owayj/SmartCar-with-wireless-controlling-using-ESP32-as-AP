#include <WiFi.h>
#include <WebServer.h>
#include <analogWrite.h>
/* Put your SSID & Password */
const char* ssid = "ESP32";  // Enter SSID here
const char* password = "12345678";  //Enter Password here

/* Put IP Address details */
IPAddress local_ip(192,168,1,1);
IPAddress gateway(192,168,1,1);
IPAddress subnet(255,255,255,0);

WebServer server(80);

uint8_t motorB2 = 18;
uint8_t motorB1 = 5;
uint8_t pwmB = 19;
uint8_t motorA1 = 17;
uint8_t motorA2 = 16;
uint8_t pwmA = 4;
bool forward = LOW;
bool backward = LOW;
bool left = LOW;
bool right = LOW;
bool pause1 = HIGH;

void setup() {
  Serial.begin(230400);
  pinMode(motorB2, OUTPUT);
  pinMode(motorB1, OUTPUT);
  pinMode(pwmB, OUTPUT);
  pinMode(motorA1, OUTPUT);
  pinMode(motorA2, OUTPUT);
  pinMode(pwmA, OUTPUT);
  
  WiFi.softAP(ssid, password);
  WiFi.softAPConfig(local_ip, gateway, subnet);
  delay(100);
  
  server.on("/", handle_OnConnect);
  server.on("/forward", handle_forward);
  server.on("/backward", handle_backward);
  server.on("/left", handle_left);
  server.on("/right", handle_right);
  server.on("/pause1", handle_pause1);
  
  server.onNotFound(handle_NotFound);
  
  server.begin();
  Serial.println("HTTP server started");
}
void loop() {
  server.handleClient();
  if(forward)
  {
    digitalWrite(motorB1, HIGH);
    digitalWrite(motorB2, LOW);  
    digitalWrite(motorA1, LOW);
    digitalWrite(motorA2, HIGH); 
    analogWrite(pwmA, 150);
    analogWrite(pwmB, 150); 
  }
  else if(backward)
  {
    digitalWrite(motorB1, LOW);
    digitalWrite(motorB2, HIGH);  
    digitalWrite(motorA1, HIGH);
    digitalWrite(motorA2, LOW);
    analogWrite(pwmA, 150);
    analogWrite(pwmB, 150); 
    }
 else if (left){
    digitalWrite(motorB1, LOW);
    digitalWrite(motorB2, LOW);  
    digitalWrite(motorA1, HIGH);
    digitalWrite(motorA2, HIGH
    );
    analogWrite(pwmA, 255);
    analogWrite(pwmB, 255); 
  } 
   else if (right){
    digitalWrite(motorB1, HIGH);
    digitalWrite(motorB2, HIGH);  
    digitalWrite(motorA1, HIGH);
    digitalWrite(motorA2, LOW);
    analogWrite(pwmA, 255);
    analogWrite(pwmB, 255); 
  } 
 else if (pause1){
    digitalWrite(motorB1, HIGH);
    digitalWrite(motorB2, HIGH);  
    digitalWrite(motorA1, HIGH);
    digitalWrite(motorA2, HIGH);
    analogWrite(pwmA, 150);
    analogWrite(pwmB, 150); 
  } 
  }

void handle_OnConnect() {
  forward = LOW;
  backward = LOW;
  left = LOW;
  right = LOW;
  pause1 = HIGH ; 
 // Serial.println("GPIO4 Status: OFF | GPIO5 Status: OFF");
  server.send(200, "text/html", SendHTML(forward,backward)); 
}

void handle_forward() {
  forward = HIGH;
  backward = LOW;
  left = LOW;
  right = LOW;  
  pause1 = LOW; 
  Serial.println("forward: ON");
  server.send(200, "text/html", SendHTML(true,false)); 
}

void handle_backward() {
  backward = HIGH;
  forward = LOW;
    left = LOW;
  right = LOW;  
  pause1 = LOW; 
  Serial.println("backward : ON");
  server.send(200, "text/html", SendHTML(false,true));  
}
void handle_left() {
  backward = LOW;
  forward = LOW;
    left = HIGH;
  right = LOW;  
  pause1 = LOW; 
  Serial.println("left : ON");
  server.send(200, "text/html", SendHTML(false,true)); 
}
void handle_right() {
  backward = LOW;
  forward = LOW;
    left = LOW;
  right = HIGH;  
  pause1 = LOW; 
  Serial.println("right : ON");
  server.send(200, "text/html", SendHTML(false,true)); 
}
void handle_pause1() {
  backward = LOW;
  forward = LOW;
  left = LOW;
  right = LOW;  
  pause1 = HIGH; 
  Serial.println("pause1 : ON");
  server.send(200, "text/html", SendHTML(false,true)); 
}
void handle_NotFound(){
  server.send(404, "text/plain", "Not found");
}

String SendHTML(uint8_t forward,uint8_t backward){
  char webpage[] PROGMEM = R"=====(

  <!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta http-equiv="X-UA-Compatible" content="IE=edge">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Second Task</title>

</head>
<body>
   
        <div class="container">
            <div class="card one">
                <p><a href="/forward"><button class="button">forward</button></a></p>
            </div>
            <div class="card two">
                <p><a href="/left"><button class="button">Left</button></a></p>
                <p><a href="/pause1"><button class="button">pause</button></a></p>
                <p><a href="/right"><button class="button">right</button></a></p>
            </div>
            <div class="card three">
                <p><a href="/backward"><button class="button">backward</button></a></p>
            </div>
        </div> 
 
</body>
</html>
)=====";
  return webpage;
}
