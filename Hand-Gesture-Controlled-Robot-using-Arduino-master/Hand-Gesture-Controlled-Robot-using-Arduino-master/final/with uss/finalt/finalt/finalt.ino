#include <VirtualWire.h>

#define rfTxPin 12
#define x A0
#define y A1

int x_val2;
int y_val2;

void setup() {
    vw_set_tx_pin(rfTxPin);
    vw_setup(2000);
    pinMode(x, INPUT);
    pinMode(y, INPUT);
    Serial.begin(9600);
    x_val2 = analogRead(x);
    y_val2 = analogRead(y);
    Serial.println("ADXL-based movement control.");
}

void loop() {
    int x_val = analogRead(x);
    int y_val = analogRead(y);

    int x_axis = x_val - x_val2;
    int y_axis = y_val - y_val2;

    if (y_axis <= -40) {
        sendData("forward");
        Serial.println("Sending 'forward' command.");
    }
    else if (y_axis >= 40) {
        sendData("backward");
        Serial.println("Sending 'backward' command.");
    }
    else if (x_axis >= 40) {
        sendData("right");
        Serial.println("Sending 'right' command.");
    }
    else if (x_axis <= -40) {
        sendData("left");
        Serial.println("Sending 'left' command.");
    }
    else {
        sendData("stop");
        Serial.println("Sending 'stop' command.");
    }

    delay(500); // Delay for 0.5 seconds
}

void sendData(const char *data) {
    vw_send((uint8_t *)data, strlen(data));
    vw_wait_tx();
}
