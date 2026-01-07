#include "display.h"
#include <GxEPD2_BW.h>
#include <Fonts/FreeSansBold12pt7b.h>
#include <Fonts/FreeSansBold18pt7b.h>

// --- Pin Definitions for Arduino Uno R4 WiFi ---
// BUSY: 7, RST: 6, DC: 5, CS: 10, SCK: 13, MOSI: 11
GxEPD2_BW<GxEPD2_154_D67, GxEPD2_154_D67::HEIGHT> display(GxEPD2_154_D67(/*CS=*/10, /*DC=*/5, /*RST=*/6, /*BUSY=*/7));

void initDisplay() {
  display.init(115200);
  display.setRotation(1); // 0 or 1 depending on orientation preference
  display.setTextColor(GxEPD_BLACK);
  
  // Initial Clear
  display.setFullWindow();
  display.firstPage();
  do {
    display.fillScreen(GxEPD_WHITE);
    display.setFont(&FreeSansBold12pt7b);
    display.setCursor(20, 30);
    display.print("Initializing...");
  } while (display.nextPage());
}

void drawSwitch(bool state) {
  int centerX = 100;
  int centerY = 100;
  
  int switchWidth = 120;
  int switchHeight = 60;
  int radius = switchHeight / 2;
  
  // Draw Track (Rounded Rectangle / Capsule)
  // Drawn as a filled rounded rect
  if (state) {
      // ON State Track - Fill Black (or maybe simplify to Outline for 'Off' and Fill for 'On'?)
      // Let's stick to mimicking the CSS: 
      // CSS: gray when off, blue (active) when on. 
      // B/W E-Paper: we can use Black for "Active" and outline for "Inactive".
      display.fillRoundRect(centerX - switchWidth/2, centerY - switchHeight/2, switchWidth, switchHeight, radius, GxEPD_BLACK);
  } else {
      // OFF State Track - Outline
      display.drawRoundRect(centerX - switchWidth/2, centerY - switchHeight/2, switchWidth, switchHeight, radius, GxEPD_BLACK);
      // Also clear inside to be sure? (White)
      // Since we clear screen at start of page loop, it is white.
  }

  // Draw Knob (Circle)
  int knobRadius = radius - 4;
  int knobX;
  
  if (state) {
      // Right side
      knobX = centerX + switchWidth/2 - radius; 
      // Knob is White on Black background
      display.fillCircle(knobX, centerY, knobRadius, GxEPD_WHITE);
  } else {
      // Left side
      knobX = centerX - switchWidth/2 + radius;
      // Knob is Black (filled) or Outline?
      // Let's make the knob solid Black on White background
      display.fillCircle(knobX, centerY, knobRadius, GxEPD_BLACK);
  }
}

void updateDisplay(bool ledState) {
  // Partial update if supported, or full update. 
  // GxEPD2_154_D67 supports partial updates usually.
  display.setFullWindow();
  
  display.firstPage();
  do {
    display.fillScreen(GxEPD_WHITE);
    
    // Header
    display.setFont(&FreeSansBold12pt7b);
    int16_t tbx, tby; uint16_t tbw, tbh;
    display.getTextBounds("LED Control", 0, 0, &tbx, &tby, &tbw, &tbh);
    display.setCursor((200 - tbw) / 2, 30);
    display.print("LED Control");
    
    // Switch Graphic
    drawSwitch(ledState);
    
    // Status Text
    display.setFont(&FreeSansBold18pt7b);
    const char* statusText = ledState ? "ON" : "OFF";
    display.getTextBounds(statusText, 0, 0, &tbx, &tby, &tbw, &tbh);
    display.setCursor((200 - tbw) / 2, 180);
    display.print(statusText);
    
  } while (display.nextPage());
}
