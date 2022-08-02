#include <SoftwareSerial.h>

SoftwareSerial mySerial(8, 9); // RX, TX

#define EVENT_PIN 2
#define RESET_PIN 4

#define MY_AWS_IOT_ENDPOINT "<hash>-ats.iot.<region>.amazonaws.com"

#define HOST_RESPONSE_BUFFER_SIZE 500

char buffer[HOST_RESPONSE_BUFFER_SIZE];

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
static unsigned long saved_timeout_value_ms;

String execute_command(String command, unsigned long timeout_ms)
{
    saved_timeout_value_ms = Serial.getTimeout();
    Serial.setTimeout(timeout_ms);
    memset(buffer, 0, HOST_RESPONSE_BUFFER_SIZE);
    Serial.println(command);
    Serial.readBytesUntil('\n', buffer, HOST_RESPONSE_BUFFER_SIZE);
    String s(buffer);
    s.trim();
    Serial.setTimeout(saved_timeout_value_ms);
    return s;
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
        response = execute_command("AT+EVENT?", 3000);
        if (response.equals("OK"))
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
    else {
        return STATE_UNPROVISIONED;
    }
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
    saved_timeout_value_ms = 0;
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
        response = execute_command("AT+CONF Endpoint="MY_AWS_IOT_ENDPOINT"", 3000);
        response = execute_command("AT+CONF Topic1=TEST", 3000);
        response = execute_command("AT+CONF? SSID", 3000);
        state = process_ssid(response);

        break;

        case STATE_UNPROVISIONED:
        response = execute_command("AT+CONFMODE", 5000);
        if (response.equals("OK CONFMODE ENABLED"))
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
        response = execute_command("AT+CONNECT", 30000);
        if (response.equals("OK 1 CONNECTED")) {
            state = STATE_CONNECTED;
        }
        break;

        case STATE_CONNECTED:
        if (event == EVENT_CONLOST)
        {
            state = STATE_PROVISIONED;
        }
        if (millis() - got_connected >= 10000)
        {
            response = execute_command("AT+SEND1 Hello World", 5000);
            got_connected = millis();
        }
        break;
    }
}