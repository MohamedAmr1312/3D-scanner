import serial

ser = serial.Serial('COM10', 9600)  # Change this to match your port
file = open("points.xyz", "a")        # Append mode so you can add more points

print("Listening... Press Ctrl+C to stop.")

try:
    while True:
        line = ser.readline().decode().strip()
        if line and line.count(" ") == 2:
            print(line)
            file.write(line + "\n")
except KeyboardInterrupt:
    print("Stopped.")
    file.close()
    ser.close()
