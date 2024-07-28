// Demo based on:
// UTFT_Demo_320x240 by Henning Karlsen
// web: http://www.henningkarlsen.com/electronics
//
/*

 This sketch uses the GLCD and font 2 only.
 
 Make sure all the display driver and pin connections are correct by
 editing the User_Setup.h file in the TFT_eSPI library folder.

 #########################################################################
 ###### DON'T FORGET TO UPDATE THE User_Setup.h FILE IN THE LIBRARY ######
 #########################################################################
 */
#include "qrcodec.h"
#include "SPI.h"
#include "TFT_eSPI.h"
#define TFT_GREY 0x7BEF
#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebSrv.h>

const char* PARAM_MESSAGE = "qrcodeStr";
const char* ssid = "Xiaomi_C208";
const char* password = "9647463..";
AsyncWebServer server(80);
int led_pin = 2;
// WiFiServer server(80);
TFT_eSPI myGLCD = TFT_eSPI();  // Invoke custom library

unsigned long runTime = 0;
String ip;
String qrcodeStr;

void notFound(AsyncWebServerRequest *request) {
    request->send(404, "text/plain", "Not found");
}

void setup() {
  Serial.begin(115200);
  pinMode(led_pin, OUTPUT);  // set the LED pin mode
  delay(10);

  // We start by connecting to a WiFi network

  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  // Serial.println(WiFi.localIP().toString().c_str());
  ip = WiFi.localIP().toString();
  // qrcodeStr = "https://zhimin-dev.github.io/#/epark?&data=VTJGc2RHVmtYMS9TdDhBanEzYjU1cnRKcERDYlZ4c2FVdGxEOElzZ1pmNlZSbFVFS2lvY1JScm5XTVhLNXM1cw==";
  drawLed();

  // server.begin();
  webServer();
}

void drawLed() {
  Serial.println(ip);
  
  Serial.println(qrcodeStr);
  randomSeed(analogRead(A0));
    // Setup the LCD

    myGLCD.init();
    myGLCD.setRotation(1);

    myGLCD.fillScreen(TFT_BLACK);

    // Clear the screen and draw the frame
    myGLCD.fillScreen(TFT_BLACK);
    myGLCD.setTextColor(TFT_WHITE, TFT_BLACK);
  if(qrcodeStr != "") {
    // gen the QR code
    QRCode qrcode;
    uint8_t qrcodeData[qrcode_getBufferSize(15)];
    Serial.println("------------2222-----");
    qrcode_initText(&qrcode, qrcodeData, 15, ECC_LOW, qrcodeStr.c_str());

    int dp = 2;

    uint8_t x0 = (myGLCD.width() - qrcode.size * dp) / 2;
    Serial.println(x0);
    Serial.println("------------1111-----");
    uint8_t y0 = 0;

    // get QR code pixels in a loop
    for (uint8_t y = 0; y < qrcode.size; y++) {
      for (uint8_t x = 0; x < qrcode.size; x++) {
        int color = TFT_BLACK;
        if (qrcode_getModule(&qrcode, x, y)) {
          color = TFT_WHITE;
        }
        for (uint8_t scale_y = 0; scale_y < dp; scale_y++) {
          uint8_t dx = x0 + x * dp;
          uint8_t dy = y0 + y * dp;
          for (uint8_t scale_x = 0; scale_x < dp; scale_x++) {
            myGLCD.drawPixel(dx + scale_x, dy + scale_y, color);
          }
        }
      }
    }
    myGLCD.drawCentreString("Get Phone Number", 120, 180, 2);
  }else{
    myGLCD.drawCentreString("go to setting", 120, 180, 2);
  }
  myGLCD.drawCentreString(ip, 120, 210, 2);
  myGLCD.setTextDatum(TC_DATUM);
}

void webServer() {
    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
        request->send(200, "text/html", "<html><head><meta data-n-head='ssr' charset='utf-8'><meta data-n-head='ssr' name='viewport' content='width=device-width, initial-scale=1'></head><body><form action='/save'><textarea name='qrcodeStr' placeholder='please input your qrcode link'></textarea><button onclick='submit'>submit</button></form></body></html>");
    });

    // Send a GET request to <IP>/get?message=<message>
    server.on("/save", HTTP_GET, [] (AsyncWebServerRequest *request) {
        String message;
        if (request->hasParam(PARAM_MESSAGE)) {
            message= request->getParam(PARAM_MESSAGE)->value();
            qrcodeStr = message;
            drawLed();
        } else {
            message = "No message sent";
        }
        request->send(200, "text/plain", "save value: " + message);
    });

    // Send a POST request to <IP>/post with a form field message set to <message>
    // server.on("/post", HTTP_POST, [](AsyncWebServerRequest *request){
    //     String message;
    //     if (request->hasParam(PARAM_MESSAGE, true)) {
    //         message = request->getParam(PARAM_MESSAGE, true)->value();
    //     } else {
    //         message = "No message sent";
    //     }
    //     request->send(200, "text/plain", "Hello, POST: " + message);
    // });

    server.onNotFound(notFound);

    server.begin();
}

void loop() {
  // WiFiClient client = server.available();  // listen for incoming clients

  // if (client) {                     // if you get a client,
  //   Serial.println("New Client.");  // print a message out the serial port
  //   String currentLine = "";        // make a String to hold incoming data from the client
  //   while (client.connected()) {    // loop while the client's connected
  //     if (client.available()) {     // if there's bytes to read from the client,
  //       char c = client.read();     // read a byte, then
  //       Serial.write(c);            // print it out the serial monitor
  //       if (c == '\n') {            // if the byte is a newline character

  //         // if the current line is blank, you got two newline characters in a row.
  //         // that's the end of the client HTTP request, so send a response:
  //         if (currentLine.length() == 0) {
  //           // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
  //           // and a content-type so the client knows what's coming, then a blank line:
  //           client.println("HTTP/1.1 200 OK");
  //           client.println("Content-type:text/html");
  //           client.println();

  //           // the content of the HTTP response follows the header:
  //           client.print("<html><head><meta data-n-head='ssr' charset='utf-8'><meta data-n-head='ssr' name='viewport' content='width=device-width, initial-scale=1'></head><body><form action='/save'><textarea name='qrcodeStr' placeholder='please input your qrcode link'></textarea><button onclick='submit'>submit</button></form></body></html>");

  //           // The HTTP response ends with another blank line:
  //           client.println();
  //           // break out of the while loop:
  //           break;
  //         } else {  // if you got a newline, then clear currentLine:
  //           currentLine = "";
  //         }
  //       } else if (c != '\r') {  // if you got anything else but a carriage return character,
  //         currentLine += c;      // add it to the end of the currentLine
  //       }
  //       // Check to see if the client request was "GET /H" or "GET /L":
  //       if (currentLine.endsWith("GET /save")) {
  //         Serial.println(currentLine);
  //         Serial.println("---ssss\n");
  //       }
  //       // if (currentLine.endsWith("GET /L")) {
  //       //   digitalWrite(led_pin, LOW);  // GET /L turns the LED off
  //       //   Serial.println("\n led close.");
  //       // }
  //     }
  //   }
  //   // close the connection:
  //   client.stop();
  //   Serial.println("Client Disconnected.");
  // }
}
