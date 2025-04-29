#plot imports
import numpy as np
import matplotlib.pyplot as plt

#filepath import
import os


def cybot_display_plot():
    absolute_path = os.path.dirname(__file__) # Absoult path to this python script
    relative_path = "./"   # Path to sensor data file relative to this python script (./ means data file is in the same directory as this python script
    full_path = os.path.join(absolute_path, relative_path) # Full path to sensor data file
    filename = 'mock-cybot-sensor-scan.txt' # Name of sensor data file

    # angle_degrees: a vector (i.e., array of numbers) for which each element is an angle at which the sensor makes a distance measurement.
    # Units: degrees
    angle_degrees = [] # Initially empty

    # angle_radians: a vector that contains converted elements of vector angle_degrees into radians 
    # Units radians
    angle_radians = [] # Initially empty

    # distance: a vector, where each element is the corresponding distance measured at each angle in vector angle_degrees
    # Units: meters
    distance = []      # Initially empty

    # Open file containing CyBot sensor scan from 0 - 180 degrees
    file_object = open(full_path + filename,'r') # Open the file: file_object is just a variable for the file "handler" returned by open()
    file_header = file_object.readline() # Read and store the header row (i.e., 1st row) of the file into file_header
    file_data = file_object.readlines()  # Read the rest of the lines of the file into file_data
    file_object.close() # Important to close file one you are done with it!!

    # For each line of the file split into columns, and assign each column to a variable
    for line in file_data: 
        data = line.split()    # Split line into columns (by default delineates columns by whitespace)
        angle_degrees.append(float(data[0]))  # Column 0 holds the angle at which distance was measured
        distance.append(float(data[1]))       # Column 1 holds the distance that was measured at a given angle 


    # Convert python sequence (list of strings) into a numpy array
    angle_degrees = np.array(angle_degrees) # Avoid "TypeError: can't multiply sequence by non-int of type float"
                                            # Link for more info: https://www.freecodecamp.org/news/typeerror-cant-multiply-sequence-by-non-int-of-type-float-solved-python-error/
                                            
    angle_radians = (np.pi/180) * angle_degrees # Convert degrees into radians

    # Create a polar plot to be returned
    fig, ax = plt.subplots(subplot_kw={'projection': 'polar'}) # One subplot of type polar
    ax.plot(angle_radians, distance, color='r', linewidth=4.0)  # Plot distance verse angle (in radians), using red, line width 4.0
    ax.set_xlabel('Distance (cm)', fontsize = 10.0)  # Label x axis
    ax.set_ylabel('Angle (degrees)', fontsize = 10.0) # Label y axis
    ax.xaxis.set_label_coords(0.5, 0.15) # Modify location of x axis label (Typically do not need or want this)
    ax.tick_params(axis='both', which='major', labelsize=6) # set font size of tick labels
    ax.set_rmax(2.5)                    # Saturate distance at 2.5 meters
    ax.set_rticks([5, 10, 15, 20, 25, 30, 35, 40, 45, 50])   # Set plot "distance" tick marks at .5, 1, 1.5, 2, and 2.5 meters
    ax.set_rlabel_position(-22.5)     # Adjust location of the radial labels
    ax.set_thetamax(180)              # Saturate angle to 180 degrees
    ax.set_xticks(np.arange(0,np.pi+.1,np.pi/4)) # Set plot "angle" tick marks to pi/4 radians (i.e., displayed at 45 degree) increments
                                                # Note: added .1 to pi to go just beyond pi (i.e., 180 degrees) so that 180 degrees is displayed
    ax.grid(True)                     # Show grid lines

    # Create title for plot (font size = 14pt, y & pad controls title vertical location)
    ax.set_title("POLAR PLOT", size=14, y=1.0, pad=-24) 

    return fig, ax