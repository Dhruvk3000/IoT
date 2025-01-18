import RPi.GPIO as GPIO
import time

# Pin configuration
AZIMUTH_PIN = 17  # GPIO pin connected to the azimuth servo
ELEVATION_PIN = 27  # GPIO pin connected to the elevation servo

# Servo angle limits
MIN_ANGLE = 0  # Minimum servo angle in degrees
MAX_ANGLE = 180  # Maximum servo angle in degrees

# PWM settings
FREQ = 50  # Servo PWM frequency (50Hz is common for servos)

def setup():
    """Setup GPIO pins and PWM for servos."""
    GPIO.setmode(GPIO.BCM)
    GPIO.setup(AZIMUTH_PIN, GPIO.OUT)
    GPIO.setup(ELEVATION_PIN, GPIO.OUT)
    
    global azimuth_pwm, elevation_pwm
    azimuth_pwm = GPIO.PWM(AZIMUTH_PIN, FREQ)
    elevation_pwm = GPIO.PWM(ELEVATION_PIN, FREQ)
    
    azimuth_pwm.start(0)  # Start with PWM off
    elevation_pwm.start(0)  # Start with PWM off

def angle_to_duty_cycle(angle):
    """
    Convert an angle in degrees to a duty cycle percentage.
    Most servos accept a pulse width range of ~1ms (0 degrees) to ~2ms (180 degrees) at 50Hz.
    """
    return 2.5 + (angle / 180) * 10

def move_to_position(azimuth, elevation):
    """
    Move the servos to the specified azimuth and elevation angles.
    
    :param azimuth: Target azimuth angle (0-180 degrees)
    :param elevation: Target elevation angle (0-180 degrees)
    """
    if not (MIN_ANGLE <= azimuth <= MAX_ANGLE):
        print(f"Azimuth angle out of range: {azimuth}")
        return
    if not (MIN_ANGLE <= elevation <= MAX_ANGLE):
        print(f"Elevation angle out of range: {elevation}")
        return

    azimuth_duty = angle_to_duty_cycle(azimuth)
    elevation_duty = angle_to_duty_cycle(elevation)

    print(f"Moving to Azimuth: {azimuth}°, Elevation: {elevation}°")
    azimuth_pwm.ChangeDutyCycle(azimuth_duty)
    elevation_pwm.ChangeDutyCycle(elevation_duty)
    time.sleep(0.5)  # Wait for servos to move
    azimuth_pwm.ChangeDutyCycle(0)  # Stop sending signal
    elevation_pwm.ChangeDutyCycle(0)  # Stop sending signal

def cleanup():
    """Clean up GPIO and PWM."""
    azimuth_pwm.stop()
    elevation_pwm.stop()
    GPIO.cleanup()

if __name__ == "__main__":
    try:
        setup()
        while True:
            azimuth = float(input("Enter azimuth angle (0-180): "))
            elevation = float(input("Enter elevation angle (0-180): "))
            move_to_position(azimuth, elevation)
    except KeyboardInterrupt:
        print("Exiting program...")
    finally:
        cleanup()
