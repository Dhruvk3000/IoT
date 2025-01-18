// Motor A connections
int enA = 6;  // Enable for Motor A
int in1 = 2;
int in2 = 3;

// Motor B connections
int enB = 7;  // Enable for Motor B
int in3 = 4;
int in4 = 5;

void setup() {
  // Set motor control pins to outputs
  pinMode(enA, OUTPUT);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(enB, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);

  // Set motor enable pins to LOW (motors on)
  digitalWrite(enA, HIGH);
  digitalWrite(enB, HIGH);

  Serial.begin(9600);
}

void loop() {
  // Forward
  Serial.println("Forward");
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);

  // Enable motors
  digitalWrite(enA, HIGH);
  digitalWrite(enB, HIGH);

  delay(3000); // Run forward for 3 seconds

  // Backward
  Serial.println("Backward");
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);
  digitalWrite(in3, LOW);
  digitalWrite(in4, HIGH);

  delay(3000); // Run backward for 3 seconds

  // Stop
  Serial.println("Stop");
  digitalWrite(enA, LOW); // Motors off
  digitalWrite(enB, LOW);

  delay(1000); // Pause for 1 second before next loop
}
