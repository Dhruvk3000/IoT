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

    // Trigger a landslide alert for any directional movement
    if (received_data[0] == 'f' || received_data[0] == 'b' || 
        received_data[0] == 'r' || received_data[0] == 'l') 
    {
      Serial.println("Landslide Alert!");
    }
    else if (received_data[0] == 's')  // Stop, no movement
    {
      Serial.println("No significant movement detected.");
    }
    else
    {
      Serial.println("Unknown command received.");
    }
  }
}
