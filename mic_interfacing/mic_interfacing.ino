#include "driver/i2s.h"

// Pin configuration
#define I2S_WS_PIN  19   // Word Select (LRCLK)
#define I2S_SCK_PIN 21   // Serial Clock (BCLK)
#define I2S_SD_PIN  18   // Serial Data (DIN)

// I2S configuration
void setupI2S() {
    i2s_config_t i2s_config = {
        .mode = (i2s_mode_t)(I2S_MODE_MASTER | I2S_MODE_RX), // Receive mode
        .sample_rate = 44100,                               // Sampling rate
        .bits_per_sample = I2S_BITS_PER_SAMPLE_16BIT,       // 16-bit samples
        .channel_format = I2S_CHANNEL_FMT_ONLY_LEFT,        // Mono
        .communication_format = I2S_COMM_FORMAT_I2S,        // I2S standard
        .intr_alloc_flags = ESP_INTR_FLAG_LEVEL1,           // Interrupt priority
        .dma_buf_count = 4,                                 // Number of DMA buffers
        .dma_buf_len = 512,                                 // Size of each DMA buffer
        .use_apll = false                                   // Use APLL clock
    };

    i2s_pin_config_t pin_config = {
        .bck_io_num = I2S_SCK_PIN,  // BCLK
        .ws_io_num = I2S_WS_PIN,    // LRCLK
        .data_out_num = I2S_PIN_NO_CHANGE, // No output
        .data_in_num = I2S_SD_PIN   // DIN
    };

    // Install and start I2S driver
    i2s_driver_install(I2S_NUM_0, &i2s_config, 0, NULL);
    i2s_set_pin(I2S_NUM_0, &pin_config);
}

void setup() {
    Serial.begin(115200);
    setupI2S();
}

void loop() {
    uint8_t audio_buffer[1024];  // Buffer to store audio samples
    size_t bytes_read;

    // Read data from I2S
    i2s_read(I2S_NUM_0, audio_buffer, sizeof(audio_buffer), &bytes_read, portMAX_DELAY);
    Serial.print("Audio data captured: ");
    Serial.println(bytes_read);
    delay(100);
}
