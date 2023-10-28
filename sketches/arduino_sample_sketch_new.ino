#include <SoftwareSerial.h>
#include <ExpressLink.h>

#define SERIAL_RX_PIN   8
#define SERIAL_TX_PIN   9

// If a pin is not used, set its value to -1
#define EVENT_PIN 2
#define WAKE_PIN  -1
#define RESET_PIN 4

#define DEBUG_ENABLE false

#define MY_AWS_IOT_ENDPOINT "<hash>-ats.iot.us-east-1.amazonaws.com"

ExpressLink el;

void expresslink_reset()
{
    digitalWrite(RESET_PIN, LOW);
    delay(200);
    digitalWrite(RESET_PIN, HIGH);
}

bool process_ssid(String response)
{
    int event_number = 0;
    char ok_string[3];
    char ssid_string[33] = {0};
    int total_read;

    total_read = sscanf(response.c_str(), "%s %s" , ok_string, ssid_string);

    if (total_read > 1) {
        return true;
    } else {
        return false;
    }
}

void setup()
{
    SoftwareSerial mySerial(SERIAL_RX_PIN, SERIAL_TX_PIN);
    Serial.begin(ExpressLink::BAUDRATE);
    mySerial.begin(115200);
    pinMode(RESET_PIN, OUTPUT);
    while (!Serial) { ; }
    while (!mySerial) { ; }
    expresslink_reset();
    delay(5000); // Wait for ExpressLink board to restart
    int count = 0;
    while (!el.begin(Serial, EVENT_PIN, WAKE_PIN, RESET_PIN, DEBUG_ENABLE)) {
      count++;
      if (count > 2) {
        mySerial.println("Failed to initialize ExpressLink.");
        while (true) { ; }
      }
    }
    el.config.setEndpoint(MY_AWS_IOT_ENDPOINT);
    el.config.setTopic(1, "TEST");
    bool enable_conf_mode = false;
    if (!process_ssid(el.config.getSSID())) {
      el.cmd("CONFMODE");
      while (el.getEvent().code != ExpressLink::EventCode::CONFMODE) {
        delay(2000); // Wait for WiFi-provisioning
      }
    }
}

void loop()
{
    if (!el.isConnected()) {
        el.connect();
    }
    static unsigned long last_send_time;
    if (millis() - last_send_time >= 10000) {
        el.publish(1, "Hello World");
        last_send_time = millis();
    }
    delay(100);
}
