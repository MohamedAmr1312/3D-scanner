import serial

ser = serial.Serial('COM6', 9600)  # Adjust COM port if needed

# Open the file in append mode to save the points
with open("ASAP89.xyz", "a") as file:
    print("Listening for points...")
    try:
        while True:
            # Read data from the serial port
            line = ser.readline().decode().strip()
            
            # Check if the line has 3 values separated by space
            if line and line.count(" ") == 2:
                try:
                    # Try to convert the line to x, y, z values
                    x, y, z = map(float, line.split())
                    
                    # Print the point to confirm it’s correct
                    print(f"Point: {x}, {y}, {z}")
                    
                    # Save the point to the file
                    file.write(f"{x} {y} {z}\n")
                
                except ValueError:
                    # If conversion fails, just skip this line
                    print(f"Skipped invalid line: {line}")
            else:
                print(f"Ignored line: {line}")

    except KeyboardInterrupt:
        # Stop the loop when Ctrl+C is pressed
        print("Stopped.")
        ser.close()  # Close the serial connection
