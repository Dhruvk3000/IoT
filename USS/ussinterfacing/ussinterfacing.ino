#define trigPin 9
#define echoPin 10

void setup() {
    Serial.begin(9600);
    pinMode(trigPin, OUTPUT);
    pinMode(echoPin, INPUT);
}

void loop() {
    long duration, distance;

    // Clear the trigger pin
    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);

    // Set the trigger pin high for 10 microseconds
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);

    // Measure the duration of the pulse on the echo pin
    duration = pulseIn(echoPin, HIGH);

    // Calculate the distance based on the speed of sound (340 m/s)
    distance = (duration / 2) * 0.034;

    Serial.print("Distance: ");
    Serial.print(distance);
    Serial.println(" cm");

    delay(1000); // Delay before taking the next measurement
}
