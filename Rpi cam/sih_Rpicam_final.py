import numpy as np
import RPi.GPIO as GPIO
import time
import os
from itertools import combinations
from pymongo import MongoClient

# Define GPIO pins for servos
PAN_SERVO_PIN = 18
TILT_SERVO_PIN = 19

# Speed of sound in air (m/s)
SPEED_OF_SOUND = 343  # m/s

# Servo setup
def setup_servos():
    GPIO.setmode(GPIO.BCM)
    GPIO.setup(PAN_SERVO_PIN, GPIO.OUT)
    GPIO.setup(TILT_SERVO_PIN, GPIO.OUT)
    global pan_pwm, tilt_pwm
    pan_pwm = GPIO.PWM(PAN_SERVO_PIN, 50)  # 50 Hz
    tilt_pwm = GPIO.PWM(TILT_SERVO_PIN, 50)
    pan_pwm.start(7.5)  # Initial position
    tilt_pwm.start(7.5)

def move_servo(pwm, angle):
    if 0 <= angle <= 180:
        duty_cycle = 2.5 + (angle / 180) * 10  # Map angle to duty cycle
        pwm.ChangeDutyCycle(duty_cycle)
        time.sleep(0.3)  # Allow servo to reach position
    else:
        print(f"Invalid angle: {angle}. Must be between 0 and 180 degrees.")

# Simulated GCC-PHAT for TDOA estimation
def gcc_phat(x, y, fs=1, max_tau=None):
    N = len(x) + len(y) - 1
    X = np.fft.fft(x, N)
    Y = np.fft.fft(y, N)
    R = X * np.conj(Y)  # Cross-power spectrum
    R = R / np.abs(R)  # PHAT weighting
    r = np.fft.ifft(R)  # Cross-correlation
    r = np.real(r)
    max_shift = int(N / 2)
    if max_tau:
        max_shift = min(int(fs * max_tau), max_shift)
    delay = np.argmax(r) - len(x) + 1
    delay = np.clip(delay, -max_shift, max_shift)
    return delay / fs, r

# Triangulation for azimuth and elevation calculation
def triangulate(tdoas, mic_positions, c=SPEED_OF_SOUND):
    azimuth = 90 + np.random.uniform(-10, 10)  # Simulated azimuth in degrees
    elevation = 60 + np.random.uniform(-10, 10)  # Simulated elevation in degrees
    range_ = np.linalg.norm(tdoas * c)  # Calculate range
    return azimuth, elevation, range_

# MongoDB connection
def connect_to_mongo():
    try:
        client = MongoClient("mongodb+srv://faiz:1234@gilda.z2wkb.mongodb.net/")
        db = client['test']  # Replace with your database name
        collection = db['localizations']  # Replace with your collection name
        print("Connected to MongoDB successfully!")
        return collection
    except Exception as e:
        print(f"Error connecting to MongoDB: {e}")
        return None

# Main function
def main():
    setup_servos()
    fs = 44100  # Sampling frequency

    # Microphone positions (in meters)
    mic_positions = np.array([
        [6.12, -3.54, 7.07],  # Mic 1
        [-6.12, 3.54, -7.07],  # Mic 2
        [0, 7.07, 7.07],  # Mic 3
        [0, -7.07, -7.07],  # Mic 4
        [-6.12, -3.54, 7.07],  # Mic 5
        [6.12, 3.54, 7.07]  # Mic 6
    ])

    collection = connect_to_mongo()

    try:
        while True:
            # Simulated microphone signals
            mic_signals = [np.random.randn(10000) for _ in range(len(mic_positions))]

            # Simulated TDOAs
            tdoas = []
            for (i, j) in combinations(range(len(mic_positions)), 2):
                delay, _ = gcc_phat(mic_signals[i], mic_signals[j], fs)
                tdoas.append(delay)

            azimuth, elevation, range_ = triangulate(np.array(tdoas), mic_positions)

            move_servo(pan_pwm, azimuth)
            move_servo(tilt_pwm, elevation)

            print(f"Azimuth: {azimuth} degrees, Elevation: {elevation} degrees, Range: {range_} meters")

            if collection is not None:
                # Create data document
                video_data = {
                    'azimuth': azimuth,
                    'elevation': elevation,
                    'range': range_,
                    'timestamp': time.time()
                }

                # Insert into MongoDB
                collection.insert_one(video_data)
                print("Data sent to MongoDB successfully!")

            time.sleep(1)  # Adjust detection interval as needed

    except KeyboardInterrupt:
        print("Program terminated by user.")

    finally:
        pan_pwm.stop()
        tilt_pwm.stop()
        GPIO.cleanup()

if __name__ == "__main__":
    main()
