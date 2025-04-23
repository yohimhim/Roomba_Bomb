# Author: Phillip Jones
# Date: 08/19/2022, updated 05/25/2023, updated 07/13/2023, updated 07/16/2023
# Description: Simple client starter code for collecting data from the cybot. Using the UART or 
#              Network socket interfaced to either a mock CyBot server, or the actual CyBot

# General Python tutorials (W3schools):  https://www.w3schools.com/python/

# Serial library:  https://pyserial.readthedocs.io/en/latest/shortintro.html 
import serial
import time # Time library   
# Socket library:  https://realpython.com/python-sockets/  
# See: Background, Socket API Overview, and TCP Sockets  
import socket   
import os  # import function for finding absolute path to this python script

import keyboard # remember to pip install keyboard

# A little python magic to make it more convient for you to adjust where you want the data file to live
# Link for more info: https://towardsthecloud.com/get-relative-path-python 
absolute_path = os.path.dirname(__file__) # Absoult path to this python script
relative_path = "./"   # Path to sensor data file relative to this python script (./ means data file is in the same directory as this python script)
full_path = os.path.join(absolute_path, relative_path) # Full path to sensor data file
filename = 'sensor-scan.txt' # Name of file you want to store sensor data from your sensor scan command

# Choose to create either a UART or TCP port socket to communicate with Cybot (Not both!!)
# UART BEGIN
#cybot = serial.Serial('COM100', 115200)  # UART (Make sure you are using the correct COM port and Baud rate!!)
# UART END

# TCP Socket BEGIN (See Echo Client example): https://realpython.com/python-sockets/#echo-client-and-server
HOST = "192.168.1.1"  # The server's hostname or IP address
PORT = 288        # The port used by the server
cybot_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)  # Create a socket object
cybot_socket.connect((HOST, PORT))   # Connect to the socket  (Note: Server must first be running)
                      
cybot = cybot_socket.makefile("rbw", buffering=0)  # makefile creates a file object out of a socket:  https://pythontic.com/modules/socket/makefile
# TCP Socket END

print("Press a key to send command to CyBot (press 'q' to quit)...");

while True:
        event = keyboard.read_event()

        if event.event_type == keyboard.KEY_DOWN: #react once the key is pressed - not when it's released
                char = event.name

                if len(char) == 1:
                        send_message = char + '\n'
                        print(f"Sending {char}")
                        cybot.write(send_message.encode())

                        if char.lower() == 'q':
                                break

                        if char.lower() == 'm':
                                print("Requested Sensor scan from Cybot:\n")
                                rx_message = bytearray(1) # Initialize a byte array

                                # Create or overwrite existing sensor scan data file
                                file_object = open(full_path + filename,'w') # Open the file: file_object is just a variable for the file "handler" returned by open()

                                while (rx_message.decode() != "END\n"): # Collect sensor data until "END" recieved
                                        rx_message = cybot.readline()   # Wait for sensor response, readline expects message to end with "\n"
                                        file_object.write(rx_message.decode())  # Write a line of sensor data to the file
                                        print(rx_message.decode()) # Convert message from bytes to String (i.e., decode), then print

                                file_object.close() # Important to close file once you are done with it!!  
                        else:
                                print("Waiting for server reply\n")
                                rx_message = cybot.readline()
                                print("Got a message from serer: " + rx_message.decode() + "\n")

print("Client exiting, closing connections...")
time.sleep(2)
cybot.close()
cybot_socket.close()