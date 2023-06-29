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

typedef enum {
    STATE_INIT = 0,
    STATE_WAIT_EL_READY,
    STATE_EL_READY,
    STATE_UNPROVISIONED,
    STATE_PROVISIONING,
    STATE_PROVISIONED,
    STATE_CONNECTED,
} state_t;


void expresslink_reset()
{
    digitalWrite(RESET_PIN, LOW);
    delay(200);
    digitalWrite(RESET_PIN, HIGH);
}

state_t process_ssid(String response)
{
    int event_number = 0;
    char ok_string[3];
    char ssid_string[33] = {0};
    int total_read;

    total_read = sscanf(response.c_str(), "%s %s" , ok_string, ssid_string);

    if(total_read > 1) {
        return STATE_PROVISIONED;
    }
    else {
        return STATE_UNPROVISIONED;
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
    if (!el.begin(Serial, EVENT_PIN, WAKE_PIN, RESET_PIN, DEBUG_ENABLE)) {
      mySerial.println("Failed to initialize ExpressLink.");
      while (true) { ; }
    }
}

static state_t state = STATE_INIT;

void loop()
{
    ExpressLink::EventCode event = ExpressLink::EventCode::NONE;
    if (state != STATE_INIT) {
        ExpressLink::Event e = el.getEvent();
        event = e.code;
    }

    switch (state) {

        case STATE_INIT:
            state = STATE_WAIT_EL_READY;
            break;

        case STATE_WAIT_EL_READY:
            if (event == ExpressLink::EventCode::STARTUP) {
                state = STATE_EL_READY;
            }
            break;

        case STATE_EL_READY:
            el.config.setEndpoint(MY_AWS_IOT_ENDPOINT);
            el.config.setTopic(1, "TEST");
            state = process_ssid(el.config.getSSID());
            break;

        case STATE_UNPROVISIONED:
            if (el.cmd("CONFMODE")) {
                state = STATE_PROVISIONING;
            }
            break;

        case STATE_PROVISIONING:
            if (event == ExpressLink::EventCode::CONFMODE) {
                state = STATE_PROVISIONED;
            }
            break;

        case STATE_PROVISIONED:
            if (el.connect()) {
                state = STATE_CONNECTED;
            }
            break;

        case STATE_CONNECTED:
            if (event == ExpressLink::EventCode::CONLOST)
            {
                state = STATE_PROVISIONED;
                break;
            }
            static unsigned long last_send_time;
            if (millis() - last_send_time >= 10000)
            {
                el.publish(1, "Hello World");
                last_send_time = millis();
            }
            break;
    }
}