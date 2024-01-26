#include <ESP8266WiFi.h>      
#include <ESP8266WebServer.h> 

ESP8266WebServer server(80);
#define leftMotorForward D2
#define leftMotorBackward D3
#define rightMotorForward D0
#define rightMotorBackward D1

void stop() {
  digitalWrite(leftMotorForward, LOW);
  digitalWrite(leftMotorBackward, LOW);
  digitalWrite(rightMotorForward, LOW);
  digitalWrite(rightMotorBackward, LOW);
}

void move_forward() {
  digitalWrite(leftMotorForward, LOW);
  digitalWrite(leftMotorBackward, HIGH);
  digitalWrite(rightMotorForward, LOW);
  digitalWrite(rightMotorBackward, HIGH);
}

void move_backward() {
  digitalWrite(leftMotorForward, HIGH);
  digitalWrite(leftMotorBackward, LOW);
  digitalWrite(rightMotorForward, HIGH);
  digitalWrite(rightMotorBackward, LOW);
}

void turn_left() {
  digitalWrite(leftMotorForward, HIGH);
  digitalWrite(leftMotorBackward, LOW);
  digitalWrite(rightMotorForward, LOW);
  digitalWrite(rightMotorBackward, HIGH);
}

void turn_right() {
  digitalWrite(leftMotorForward, LOW);
  digitalWrite(leftMotorBackward, HIGH);
  digitalWrite(rightMotorForward, HIGH);
  digitalWrite(rightMotorBackward, LOW);
}



const char* ssid = "TNCAP72B3D8";
const char* password = "**********";



void setup(void)
{ 
  Serial.begin(115200);        
  pinMode(leftMotorForward, OUTPUT);
  pinMode(leftMotorBackward, OUTPUT);
  pinMode(rightMotorForward, OUTPUT);
  pinMode(rightMotorBackward, OUTPUT);
  stop();
  WiFi.begin(ssid, password);  

  while (WiFi.status() != WL_CONNECTED) 
  {
    delay(500);
    Serial.print("."); 
  } 
      Serial.println("");
    Serial.print("Connected to ");
    Serial.println(ssid);
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());

    server.on("/forward", [](){ 
    server.send(200, "text/plain", "going forward"); 
    move_forward();
  });

  server.on("/backward", [](){           
    server.send(200, "text/plain", "going backward"); 
    move_backward();       
  });

  server.on("/left", [](){                 
    server.send(200, "text/plain", "going left"); 
    turn_left();        
  }); 

  server.on("/right", [](){ 
    server.send(200, "text/plain", "going right"); 
    turn_right();
  });

  server.on("/stop_forward", [](){ 
    stop();
    server.send(200, "text/plain", "stopped forward"); 
 });

 server.on("/stop_backward", [](){      
    stop(); 
    server.send(200, "text/plain", "stopped backward"); 
 });

 server.on("/stop_left", [](){         
    stop();
    server.send(200, "text/plain", "stopped left"); 
 }); 

 server.on("/stop_right", [](){ 
    stop();
    server.send(200, "text/plain", "stopped right"); 
 });

  server.on("/", [](){ 
    server.send(200, "text/html", R"===(<!DOCTYPE html>
    <html>
      <head>
        <title>Robot Controller</title>
        <style>
            * {
                margin: 0;
                padding: 0;
            }

            body {
                height: 100vh;
                width: 100%;
                background-color: black;
                position: relative;
            }

            button {
                padding: 10px;
                height: 100px;
                width: 100px;
                border-radius: 100%;
                border: none;
                font-size: 50px;
                font-weight: bold;
                cursor: pointer;
                position: absolute;
                top: 50%;
                left: 50%;
                transform: translate(-50%, -50%);
            }

            #forward {
                top: 30%;
                left: 50%;
                transform: translate(-50%, -50%);
                background-color: red;
            }

            #backward {
                top: 70%;
                left: 50%;
                transform: translate(-50%, -50%);
                background-color: green;
            }

            #left {
                top: 50%;
                left: 30%;
                transform: translate(-50%, -50%);
                background-color: yellow;
            }
            #right {
                top: 50%;
                left: 70%;
                transform: translate(-50%, -50%);
                background-color: blue;
            }

            #center {
                background-color: gray;
            }
        </style>
      </head>
      <body>
            <button id="forward" onmousedown="forward()" onmouseup="stop_forward()" ontouchstart="forward()" ontouchend="stop_forward()">F</button>
            <button id="backward" onmousedown="backward()" onmouseup="stop_backward()" ontouchstart="backward()" ontouchend="stop_backward()">B</button>
            <button id="left" onmousedown="left()" onmouseup="stop_left()" ontouchstart="left()" ontouchend="stop_left()">L</button>
            <button id="right" onmousedown="right()" onmouseup="stop_right()" ontouchstart="right()" ontouchend="stop_right()">R</button>
            <button id="center" disabled></button>

            <script>
              function forward() { sendCommand("/forward"); }
              function backward() { sendCommand("/backward"); }
              function left() { sendCommand("/left"); }
              function right() { sendCommand("/right"); }

              function stop_forward() { sendCommand("/stop_forward"); }
              function stop_backward() { sendCommand("/stop_backward"); }
              function stop_left() { sendCommand("/stop_left"); }
              function stop_right() { sendCommand("/stop_right"); }

              function sendCommand(command) {
                var xhttp = new XMLHttpRequest();
                xhttp.open("GET", command, true);
                xhttp.send();
              }
            </script>
      </body>
    </html>)===");
  });

  server.begin();                  
}

void loop(void)
{ 
  server.handleClient();
}

