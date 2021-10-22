#include <SoftwareSerial.h>

SoftwareSerial mySerial(8, 9); // RX, TX

#define EVENT_PIN 2
#define RESET_PIN 4

#define MY_AWS_IOT_ENDPOINT "<hash>-ats.iot.<region>.amazonaws.com"

typedef enum event_t {
    EVENT_NONE = 0,
    EVENT_STARTUP = 2,
    EVENT_CONLOST,
    EVENT_OVERRUN,
    EVENT_OTA,
    EVENT_SHADOW,
    EVENT_CONFMODE,
} event_t;


typedef enum {
    STATE_INIT = 0,
    STATE_WAIT_EL_READY,
    STATE_EL_READY,
    STATE_UNPROVISIONED,
    STATE_PROVISIONING,
    STATE_PROVISIONED,
    STATE_CONNECTED,
} state_t;

static state_t state;
static unsigned long got_connected;

String execute_command(String command)
{
    Serial.print(command);
    String response = Serial.readString();
    return response;
}

void expresslink_reset()
{
    digitalWrite(RESET_PIN, LOW);
    delay(200);
    digitalWrite(RESET_PIN, HIGH);
}

event_t process_event()
{
    int val = 0;
    event_t event_number = EVENT_NONE;
    String response;
    val = digitalRead(EVENT_PIN);
    if(val)
    {
        response = execute_command("AT+EVENT?\n");
        if (response.equals("OK\n"))
        {
            return EVENT_NONE;
        }
        else {
            char ok_string[3];
            int topic_index;
            int total_read;

            total_read = sscanf(response.c_str(), "%s %d %d %*s" , ok_string, &event_number, &topic_index);

                        
            return event_number;
        }
    }
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
    else
        return STATE_UNPROVISIONED;
}

void setup()
{
    state = STATE_INIT;
    Serial.begin(115200);
    mySerial.begin(115200);
    pinMode(EVENT_PIN, INPUT);
    pinMode(RESET_PIN, OUTPUT);
    while (!Serial) {
        ;
    }
    while (!mySerial) {
        ;
    }
    got_connected = 0;
}

void loop()
{
    event_t event = EVENT_NONE;
    String response;

    if (state != STATE_INIT)
    {
        event = process_event();
    }

    switch (state) {

        case STATE_INIT:
        expresslink_reset();
        state = STATE_WAIT_EL_READY;
        break;

        case STATE_WAIT_EL_READY:
        if (event == EVENT_STARTUP) {
            state = STATE_EL_READY;
        }
        break;

        case STATE_EL_READY:
        response = execute_command("AT+CONF EndPoint="MY_AWS_IOT_ENDPOINT"\n");
        response = execute_command("AT+CONF Topic1=TEST\n");
        response = execute_command("AT+CONF? SSID\n");
        state = process_ssid(response);

        break;

        case STATE_UNPROVISIONED:
        response = execute_command("AT+CONFMODE\n");
        if (response.equals("OK CONFMODE ENABLED\n"))
        {
            state = STATE_PROVISIONING;
        }
        break;

        case STATE_PROVISIONING:
        if (event == EVENT_CONFMODE) {
            state = STATE_PROVISIONED;
        }
        break;

        case STATE_PROVISIONED:
        response = execute_command("AT+CONNECT\n");
        if (response.equals("OK CONNECTED\n")) {
            state = STATE_CONNECTED;
        }

        case STATE_CONNECTED:
        if (event == EVENT_CONLOST)
        {
            state = STATE_PROVISIONED;
        }
        if (millis() - got_connected >= 10000)
        {
            response = execute_command("AT+SEND1 Hello World\n");
            got_connected = millis();
        }
        break;
    }
}
