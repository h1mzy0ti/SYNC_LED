/* Led Music Visualizer With multiple modes
 Features - Wifi connectivity, dedicated app
         Author - H1mzy0t1
                                           */
#include <FastLED.h>                                           
#include <ESP8266WiFi.h>

#define FASTLED_FORCE_SOFTWARE_SPI
#define FASTLED_FORCE_SOFTWARE_PINS

#define LED_PIN  4                                      // W2812B DIN, GPIO - D2
#define sensor_pin 5                                    // Sensor PIN, GPIO5 - D1
#define ini_freq 250                                    // Initial Value
#define NUM_LEDS 61                                     // Numbers of Leds in my strip
#define BRIGHTNESS 64                                   // Brightless of the stip

const char* ssid = "Sync Led v0.1.0";                   // AP SSID
const char* password = "SyncrocksSyncr0ck5@!";          // AP password

// For static IP
IPAddress local_ip(192,168,2,1);
IPAddress gateway(192,168,2,1);
IPAddress subnet(255,255,255,0);

WiFiServer server(80);

// web server stuff
unsigned long currentTime = millis();
unsigned long previousTime = 0; 
const long timeoutTime = 2000;

int i;
CRGB leds[NUM_LEDS];

void setup(){
    Serial.begin(9600);
    FastLED.addLeds<WS2812B, LED_PIN , GRB>(leds, NUM_LEDS);
    FastLED.setMaxPowerInVoltsAndMilliamps(5, 500);
    FastLED.setBrightness(BRIGHTNESS);
    FastLED.clear();
    FastLED.show();

    IPAddress myIP = WiFi.softAPIP();
    WiFi.softAPConfig(local_ip, gateway, subnet);
    delay(100);
    boolean result = WiFi.softAP(ssid, password);

    Serial.println("<---- Debugger SL022 Started ---->");
    Serial.println("Web server IP Address is : ");
    Serial.println("192.168.2.1");
    server.begin(); 
}
enum HTMLColorCode {
    Cyan = 0x00FFFF
};

void LED_val(int r, int g, int b){          // Color Value function
    CRGB color = CRGB(r, g, b);
    fill_solid(leds, NUM_LEDS, color);
    FastLED.show();
}

void LED_OFF(){
    for(i=1; i<NUM_LEDS; i++){
        leds[i] = CRGB::Black;
    }
    FastLED.show();
}
void red(){                                 //This will turn on Red colour
    for (i=1; i<NUM_LEDS;i++){
        leds[i] = CRGB::Red;
    } 
    FastLED.show();                             
}

void blue(){                               //This will turn on blue colour
    for (i=1; i<NUM_LEDS;i++){
        leds[i] = CRGB::Blue;
    }
    FastLED.show();
}
void green(){                              //This will turn on green colour
    for (i=1; i<NUM_LEDS;i++){
        leds[i] = CRGB::Green;
    }
    FastLED.show();
}
void cyan(){
    for (i=1; i<NUM_LEDS;i++){
    leds[i] = CRGB::Cyan;
    }
    FastLED.show();
}
void purple(){
    LED_val(23,23,23);
}

void loop(){
    int v;
    String header;
    String os_red = "off";
    String pickervalue = "val";
    String os_blue = "off";
    String os_green = "off";
    String os_cyan = "off";
    String os_purple = "off";
    String trip_mode = "off";
    String peace_mode = "off";
    String timer_mode = "off";
    String meteorRain = "off";
    String os_smooth_effect = "off";

    WiFiClient Client = server.available();
    if (Client) {
        Serial.println("Client connected .... [+]");
        String currentLine = "";           
        currentTime = millis();
        previousTime = currentTime;
        while (Client.connected() && currentTime - previousTime <= timeoutTime) {
            currentTime = millis();
            if (Client.available()) {
                char c = Client.read();             
                Serial.write(c);                    
                header += c;
                if (c == '\n') {
                    if (currentLine.length() == 0) {
                        Client.println("HTTP/1.1 200 OK");
                        Client.println("Content-type:text/html");
                        Client.println("Connection: close");
                        Client.println();
                
                        if (header.indexOf("GET /red/on") >=0 ) {                        //Client requests
                            os_red = "on";
                            delay(1000);
                            red();
                            Serial.println("RED INIT");
                        }
                        else if (header.indexOf("GET /red/off") >=0 ) {
                            os_red = "off";
                            delay(1000);
                            LED_OFF();
                            Serial.println("RED OFF"); 
                        }
                        else if (header.indexOf("GET /green/on") >=0 ){
                            os_green ="on";
                            delay(1000);
                            green();
                            Serial.println("GREEN INIT");
                        }
                        else if (header.indexOf("GET /green/off") >=0 ) {
                            os_green = "off";
                            delay(1000);
                            LED_OFF();
                            Serial.println("GREEN OFF");
                        }
                        else if (header.indexOf("GET /blue/on") >=0 ){
                            os_blue = "on";
                            delay(1000);
                            blue();
                            Serial.println("BLUE INIT");
                        }
                        else if (header.indexOf("GET /blue/off") >=0 ) {
                            os_blue = "off";
                            delay(1000);
                            LED_OFF();
                            Serial.println("BLUE OFF");
                        }
                        else if (header.indexOf("GET /cyan/on") >=0 ){
                            os_cyan ="on";
                            delay(1000);
                            cyan();
                            Serial.println("CYAN INIT");
                        }
                        else if (header.indexOf("GET /cyan/off") >=0 ){
                            os_cyan = "off";
                            delay(1000);
                            LED_OFF();
                            Serial.println("CYAN OFF");
                        }
                        else if (header.indexOf("GET /purple/on") >=0 ){
                            os_purple ="on";
                            delay(1000);
                            purple();
                            Serial.println("PURPLE INIT");
                        }
                        else if (header.indexOf("GET /purple/off") >=0 ){
                            os_purple = "off";
                            delay(1000);
                            LED_OFF();
                            Serial.println("PURPLE OFF");
                        }
                        else if (header.indexOf("GET /pickervalue")>=0){
                            delay(1000);
                            pickervalue = "val";
                            Serial.println("Recieving Picker value");
                            Serial.println(header.indexOf("GET /pickervalue"));
                        }
                    
                        Client.println("<!DOCTYPE html><html>");
                        Client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
                        Client.println("<link rel=\"icon\" href=\"data:,\">");

                        Client.println("<style>html { font-family: OCR A Std; background-color: rgba(16, 17, 17, 0.966); display: inline-block; margin: 0px auto; text-align: center;}");
                        Client.println(".button2 {background-color: #77878A;}</style></head>");

                        Client.println("<body style=\"color:rgb(236, 234, 232)\"><h1>SYNC LED</h1>");
                        Client.println("<p1>Led Music Visualizer</p1><br>");
                        Client.println("<p2>Your device is connected successfully &#10004; </p2>");
                        Client.println("<p3><br>Please select </p3><br>");

                        if (pickervalue=="val"){
                            Client.println("<p><a href=\"/pickervalue/val\"<label for=\"colorpicker\">Color Picker:</label><input type=\"color\" id=\"colorpicker\" value=\"#0000ff\"></p>");
                        }
                        if (os_red=="off") {
                            Client.println("<p><a href=\"/red/on\"><button style=\"background-color: #FF0000; padding: 15px 32px; display: inline-block; font-size: 15px;\" class=\"button buttonred\">Red &#9675;</button></a></p>");
                        } 
                        else {
                            Client.println("<p><a href=\"/red/off\"><button style=\"background-color: #FFFFFF; padding: 15px 32px; display: inline-block; font-size: 15px;\" class=\"button button2\">Red - ON<br></button></a></p>");
                        }
                        if (os_green=="off"){
                            Client.println("<p><a href=\"/green/on\"><button style=\"background-color: #00FF00; padding: 15px 32px; display: inline-block; font-size: 15px;\" class=\"button buttongreen\">Green &#9675;</button></a></p>");
                        }
                        else {
                            Client.println("<p><a href=\"/green/off\"><button style=\"background-color: #FFFFFF; padding: 15px 32px; display: inline-block; font-size: 15px;\" class=\"button button2\">Green - ON<br></button></a></p>");
                        }
                        if (os_blue=="off"){
                            Client.println("<p><a href=\"/blue/on\"><button style=\"background-color: #0000FF; padding: 15px 32px; display: inline-block; font-size: 15px;\" class=\"button buttonblue\">Blue &#9675;</button></a></p>");
                        }
                        else {
                            Client.println("<p><a href=\"/blue/off\"><button style=\"background-color: #FFFFFF; padding: 15px 32px; display: inline-block; font-size: 15px;\" class=\"button button2\">Blue - ON<br></button></a></p>");

                        }
                        if (os_cyan=="off"){
                            Client.println("<p><a href=\"/cyan/on\"><button style=\"background-color: #00FFFF; padding: 15px 32px; display: inline-block; font-size: 15px;\" class=\"button buttonblue\">Cyan &#9675;</button></a></p>");
                        }
                        else {
                            Client.println("<p><a href=\"/cyan/off\"><button style=\"background-color: #FFFFFF; padding: 15px 32px; display: inline-block; font-size: 15px;\" class=\"button button2\">Cyan - ON<br></button></a></p>");
                        }
                        if (os_purple=="off"){
                            Client.println("<p><a href=\"/purple/on\"><button style=\"background-color: #800080; padding: 15px 32px; display: inline-block; font-size: 15px;\" class=\"button buttonblue\">Purple &#9675;</button></a></p>");
                        }
                        else {
                            Client.println("<p><a href=\"/purle/off\"><button style=\"background-color: #FFFFFF; padding: 15px 32px; display: inline-block; font-size: 15px;\" class=\"button button2\">Purple - ON<br></button></a></p>");
                        }

                        Client.println("<footer><p>Made with &hearts; by H1mzy0t1</p></footer>");
                        Client.println("</body></html>");
                        break ;
                    }
                    else {
                        currentLine = "";
                    }
                }
                else if (c != '\r') {
                    currentLine += c;
                }
            }
        }
        header = "";
        Client.stop();
        Serial.println("Command recieved...");
        Serial.println("Disconnected");
    }
}