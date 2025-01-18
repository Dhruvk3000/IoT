#include <VirtualWire.h>

char received_data[2];  // Array to store received data (1 char + null terminator)
uint8_t message_length = sizeof(received_data);

void setup()
{
  Serial.begin(9600);        // Begin serial communication
  vw_set_rx_pin(11);         // Set pin 11 as the receiver pin
  vw_setup(2000);            // Initialize the VirtualWire library at 2000bps
  vw_rx_start();             // Start the receiver
}

void loop()
{
  if (vw_get_message((uint8_t *)received_data, &message_length))  // Check if data received
  {
    // Print received data for debugging
    Serial.print("Received: ");
    Serial.println(received_data[0]);

    // Process the received command
    switch (received_data[0])
    {
      case 'f':  // Forward
        Serial.println("Moving Forward");
        break;
      case 'b':  // Backward
        Serial.println("Moving Backward");
        break;
      case 'r':  // Right
        Serial.println("Turning Right");
        break;
      case 'l':  // Left
        Serial.println("Turning Left");
        break;
      case 's':  // Stop
        Serial.println("Stopped");
        break;
      default:
        Serial.println("Unknown command");
        break;
    }
  }
}
