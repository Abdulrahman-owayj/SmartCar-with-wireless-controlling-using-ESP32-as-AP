# SmartCar-with-wireless-controlling-using-ESP32-as-AP
<br/>

## About the Project
<br/>

This project about a Smart Car with wireless controlling using microcontroller ESP32 in AP mode (Access Point).
Here i used ESP32 to control 2 motors with 4 wheels with H-bridge motor controller (TB6612FNG)

<img src="https://user-images.githubusercontent.com/5675794/130445014-ffd060d6-66bc-4091-ab76-dcd984f68b8a.png" alt="alt text" width="300" height="300"> <img src="https://user-images.githubusercontent.com/5675794/130444999-84827549-35cc-4e8e-a57a-6a3a4e895658.png" alt="alt text" width="300" height="300"> <img src="https://user-images.githubusercontent.com/5675794/130445030-8f4e3c18-57fb-46a1-a897-eefda05399f3.png" alt="alt text" width="300" height="300">



<br/>

## Background <br/>
### What is ESP32 
ESP32 is a low-cost, low-power system on a chip microcontrollers with dual-core and integrated Wi-Fi and dual-mode Bluetooth. it has different power modes including deep-sleep mode with 5uA only.
<br/>

### Why ESP32
ESP32 has advantage over other microcontrollers because of its small form factor and lower power consumtions as well as the integerated WIFI and BLE.
<br/>

### Previoues Projects <br/>
Before working in this project, i implemented alot of small projects which helped me with the car project like: <br/> 
1- Controlling the ESP32's LED through Wifi. <br/>
2- Controlling a single motor using ESP32 as access point <br/>
3- Controlling 2 or more LED using webpage with ESP32 as server (AP) <br/>
<br/>

## ESP32
![ESP32](https://user-images.githubusercontent.com/5675794/130447355-1972fd3d-514d-4743-ae26-0bb2fc784c8a.jpg) <br/> <br/>

### ESP32 Access Point Mode (AP) <br/>
The ESP32 that creates its own WiFi network and acts as a hub (Just like WiFi router) for one or more stations is called Access Point (AP). Unlike WiFi router, it does not have interface to a wired network. So, such mode of operation is called Soft Access Point (soft-AP). Also the maximum number of stations that can connect to it is limited to five.
<br/>

<br/>

## Motor Driver (TB6612FNG)

<img src="https://user-images.githubusercontent.com/5675794/130448700-76826a1a-bd1a-4916-82d6-4a4471a27508.jpeg" alt="alt text" width="600" height="400"> 


# Smart Car with wireless control using ESP32 as access point (Web Server) <br/>
<img src="https://user-images.githubusercontent.com/5675794/130461118-36ee4123-e561-4844-8394-6100970635a0.jpeg" alt="alt text" width="500" height="400">
<img src="https://user-images.githubusercontent.com/5675794/130461128-4dc63c90-3b1c-4dea-8c14-9c9504438776.jpeg" alt="alt text" width="500" height="400">

### Real-Time Simulation

https://user-images.githubusercontent.com/5675794/130460859-3557d418-6527-4f81-9a6d-5dddc842c543.mp4 



<br/>

## Code <br/>
```ruby
#include <WiFi.h>
#include <WebServer.h>
#include <analogWrite.h>
/* Put your SSID & Password */
const char* ssid = "ESP32";                          // Enter SSID here
const char* password = "12345678";                   //Enter Password here

/* Put IP Address details */
IPAddress local_ip(192,168,1,1);                     // the IP which will have the webpage
IPAddress gateway(192,168,1,1);
IPAddress subnet(255,255,255,0);

WebServer server(80);                                // initialising a server though port 80

uint8_t motorB2 = 18;                                // declaring all the variables
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
  Serial.begin(230400);                             // Declaring the Pins mode
  pinMode(motorB2, OUTPUT);
  pinMode(motorB1, OUTPUT);
  pinMode(pwmB, OUTPUT);
  pinMode(motorA1, OUTPUT);
  pinMode(motorA2, OUTPUT);
  pinMode(pwmA, OUTPUT);
  
  WiFi.softAP(ssid, password);                      //starting the access point            
  WiFi.softAPConfig(local_ip, gateway, subnet);
  delay(100);
  
  server.on("/", handle_OnConnect);                 // using the (on) command to let the page respond to each / to execute different code
  server.on("/forward", handle_forward);
  server.on("/backward", handle_backward);
  server.on("/left", handle_left);
  server.on("/right", handle_right);
  server.on("/pause1", handle_pause1);
  
  server.onNotFound(handle_NotFound);               // execute when the page /  not recognised 
  
  server.begin();                                   // Initialize the server
  Serial.println("HTTP server started");
}
void loop() {
  server.handleClient();                            // to execute the handle request
  if(forward)                                       // if the forward button pressed and the forward varabile triggered high
  {
    digitalWrite(motorB1, HIGH);
    digitalWrite(motorB2, LOW);  
    digitalWrite(motorA1, LOW);
    digitalWrite(motorA2, HIGH); 
    analogWrite(pwmA, 150);
    analogWrite(pwmB, 150); 
  }
  else if(backward)                                   // if the backward button pressed and the backward varabile triggered high
  {
    digitalWrite(motorB1, LOW);
    digitalWrite(motorB2, HIGH);  
    digitalWrite(motorA1, HIGH);
    digitalWrite(motorA2, LOW);
    analogWrite(pwmA, 150);
    analogWrite(pwmB, 150); 
    }
 else if (left){                                       // if the left button pressed and the left varabile triggered high
    digitalWrite(motorB1, LOW);
    digitalWrite(motorB2, LOW);  
    digitalWrite(motorA1, HIGH);
    digitalWrite(motorA2, HIGH);
    analogWrite(pwmA, 255);
    analogWrite(pwmB, 255); 
  } 
   else if (right){                                     // if the right button pressed and the right varabile triggered high
    digitalWrite(motorB1, HIGH);
    digitalWrite(motorB2, HIGH);  
    digitalWrite(motorA1, HIGH);
    digitalWrite(motorA2, LOW);
    analogWrite(pwmA, 255);
    analogWrite(pwmB, 255); 
  } 
 else if (pause1){                                       // if the pause button pressed and the pause varabile triggered high
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

void handle_forward() {                                 // execute when the button pressed via the (server.on) command
  forward = HIGH;
  backward = LOW;
  left = LOW;
  right = LOW;  
  pause1 = LOW; 
  Serial.println("forward: ON");
  server.send(200, "text/html", SendHTML(true,false));         // .Send allows the us to receive automatic updates from a server via HTTP connection while SendHTML function which rebuild the webpage with the updated values
}

void handle_backward() {
  backward = HIGH;
  forward = LOW;
    left = LOW;
  right = LOW;  
  pause1 = LOW; 
  Serial.println("backward : ON");
  server.send(200, "text/html", SendHTML(false,true));          // .Send allows the us to receive automatic updates from a server via HTTP connection while SendHTML function which rebuild the webpage with the updated values
}
void handle_left() {
  backward = LOW;
  forward = LOW;
    left = HIGH;
  right = LOW;  
  pause1 = LOW; 
  Serial.println("left : ON");
  server.send(200, "text/html", SendHTML(false,true));          // .Send allows the us to receive automatic updates from a server via HTTP connection while SendHTML function which rebuild the webpage with the updated values
}
void handle_right() {
  backward = LOW;
  forward = LOW;
    left = LOW;
  right = HIGH;  
  pause1 = LOW; 
  Serial.println("right : ON");
  server.send(200, "text/html", SendHTML(false,true));           // .Send allows the us to receive automatic updates from a server via HTTP connection while SendHTML function which rebuild the webpage with the updated values
}
void handle_pause1() {
  backward = LOW;
  forward = LOW;
  left = LOW;
  right = LOW;  
  pause1 = HIGH; 
  Serial.println("pause1 : ON");
  server.send(200, "text/html", SendHTML(false,true));            // .Send allows the us to receive automatic updates from a server via HTTP connection while SendHTML function which rebuild the webpage with the updated values
}
void handle_NotFound(){
  server.send(404, "text/plain", "Not found");
}

String SendHTML(uint8_t forward,uint8_t backward){                     //HTML code for the webpage in the ESP32
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
   
        <div class="container">                                                                 //parent class
            <div class="card one">                                                              //class for the row
                <p><a href="/forward"><button class="button">forward</button></a></p>            // forward button
            </div>
            <div class="card two">
                <p><a href="/left"><button class="button">Left</button></a></p>                  // forward button 
                <p><a href="/pause1"><button class="button">pause</button></a></p>               // forward button
                <p><a href="/right"><button class="button">right</button></a></p>                // forward button
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
```
