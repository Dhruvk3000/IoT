import os
import subprocess

# Directory to save captured files
OUTPUT_DIR = "/home/pi/camera_output"
os.makedirs(OUTPUT_DIR, exist_ok=True)

def preview_camera(duration=5):
    """
    Preview the camera feed for a specified duration.
    
    :param duration: Time to preview the feed in seconds.
    """
    print(f"Previewing camera for {duration} seconds...")
    subprocess.run(["libcamera-hello", "--timeout", str(duration * 1000)])

def capture_photo(filename="photo.jpg"):
    """
    Capture a photo using the Raspberry Pi camera.
    
    :param filename: Name of the photo file to save.
    """
    filepath = os.path.join(OUTPUT_DIR, filename)
    print(f"Capturing photo and saving to {filepath}...")
    subprocess.run(["libcamera-jpeg", "-o", filepath])

def record_video(filename="video.h264", duration=10):
    """
    Record a video using the Raspberry Pi camera.
    
    :param filename: Name of the video file to save.
    :param duration: Duration of the video in seconds.
    """
    filepath = os.path.join(OUTPUT_DIR, filename)
    print(f"Recording video for {duration} seconds and saving to {filepath}...")
    subprocess.run(["libcamera-vid", "--timeout", str(duration * 1000), "-o", filepath])

def main():
    """
    Main function to provide options to the user for camera operations.
    """
    while True:
        print("\nSelect an option:")
        print("1. Preview Camera")
        print("2. Capture Photo")
        print("3. Record Video")
        print("4. Exit")
        choice = input("Enter your choice: ")

        if choice == "1":
            duration = int(input("Enter preview duration (seconds): "))
            preview_camera(duration)
        elif choice == "2":
            filename = input("Enter filename for the photo (default: photo.jpg): ") or "photo.jpg"
            capture_photo(filename)
        elif choice == "3":
            filename = input("Enter filename for the video (default: video.h264): ") or "video.h264"
            duration = int(input("Enter video duration (seconds): "))
            record_video(filename, duration)
        elif choice == "4":
            print("Exiting...")
            break
        else:
            print("Invalid choice. Please try again.")

if __name__ == "__main__":
    main()
