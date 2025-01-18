#include <VirtualWire.h>
#include <math.h>

#define x A0  // Accelerometer X-axis pin
#define y A1  // Accelerometer Y-axis pin
#define z A2  // Accelerometer Z-axis pin

char *data;

float x_val, y_val, z_val;
float pitch, roll;
float initial_pitch, initial_roll;
bool is_initialized = false;  // Flag to check if initial values are captured

void setup() 
{
  vw_set_tx_pin(12);
  vw_setup(2000);
  pinMode(x, INPUT);
  pinMode(y, INPUT);
  pinMode(z, INPUT);
  Serial.begin(9600);
}

void loop()
{
  // Read accelerometer values from analog pins
  x_val = analogRead(x);
  y_val = analogRead(y);
  z_val = analogRead(z);

  // Calculate pitch and roll based on accelerometer data
  pitch = atan2(y_val, sqrt(x_val * x_val + z_val * z_val)) * 180 / PI;
  roll = atan2(x_val, sqrt(y_val * y_val + z_val * z_val)) * 180 / PI;

  // Initialize the reference values for pitch and roll on first iteration
  if (!is_initialized) {
    initial_pitch = pitch;
    initial_roll = roll;
    is_initialized = true;
    Serial.println("Initial pitch and roll captured");
  }

  // Calculate differences from initial position
  float pitch_difference = pitch - initial_pitch;
  float roll_difference = roll - initial_roll;

  // Print pitch and roll for debugging
  Serial.print("Pitch difference: ");
  Serial.println(pitch_difference);
  Serial.print("Roll difference: ");
  Serial.println(roll_difference);

  // Define inclination threshold for movement compared to initial position
  if (pitch_difference >= 4)  // Tilt forward relative to initial position
  {
    data = "f";
    vw_send((uint8_t *)data, strlen(data));
    vw_wait_tx();
    delay(500);
    Serial.println("Forward");
  }
  else if (pitch_difference <= -4)  // Tilt backward relative to initial position
  {
    data = "b";
    vw_send((uint8_t *)data, strlen(data));
    vw_wait_tx();
    delay(500);
    Serial.println("Backward");
  }
  else if (roll_difference >= 4)  // Tilt right relative to initial position
  {
    data = "r";
    vw_send((uint8_t *)data, strlen(data));
    vw_wait_tx();
    delay(500);
    Serial.println("Right");
  }
  else if (roll_difference <= -4)  // Tilt left relative to initial position
  {
    data = "l";
    vw_send((uint8_t *)data, strlen(data));
    vw_wait_tx();
    delay(500);
    Serial.println("Left");
  }
  else  // No significant tilt, stop
  {
    data = "s";
    vw_send((uint8_t *)data, strlen(data));
    vw_wait_tx();
    delay(500);
    Serial.println("Stop");
  }
}
