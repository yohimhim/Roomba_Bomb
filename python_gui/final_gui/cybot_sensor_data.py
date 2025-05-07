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

cliff1_var = None
cliff2_var = None
cliff3_var = None
cliff4_var = None
bump_right_var = None
bump_left_var = None
sensor_label_container = None

def draw_cybot_sensors(parent_frame):
    global cliff1_var, cliff2_var, cliff3_var, cliff4_var, bump_right_var, bump_left_var, sensor_label_container

    sensor_label_container = tk.Frame(parent_frame, bg="white")
    sensor_label_container.grid(row=0, column=0, sticky="nw", padx=10, pady=10)

    #create labels
    cliff1_var = tk.StringVar()
    cliff2_var = tk.StringVar()
    cliff3_var = tk.StringVar()
    cliff4_var = tk.StringVar()
    bump_right_var = tk.StringVar()
    bump_left_var = tk.StringVar()

    #set default starting text
    cliff1_var.set("Cliff 1 Sensor: ---")
    cliff2_var.set("Cliff 2 Sensor: ---")
    cliff3_var.set("Cliff 3 Sensor: ---")
    cliff4_var.set("Cliff 4 Sensor: ---")
    bump_right_var.set("Bump right Sensor: ---")
    bump_left_var.set("Bump left Sensor: ---")

    #config labels
    cliff1_label = tk.Label(sensor_label_container, textvariable=cliff1_var, font=(16))
    cliff2_label = tk.Label(sensor_label_container, textvariable=cliff2_var, font=(16))
    cliff3_label = tk.Label(sensor_label_container, textvariable=cliff3_var, font=(16))
    cliff4_label = tk.Label(sensor_label_container, textvariable=cliff4_var, font=(16))
    bump_right_label = tk.Label(sensor_label_container, textvariable=bump_right_var, font=(16))
    bump_left_label = tk.Label(sensor_label_container, textvariable=bump_left_var, font=(16))

    #place labels
    cliff1_label.grid(row=0, column=0, sticky="w", padx=2, pady=2)
    cliff2_label.grid(row=1, column=0, sticky="w", padx=2, pady=2)
    cliff3_label.grid(row=2, column=0, sticky="w", padx=2, pady=2)
    cliff4_label.grid(row=3, column=0, sticky="w", padx=2, pady=2)
    bump_right_label.grid(row=4, column=0, sticky="w", padx=2, pady=2)
    bump_left_label.grid(row=5, column=0, sticky="w", padx=2, pady=2)


def update_cybot_sensors(string, sensor):
    global cliff1_var, cliff2_var, cliff3_var, cliff4_var, bump_right_var, bump_left_var
    if sensor == "Cliff:1":
        cliff1_var.set("Cliff 1 Sensor: " + string)
        return

    elif sensor == "Cliff:2": 
        cliff2_var.set("Cliff 2 Sensor: " + string)
        return

    elif sensor == "Cliff:3": 
        cliff3_var.set("Cliff 3 Sensor: " + string)
        return

    elif sensor == "Cliff:4": 
        cliff4_var.set("Cliff 4 Sensor: " + string)
        return

    elif sensor == "Cliff:2,3": 
        cliff2_var.set("Cliff 2 Sensor: " + string)
        cliff3_var.set("Cliff 3 Sensor: " + string)
        return

    elif sensor == "Cliff:1,2": 
        cliff1_var.set("Cliff 1 Sensor: " + string)
        cliff2_var.set("Cliff 2 Sensor: " + string)
        return

    elif sensor == "Cliff:3,4":
        cliff3_var.set("Cliff 3 Sensor: " + string)
        cliff4_var.set("Cliff 4 Sensor: " + string)
        return
    
    elif sensor == "Bump:Left":
        bump_left_var.set("Bump left Sensor: " + string)
        return
    elif sensor == "Bump:Right":
        bump_right_var.set("Bump right Sensor: " + string)
        return

    cliff1_var.set("Cliff 1 Sensor: " + string)
    cliff2_var.set("Cliff 2 Sensor: " + string)
    cliff3_var.set("Cliff 3 Sensor: " + string)
    cliff4_var.set("Cliff 4 Sensor: " + string)
    bump_left_var.set("Bump left Sensor: " + string)
    bump_right_var.set("Bump right Sensor: " + string)
