#include <VirtualWire.h>

#define m1 2
#define m2 3
#define m3 4
#define m4 5
#define enA 6  // Adjust this pin number as per your wiring
#define enB 7  // Adjust this pin number as per your wiring

const int trigPin = 9;   // Trig pin of the HC-SR04
const int echoPin = 10;  // Echo pin of the HC-SR04

void setup() {
    vw_set_rx_pin(11);
    vw_setup(2000);
    pinMode(m1, OUTPUT);
    pinMode(m2, OUTPUT);
    pinMode(m3, OUTPUT);
    pinMode(m4, OUTPUT);
    pinMode(enA, OUTPUT);  // Enable pin A of the motor driver
    pinMode(enB, OUTPUT);  // Enable pin B of the motor driver
    pinMode(trigPin, OUTPUT);
    pinMode(echoPin, INPUT);
    vw_rx_start();
    Serial.begin(9600);
}

void loop() {
    uint8_t buf[VW_MAX_MESSAGE_LEN];
    uint8_t buflen = VW_MAX_MESSAGE_LEN; 
 
    if (vw_get_message(buf, &buflen)) {
        if (buf[0] == 'f') {  
            moveForward();
            Serial.println("Forward");
        }  
        else if (buf[0] == 'b') {
            moveBackward();
            Serial.println("Backward");
        }
        else if (buf[0] == 'r') {
            turnLeft();
            Serial.println("Left");
        }
        else if (buf[0] == 'l') {
            turnRight();
            Serial.println("Right");
        }
        else if (buf[0] == 's') {
            stop();
            Serial.println("Stop");
        }
    }
    
    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);

    unsigned long duration = pulseIn(echoPin, HIGH);
    // Speed of sound = 343 m/s or 0.0343 cm/microsecond
    // Divide by 2 to account for the time taken to travel to the object and back
    float distance = duration * 0.0343 / 2;

    if (distance < 5) {
        stop();
        Serial.println("Obstacle detected, stopping.");
    }
    
    delay(500); // Add a delay before the next measurement
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

void stop() {
    digitalWrite(m1, LOW);
    digitalWrite(m2, LOW);
    digitalWrite(m3, LOW);
    digitalWrite(m4, LOW);
}
