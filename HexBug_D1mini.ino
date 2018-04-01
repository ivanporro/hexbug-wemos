#include <ESP8266WiFi.h>
#include "WEMOS_Motor.h"

// HC-SR04
#define echoPin D7    // Echo Pin
#define trigPin D6    // Trigger Pin

#define red   D4    
#define green D8    
#define blue  D3    

#define DBG     true  // debug

// L = locomotion
// H = head

//Motor shiled I2C Address: 0x30
//PWM frequency: 1000Hz(1kHz)
Motor ML(0x30,_MOTOR_A, 1000);//Motor A  // LEG
Motor MH(0x30,_MOTOR_B, 1000);//Motor B  // HEAD
int pwml = 100;
int pwmh = 100;
int loco = 0;
int head = 0;

// WiFi setup
const char* ssid = "Vodafone-mysite";
const char* password = "superbio05!";
WiFiServer server(80);
 
long dist=100;
long safedist = 8;  // cm
 
long getDistance()
{
  long duration, distance; // Duration used to calculate distance
  // The following trigPin/echoPin cycle is used to determine the
  // distance of the nearest object by bouncing soundwaves off of it. 
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  
  // Calculate the distance (in cm) based on the speed of sound.
  distance = duration/58.2;
  dbg("calculated distance: " + (String)distance);
  
  // Delay 50ms before next reading
  delay(50);
  return distance;
}




// motion control

void dbg(String txt) {  
  if(DBG) Serial.println(txt);
};

void loco_fwd() { 
  ML.setmotor(_CW, pwml);
  dbg("loco fwd");
}

void loco_back() { 
  ML.setmotor(_CCW, pwml);
  dbg("loco back");
}

void loco_stop() { 
  ML.setmotor(_STOP);
  dbg("loco stop");
}


void head_left() { 
  MH.setmotor(_CW, pwmh);
  dbg("head left");
  }

void head_right() { 
  MH.setmotor(_CCW, pwmh);
  dbg("head right");
  }

void head_stop() { 
  MH.setmotor(_STOP);
  dbg("head stop");
  }

void head_nono() {
  dbg("head nono");
  MH.setmotor(_CW);
  led_blink(red);
  MH.setmotor(_CCW);
  led_blink(red);
  MH.setmotor(_CW);
  delay(50);
  MH.setmotor(_CCW);
  delay(50);
  MH.setmotor(_STOP);
  led_blink(green);
  }
  



void led_on(int pin)  {  digitalWrite(pin, HIGH);   }
void led_off(int pin) {  digitalWrite(pin, LOW);    }

void led_toggle(int pin) {  digitalWrite(pin, !digitalRead(pin));    }

void led_blink(int pin) {
  digitalWrite(pin, HIGH);  
  delay(50);
  digitalWrite(pin, LOW);  
  delay(50);
  }

 
void setup() {
  Serial.begin(115200);
  delay(100);

  // HC-SR04
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  // led
  pinMode(red, OUTPUT);
  pinMode(green, OUTPUT);
  pinMode(blue, OUTPUT);

  digitalWrite(red, LOW);
  digitalWrite(green, LOW);
  digitalWrite(blue, LOW);
  
  
  // stop
  loco_stop();
  head_stop();

  // Connect to WiFi network
  Serial.println("Motor L & H STOP");
  Serial.print("Connecting to ");
  Serial.println(ssid);
 
  WiFi.begin(ssid, password);
 
  while (WiFi.status() != WL_CONNECTED) {
    led_blink(blue);
    Serial.print(".");
  }
  Serial.println("");
  led_on(blue);
  delay(500);
  led_off(blue);
  Serial.println("WiFi connected");
 
  // Start the server
  server.begin();
  Serial.println("Server started");
 
  // Print the IP address
  Serial.print("Use this URL : ");
  Serial.print("http://");
  Serial.print(WiFi.localIP());
  Serial.println("/");
 
}
 
void loop() {

  // Check if a client has connected
  WiFiClient client = server.available();
  if (!client) {

    delay(100);
    dist = getDistance();
    
    if(loco == 1) {
        // safety check
      if (dist > safedist) { 
        loco_fwd(); 
      }    
      else { // Alt!
        loco_stop(); 
        loco = 0;  
        head = 0;
        head_nono();
        
      }
    }
    else if(loco == -1) loco_back();
    else if(loco == 0)  loco_stop();
    else loco_stop();
    
    if(head == 1)  head_right();
    else if(head == -1)  head_left();
    else if(head == 0)  head_stop();
    else head_stop();
    
    return;
  }
 
  // Wait until the client sends some data
  dbg("new client");
  while(!client.available()){
    delay(1);
  }
 
  // Read the first line of the request
  String request = client.readStringUntil('\r');
  dbg(request);
  client.flush();
 
  // Match the request & move
  if (request.indexOf("/L=f") != -1) { loco = 1;  }
  if (request.indexOf("/L=b") != -1) { loco = -1; }
  if (request.indexOf("/L=s") != -1) { loco = 0;  } 
 
  if (request.indexOf("/H=l") != -1) { head = -1; } 
  if (request.indexOf("/H=r") != -1) { head = 1;  } 
  if (request.indexOf("/H=s") != -1) { head = 0;  } 


  if (request.indexOf("/led=r") != -1) { led_toggle(red);  } 
  if (request.indexOf("/led=g") != -1) { led_toggle(green);  } 
  if (request.indexOf("/led=b") != -1) { led_toggle(blue);  } 

  // Return the response
  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/html");
  client.println(""); //  do not forget this one
  client.println("<!DOCTYPE HTML>");
  client.println("<html>");
  client.println("<br>Locomotion<br>");
  client.println("Click <a href=\"/L=f\">Forward</a><br>");
  client.println("Click <a href=\"/L=b\">Back</a><br>");
  client.println("Click <a href=\"/L=s\">Stop</a><br>");

  client.println("<br>Head<br>");
  client.println("Click <a href=\"/H=l\">Left</a><br>");
  client.println("Click <a href=\"/H=r\">Right</a><br>");
  client.println("Click <a href=\"/H=s\">Stop</a><br>");


  client.println("Click <a href=\"/led=r\">Led_R</a><br>");
  client.println("Click <a href=\"/led=g\">Led_G</a><br>");
  client.println("Click <a href=\"/led=b\">Led_B</a><br>");

  client.println("</html>");
 
  delay(1);
  dbg("Client disconnected");
  dbg("");

}



