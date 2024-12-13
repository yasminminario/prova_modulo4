#include <WiFi.h>
#include <HTTPClient.h>

#define blue_led 9 // Pin used to control the blue led
#define green_led 41 // Pin used to control the green led
#define red_led 40 // Pin used to control the red led
#define yellow_led 9 // Pin used to control the yellow led

const int buttonPin = 18;  // the number of the pushbutton pin
int buttonState = 0;  // variable for reading the pushbutton status

const int ldrPin = 4;  // the number of the pushbutton pin
int threshold = 600;

void setup() {

  // inicial pin settings to control the leds as esp32 OUTPUTs
  pinMode(blue_led,OUTPUT);
  pinMode(green_led,OUTPUT);
  pinMode(red_led,OUTPUT);

  // Initialize the inputs
  pinMode(buttonPin, INPUT); // Initialize the pushbutton pin as an input

  digitalWrite(blue_led, LOW);
  digitalWrite(green_led, LOW);
  digitalWrite(red_led, LOW);

  Serial.begin(9600); // debug setting through serial interface between ESP and computer with 9600 baud rate

  WiFi.begin("Wokwi-GUEST", ""); // connection to open WiFi Network with SSID Wokwi-GUEST

  while (WiFi.status() != WL_CONNECT_FAILED) {
    delay(100);
    Serial.print(".");
  }
  Serial.println("Successfully connected to WiFi!"); // considering it is out of the loop, ESP32 is now connected to the WiFi (another option is to put inside the following if)

  // verifies the button state
  buttonState = digitalRead(buttonPin);
  if (buttonState == HIGH) {
    delay(1000);
    digitalWrite(red_led, LOW);
    digitalWrite(green_led, HIGH);
    delay(3000);
    Serial.println("Button pressed!");
  } else {
    Serial.println("Button not pressed!");
  }

  if(WiFi.status() == WL_CONNECTED){ // if esp32 is connected to the internet
    HTTPClient http;

    String serverPath = "http://www.google.com.br/"; // HTTP request Endpoint

    http.begin(serverPath.c_str());

    int httpResponseCode = http.GET(); // result of HTTP request

    if (httpResponseCode>0) {
      Serial.print("HTTP Response code: ");
      Serial.println(httpResponseCode);
      String payload = http.getString();
      Serial.println(payload);
      }
    else {
      Serial.print("Error code: ");
      Serial.println(httpResponseCode);
      }
      http.end();
    }

  else {
    Serial.println("WiFi Disconnected");
  }
}

void loop() {
  int ldrstatus=analogRead(ldrPin);

  if(ldrstatus<=threshold){
    Serial.print("its dark turn on led");
    Serial.println(ldrstatus);
    digitalWrite(yellow_led, HIGH);
    delay(1000);
    digitalWrite(yellow_led, LOW);
    delay(1000);

  }else{
    Serial.print("its bright turn off light");
    Serial.println(ldrstatus);
    digitalWrite(green_led, HIGH);
    delay(3000);
    digitalWrite(yellow_led, HIGH);
    delay(2000);  
    digitalWrite(red_led, HIGH);
    delay(5000);    
  }
}