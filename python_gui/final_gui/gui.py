# Tkinter GUI library
import tkinter as tk
from tkinter import ttk
from matplotlib.backends.backend_tkagg import FigureCanvasTkAgg
#CyBot plot imports
import cybot_scan_data
import cybot_table_data
import cybot_sensor_data
from PIL import Image, ImageTk
import pylab as pl
#plot imports
import numpy as np
import matplotlib.pyplot as plt

# Serial library:  https://pyserial.readthedocs.io/en/latest/shortintro.html 
import serial
import time # Time library   
# Socket library:  https://realpython.com/python-sockets/  
# See: Background, Socket API Overview, and TCP Sockets  
import socket   
import os  # import function for finding absolute path to this python script

import keyboard # remember to pip install keyboard
#A little python magic to make it more convient for you to adjust where you want the data file to live
#Link for more info: https://towardsthecloud.com/get-relative-path-python 
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
    
# Data Tables
angle_degrees = []
angle_radians = []
ping = []
ir = []

cy_x = 0
cy_y = -30
circle = None
ax = None
canvas = None

def main():
    print("Press a key to send command to CyBot (press 'q' to quit)...")
    draw_gui()
    update_data()
    window.mainloop()
    print("Client exiting, closing connections...")
    time.sleep(2)
    cybot.close()
    cybot_socket.close()

def update_data():
    socket_data()
    window.after(1, update_data)

def draw_gui():
    # Made global window so quit function (send_quit) can access
    global window
    #Make global frames so that data can be displayed from data_init()
    global scan_data, polar_plot, cybot_sensors
    global fig, ax, canvas, circle, tree


    window = tk.Tk() # Create a Tk GUI Window
    window.title("CPRE-2880 Project Final GUI")
    window.geometry("1000x1000")

    #Using grid formation and .grid() is easier than .pack()
    window.rowconfigure([0, 1], weight=1)
    window.columnconfigure(0, weight=1)
    window.columnconfigure(1, weight=2)

    #Initialize frames for data visualization
    scan_data = tk.Frame(window, bg="white")
    polar_plot = tk.Frame(window, bg="white")
    cybot_sensors = tk.Frame(window, bg="white")
    
    #Place frames in grid
    scan_data.grid(row=0, column=0, rowspan=2, sticky="nsew")
    polar_plot.grid(row=0, column=1, sticky="nsew")
    cybot_sensors.grid(row=1, column=1, sticky="nsew")

    #Allow for items in frames to resize with window
    scan_data.grid_rowconfigure(0, weight=1)
    scan_data.grid_columnconfigure(0, weight=1)
    polar_plot.grid_rowconfigure(0, weight=1)
    polar_plot.grid_columnconfigure(0, weight=1)
    cybot_sensors.grid_rowconfigure(0, weight=1)
    cybot_sensors.grid_columnconfigure(0, weight=1)

    #The following code configures the cybot_sensors frame---------------------------------------------------
    sensor_label_container = tk.Frame(cybot_sensors, bg="white")
    sensor_label_container.grid(row=0, column=0, sticky="nw", padx=10, pady=10)

    #create labels
    cliff1_label = tk.StringVar()
    cliff2_label = tk.StringVar()
    cliff3_label = tk.StringVar()
    cliff4_label = tk.StringVar()
    bump_right_label = tk.StringVar()
    bump_left_label = tk.StringVar()

    #set default starting text
    cliff1_label.set("Cliff 1 Sensor: ---")
    cliff2_label.set("Cliff 2 Sensor: ---")
    cliff3_label.set("Cliff 3 Sensor: ---")
    cliff4_label.set("Cliff 4 Sensor: ---")
    bump_right_label.set("Bump right Sensor: ---")
    bump_left_label.set("Bump left Sensor: ---")

    #config labels
    cliff1_label = tk.Label(sensor_label_container, textvariable=cliff1_label, font=(16))
    cliff2_label = tk.Label(sensor_label_container, textvariable=cliff2_label, font=(16))
    cliff3_label = tk.Label(sensor_label_container, textvariable=cliff3_label, font=(16))
    cliff4_label = tk.Label(sensor_label_container, textvariable=cliff4_label, font=(16))
    bump_right_label = tk.Label(sensor_label_container, textvariable=bump_right_label, font=(16))
    bump_left_label = tk.Label(sensor_label_container, textvariable=bump_left_label, font=(16))

    #place labels
    cliff1_label.grid(row=0, column=0, sticky="w", padx=2, pady=2)
    cliff2_label.grid(row=1, column=0, sticky="w", padx=2, pady=2)
    cliff3_label.grid(row=2, column=0, sticky="w", padx=2, pady=2)
    cliff4_label.grid(row=3, column=0, sticky="w", padx=2, pady=2)
    bump_right_label.grid(row=4, column=0, sticky="w", padx=2, pady=2)
    bump_left_label.grid(row=5, column=0, sticky="w", padx=2, pady=2)

    #Add initial polar plot
    fig, ax = cybot_scan_data.cybot_display_plot(polar_plot, angle_degrees, ping)

    #Add initial scan data tree
    tree = cybot_table_data.ping_table_data(scan_data, angle_degrees, ping, ir)

    # Add initial Cybot at 0, 0
    circle = pl.Circle((cy_x, cy_y), 35, transform=ax.transData._b, color="blue", alpha=0.4)
    ax.add_artist(circle)

    #Load and display punching cy!
    original_image = Image.open("python_gui/final_gui/punchingcy.jpg")
    resized_image = original_image.resize((300, 300), Image.Resampling.LANCZOS)
    photo = ImageTk.PhotoImage(resized_image)
    # This line fixed image not appearing
    cybot_sensors.photo = photo
    image_label = tk.Label(cybot_sensors, image=photo)
    image_label.grid(row=0, column=1)


def socket_data():
    global scan_data, polar_plot
    global fig, ax, tree 
    event = keyboard.read_event()

    if event.event_type == keyboard.KEY_DOWN: #react once the key is pressed - not when it's released
            char = event.name

            if len(char) == 1:
                    send_message = char + '\n'
                    print(f"Sending {char}")
                    cybot.write(send_message.encode())

                    if char.lower() == 'z':
                            print("Requested Sensor scan from Cybot:\n")
                            rx_message = bytearray(1) # Initialize a byte array

                            # Create or overwrite existing sensor scan data file
                            file_object = open(full_path + filename,'w') # Open the file: file_object is just a variable for the file "handler" returned by open()

                            while (rx_message.decode() != "END\n"): # Collect sensor data until "END" recieved
                                    rx_message = cybot.readline()   # Wait for sensor response, readline expects message to end with "\n"
                                    file_object.write(rx_message.decode())  # Write a line of sensor data to the file
                                    print(rx_message.decode()) # Convert message from bytes to String (i.e., decode), then print
                            file_object.close() # Important to close file one you are done with it!!
                            # For each line of the file split into columns, and assign each column to a variable
                            file_object = open(full_path + filename,'r') # Open the file: file_object is just a variable for the file "handler" returned by open()
                            line1 = file_object.readline()
                            file_header = file_object.readline() # Read and store the header row (i.e., 1st row) of the file into file_header
                            line3 = file_object.readline()
                            file_data = file_object.readlines() # Read the rest of the lines of the file into file_data
                            file_object.close() # Important to close file one you are done with it!!
                            
                            for line in file_data:
                                data = line.split()    # Split line into columns (by default delineates columns by whitespace)
                                if data[0] == "END":
                                    break
                                angle_degrees.append(float(data[0]))  # Column 0 holds the angle at which distance was measured
                                ping.append(float(data[1]))       # Column 1 holds the distance that was measured at a given angle 
                                ir.append(float(data[2]))

                            ax.cla() #clear plot and redraw
                            cybot_scan_data.cybot_display_plot(fig, ax, polar_plot, angle_degrees, ping)
                            for item in tree.get_children(): #clear table data
                                tree.delete(item)
                            cybot_table_data.ping_table_data(tree, scan_data, angle_degrees, ping, ir)
                    else:
                            print("Waiting for server reply\n")
                            rx_message = cybot.readline()
                            print("Got a message from serer: " + rx_message.decode() + "\n")
                    # if char.lower() == 'z':
                    #         print("Requested Sensor scan from Cybot:\n")
                            
                    #         with open(full_path + filename, 'w') as file_object:
                    #                 while True:
                    #                         rx_message = cybot.readline()
                    #                         decoded_message = rx_message.decode()
                                            
                    #                         if decoded_message.strip() == "END": #IT ENDS WHEN IT SEES 'END' SO CHANGE IT WHEN I GET BACK FROM 310!!!
                    #                                 break
                                            
                    #                         file_object.write(decoded_message)
                    #                         print(decoded_message)

def cybot_animate(x, y):
    global cy_x, cy_y, circle, canvas

    cy_x += x
    cy_y += y

    #Converting cartesian coordinates to polar coordinates
    # r = math.sqrt((cy_x * cy_x) + (cy_y * cy_y))
    # theta = math.atan2(cy_y, cy_x)
    # theta_degrees = math.degrees(theta)


    #if circle is created
    if circle:
        circle.set_center((cy_x, cy_y))  # Move cybot

    canvas.draw()  # Redraw canvas with new coordinates

    window.after(1000, cybot_animate)


main()