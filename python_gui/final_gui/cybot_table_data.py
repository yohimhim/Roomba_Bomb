from tkinter import ttk
#filepath import
import os

def ping_table_data(parent_frame):
    absolute_path = os.path.dirname(__file__) # Absolut path to this python script
    relative_path = "./"   # Path to sensor data file relative to this python script (./ means data file is in the same directory as this python script
    full_path = os.path.join(absolute_path, relative_path) # Full path to sensor data file
    filename = 'sensor-scan.txt' # Name of sensor data file

    # Open file containing CyBot sensor scan from 0 - 180 degrees
    file_object = open(full_path + filename,'r') # Open the file: file_object is just a variable for the file "handler" returned by open()
    file_z = file_object.readline()
    file_header = file_object.readline() # Read and store the header row (i.e., 1st row) of the file into file_header
    file_space = file_object.readline()
    
      # Read the rest of the lines of the file into file_data

    
    file_data = file_object.readlines()
    
    file_object.close() # Important to close file one you are done with it!!

    # Data Tables
    angle_degrees = []
    ping = []
    ir = []

    for line in file_data: 
        if line in file_data == 'END':
            break
        data = line.split()                   # Split line into columns (by default delineates columns by whitespace)
        angle_degrees.append(float(data[0]))  # Column 0 holds the angle at which distance was measured
        ping.append(float(data[1]))       # Column 1 holds the distance that was measured at a given angle 
        ir.append(float(data[2]))

    # Create Treeview
    tree = ttk.Treeview(parent_frame, columns=("Angle", "Ping", "IR"), show="headings")
    tree.heading("Angle", text="Angle (degrees)")
    tree.heading("Ping", text="Ping (cm)")
    tree.heading("IR", text="IR (cm)")
    tree.column("Angle", anchor="center", width=100)
    tree.column("Ping", anchor="center", width=100)
    tree.column("IR", anchor="center", width=100)

    # Insert data
    for angle, ping_val, ir_val in zip(angle_degrees, ping, ir):
        tree.insert("", "end", values=(angle, ping_val, ir_val))

    # Add Treeview (table) to parent frame/window grid
    tree.grid(sticky="nsew")