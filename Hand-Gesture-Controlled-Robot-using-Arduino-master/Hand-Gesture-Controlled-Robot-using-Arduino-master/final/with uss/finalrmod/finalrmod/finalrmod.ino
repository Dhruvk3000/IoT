#include <VirtualWire.h>

#define m1 2
#define m2 3
#define m3 4
#define m4 5
#define enA 6
#define enB 7
#define rfRxPin 11
#define trigPin 9
#define echoPin 10

bool motorsActivated = true; // Initialize with motors activated
unsigned long lastCommandTime = 0;
const unsigned long commandDelay = 500; // Delay between commands in milliseconds

enum MotorState {
    STOPPED,
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT
};

MotorState currentMotorState = STOPPED;

void setup() {
    vw_set_rx_pin(rfRxPin);
    vw_setup(2000);
    pinMode(m1, OUTPUT);
    pinMode(m2, OUTPUT);
    pinMode(m3, OUTPUT);
    pinMode(m4, OUTPUT);
    pinMode(enA, OUTPUT);
    pinMode(enB, OUTPUT);
    pinMode(trigPin, OUTPUT);
    pinMode(echoPin, INPUT);

    digitalWrite(enA, HIGH); // Enable motor driver A
    digitalWrite(enB, HIGH); // Enable motor driver B

    Serial.begin(9600);
    vw_rx_start();
}

void loop() {
    uint8_t buf[VW_MAX_MESSAGE_LEN];
    uint8_t buflen = VW_MAX_MESSAGE_LEN;

    if (vw_get_message(buf, &buflen)) {
        buf[buflen] = '\0'; // Null-terminate the received message
        String receivedCommand = (char *)buf;

        if (millis() - lastCommandTime >= commandDelay) {
            if (receivedCommand == "forward") {
                if (motorsActivated) {
                    currentMotorState = FORWARD;
                } else {
                    currentMotorState = STOPPED;
                }
            }
            else if (receivedCommand == "backward") {
                if (motorsActivated) {
                    currentMotorState = BACKWARD;
                } else {
                    currentMotorState = BACKWARD;
                }
            }
            else if (receivedCommand == "left") {
                if (motorsActivated) {
                    currentMotorState = LEFT;
                } else {
                    currentMotorState = STOPPED;
                }
            }
            else if (receivedCommand == "right") {
                if (motorsActivated) {
                    currentMotorState = RIGHT;
                } else {
                    currentMotorState = STOPPED;
                }
            }
            else if (receivedCommand == "stop") {
                currentMotorState = STOPPED;
            }

            lastCommandTime = millis();
        }
    }

    long distance = measureDistance();

    // If an obstacle is detected within 10cm, stop forward movement and turns
    if (distance < 10) {
        motorsActivated = false;
        Serial.println("Obstacle detected, stopping forward movement and turns.");
    } else {
        motorsActivated = true;
    }

    updateMotors();
}

void updateMotors() {
    switch (currentMotorState) {
        case STOPPED:
            stopMotors();
            break;
        case FORWARD:
            moveForward();
            break;
        case BACKWARD:
            moveBackward();
            break;
        case LEFT:
            turnLeft();
            break;
        case RIGHT:
            turnRight();
            break;
    }
}

void moveForward() {
    digitalWrite(m1, HIGH);
    digitalWrite(m2, LOW);
    digitalWrite(m3, HIGH);
    digitalWrite(m4, LOW);
}

void moveBackward() {
    digitalWrite(m1, LOW);
    digitalWrite(m2, HIGH);
    digitalWrite(m3, LOW);
    digitalWrite(m4, HIGH);
}

void turnLeft() {
    digitalWrite(m1, HIGH);
    digitalWrite(m2, LOW);
    digitalWrite(m3, LOW);
    digitalWrite(m4, LOW);
}

void turnRight() {
    digitalWrite(m1, LOW);
    digitalWrite(m2, LOW);
    digitalWrite(m3, HIGH);
    digitalWrite(m4, LOW);
}

void stopMotors() {
    digitalWrite(m1, LOW);
    digitalWrite(m2, LOW);
    digitalWrite(m3, LOW);
    digitalWrite(m4, LOW);
}

long measureDistance() {
    long duration, distance;

    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);
    duration = pulseIn(echoPin, HIGH);
    distance = (duration / 2) * 0.034;
    return distance;
}
