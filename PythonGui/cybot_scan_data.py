#plot imports
import numpy as np
from matplotlib.backends.backend_tkagg import FigureCanvasTkAgg
import pylab as pl

def cybot_display_plot(fig, ax, parent_frame, angle, ping_data, x, y):
    angle_degrees = angle
    angle_radians = [] # Initially empty
    distance = ping_data  

    ax.cla() #clear plot and redraw

    # Convert python sequence (list of strings) into a numpy array
    angle_degrees = np.array(angle_degrees) # Avoid "TypeError: can't multiply sequence by non-int of type float"
                                            # Link for more info: https://www.freecodecamp.org/news/typeerror-cant-multiply-sequence-by-non-int-of-type-float-solved-python-error/
                                            
    angle_radians = (np.pi/180) * angle_degrees # Convert degrees into radians

    # Create a polar plot to be returned
    ax.plot(angle_radians, distance, color='r', linewidth=4.0)  # Plot distance verse angle (in radians), using red, line width 4.0
    ax.set_xlabel('Distance (cm)', fontsize = 10.0)  # Label x axis
    ax.set_ylabel('Angle (degrees)', fontsize = 10.0) # Label y axis
    ax.xaxis.set_label_coords(0.5, 0.15) # Modify location of x axis label (Typically do not need or want this)
    ax.tick_params(axis='both', which='major', labelsize=6) # set font size of tick labels
    ax.set_rmax(2.5)                    # Saturate distance at 2.5 meters
    ax.set_rticks([10, 20, 30, 40, 50, 60, 70, 80, 100, 120])   # Set plot "distance" tick marks at .5, 1, 1.5, 2, and 2.5 meters
    ax.set_rlabel_position(-22.5)     # Adjust location of the radial labels
    ax.set_thetamax(180)              # Saturate angle to 180 degrees
    ax.set_xticks(np.arange(0,np.pi+.1,np.pi/4)) # Set plot "angle" tick marks to pi/4 radians (i.e., displayed at 45 degree) increments
                                                # Note: added .1 to pi to go just beyond pi (i.e., 180 degrees) so that 180 degrees is displayed
    ax.grid(True)                     # Show grid lines

    # Create title for plot (font size = 14pt, y & pad controls title vertical location)
    ax.set_title("POLAR PLOT", size=14, y=1.0, pad=-24) 
    canvas = FigureCanvasTkAgg(fig, master=parent_frame)
    canvas.get_tk_widget().grid(row=0, column=0, sticky="nsew")

    # Add initial Cybot at 0, 0
    mock_cybot = pl.Circle((x, y), 35, transform=ax.transData._b, color="blue", alpha=0.4)
    front = pl.Circle((x, y + 20), 5, transform=ax.transData._b, color="red", alpha=0.4)
    ax.add_artist(mock_cybot)
    ax.add_artist(front)

    canvas.draw()