#include <Arduino.h>
#include <WiFiS3.h>
#include <ArduinoBLE.h>
#include "web_assets.h"
#include "display.h"
#include "arduino_secrets.h" 

// --- Configuration ---
// Create a file "arduino_secrets.h" in src/ folder with:
// #define SECRET_SSID "YourWiFiName"
// #define SECRET_PASS "YourWiFiPassword"

char ssid[] = SECRET_SSID;
char pass[] = SECRET_PASS;

int ledPin = 4;
int status = WL_IDLE_STATUS;
bool currentLedState = false;
volatile bool displayNeedsUpdate = false;

// --- Helper ---
void setLedState(bool on);

// --- WiFi Server Setup ---
WiFiServer server(80);

// --- BLE Setup ---
BLEService ledService("19B10000-E8F2-537E-4F6C-D104768A1214"); 
// Create a read/write characteristic
BLEByteCharacteristic switchCharacteristic("19B10001-E8F2-537E-4F6C-D104768A1214", BLERead | BLEWrite);

void handleWiFiClients();

void setup() {
  Serial.begin(115200);
  while (!Serial); // Wait for Serial Monitor

  pinMode(ledPin, OUTPUT);

  // 1. Initialize WiFi
  Serial.print("Connecting to WiFi...");
  while (status != WL_CONNECTED) {
    Serial.print(".");
    status = WiFi.begin(ssid, pass);
    delay(1000);
  }
  Serial.println("\nConnected to WiFi!");
  server.begin();
  
  IPAddress ip = WiFi.localIP();
  Serial.print("HTTP Server running at: http://");
  Serial.println(ip);

  // 2. Initialize BLE
  if (!BLE.begin()) {
    Serial.println("starting BLE failed!");
    while (1);
  }

  BLE.setLocalName("UnoR4_IoT");
  BLE.setAdvertisedService(ledService);
  ledService.addCharacteristic(switchCharacteristic);
  BLE.addService(ledService);

  switchCharacteristic.writeValue(0); // Default value
  
  // 3. Initialize Display
  initDisplay();
  updateDisplay(false); // Initial State

  BLE.advertise();
  Serial.println("BLE Active. Waiting for connections...");
}

void setLedState(bool on) {
  if (on != currentLedState) {
      currentLedState = on;
      digitalWrite(ledPin, on ? HIGH : LOW);
      switchCharacteristic.writeValue(on ? 1 : 0);
      Serial.print("State Changed to: ");
      Serial.println(on ? "ON" : "OFF");
      Serial.println(on ? "ON" : "OFF");
      displayNeedsUpdate = true;
  }
}

void loop() {
  // --- PART 1: Handle BLE ---
  BLEDevice central = BLE.central();
  
  if (central) {
    if (central.connected()) {
      // Handle BLE writes
      // Handle BLE writes
      if (switchCharacteristic.written()) {
        bool val = switchCharacteristic.value() != 0;
        setLedState(val);
      }
    }
  }

  // --- PART 2: Handle WiFi (Always runs) ---
  handleWiFiClients();
  
  // --- PART 3: Handle Display Updates (Deferred) ---
  if (displayNeedsUpdate) {
      updateDisplay(currentLedState);
      displayNeedsUpdate = false;
  }
}

void handleWiFiClients() {
  WiFiClient client = server.available();
  if (client) {
    Serial.println("New WiFi Client");
    String currentLine = "";   
    String requestLine = "";   // Store the first line of the request
    boolean isFirstLine = true;

    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        if (c == '\n') {
          if (currentLine.length() == 0) {
            // Headers finished, now check what was requested to send correct response
            
            // Default to HTML unless CSS was requested
            if (requestLine.indexOf("GET /style.css") >= 0) {
              client.println("HTTP/1.1 200 OK");
              client.println("Content-type: text/css");
              client.println("Connection: close");
              client.println();
              client.println(css_content);
            } 
            else if (requestLine.indexOf("GET / ") >= 0 || requestLine.indexOf("GET /index.html") >= 0) {
              client.println("HTTP/1.1 200 OK");
              client.println("Content-type: text/html");
              client.println("Connection: close");
              client.println();
              
              // Serve HTML parts and inject state
              client.print(html_head);
              if (currentLedState) client.print(" checked");
              client.print(html_tail);
            }
            // If it was an API call (GET /led/on or off) we typically don't return content, 
            // or just a small OK. The browser fetch ignores the body usually, but let's be polite.
            else if (requestLine.indexOf("GET /led") >= 0) {
                client.println("HTTP/1.1 200 OK");
                client.println("Content-type: text/plain");
                client.println("Connection: close");
                client.println();
                client.println("OK");
            }

            break;
          } else {
            // End of a line (not empty)
            if (isFirstLine) {
                requestLine = currentLine;
                isFirstLine = false;
                
                // Check command immediately after first line for side effects
                // Check command immediately after first line for side effects
                if (requestLine.indexOf("GET /led/on") >= 0) {
                  setLedState(true);
                } else if (requestLine.indexOf("GET /led/off") >= 0) {
                  setLedState(false);
                }
            }
            currentLine = "";
          }
        } else if (c != '\r') {
          currentLine += c;
        }
      }
    }
    client.stop();
    Serial.println("WiFi Client Disconnected");
  }
}
