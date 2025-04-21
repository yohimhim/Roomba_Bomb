#plot imports
from logging import root
from tkinter import ttk
import tkinter as tk
import numpy as np
import matplotlib.pyplot as plt
#filepath import
import os

def ping_table_data(parent_frame):
    absolute_path = os.path.dirname(__file__) # Absolut path to this python script
    relative_path = "./"   # Path to sensor data file relative to this python script (./ means data file is in the same directory as this python script
    full_path = os.path.join(absolute_path, relative_path) # Full path to sensor data file
    filename = 'mock-cybot-sensor-scan.txt' # Name of sensor data file

    # Open file containing CyBot sensor scan from 0 - 180 degrees
    file_object = open(full_path + filename,'r') # Open the file: file_object is just a variable for the file "handler" returned by open()
    file_header = file_object.readline() # Read and store the header row (i.e., 1st row) of the file into file_header
    file_data = file_object.readlines()  # Read the rest of the lines of the file into file_data
    file_object.close() # Important to close file one you are done with it!!

    # Data Tables
    angle_degrees = []
    distance = []

    for line in file_data: 
        data = line.split()                   # Split line into columns (by default delineates columns by whitespace)
        angle_degrees.append(float(data[0]))  # Column 0 holds the angle at which distance was measured
        distance.append(float(data[1]))       # Column 1 holds the distance that was measured at a given angle 

    # Create Treeview
    tree = ttk.Treeview(parent_frame, columns=("Angle", "Distance"), show="headings")
    tree.heading("Angle", text="Angle (degrees)")
    tree.heading("Distance", text="Distance (cm)")
    tree.column("Angle", anchor="center", width=100)
    tree.column("Distance", anchor="center", width=100)

    # Insert data
    for angle, dist in zip(angle_degrees, distance):
        tree.insert("", "end", values=(angle, dist))

    # Add Treeview (table) to parent frame/window grid
    tree.grid(row=0, column=0, sticky="nsew")
    
    # Makes sure columns are equal size
    parent_frame.columnconfigure(0, weight=1, uniform="equal")  # Column 0 for angle
    parent_frame.columnconfigure(1, weight=1, uniform="equal")  # Column 1 for distance


