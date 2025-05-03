# Tkinter GUI library
import tkinter as tk
from matplotlib.backends.backend_tkagg import FigureCanvasTkAgg
#CyBot plot imports
import cybot_scan_data
import cybot_table_data
import cybot_sensor_data
from PIL import Image, ImageTk
import pylab as pl

cy_x = 0
cy_y = 0
circle = None
ax = None
canvas = None

def draw_gui():
    # Made global window so quit function (send_quit) can access
    global window
    #Make global frames so that data can be displayed from data_init()
    global scan_data, polar_plot, RT_cybot_visual, cybot_sensors

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

    # Load and display punching cy!
    # original_image = Image.open("U:\CprE288Workspace\FinalProject\Roomba_Bomb\python_gui\punchingcy.jpg")
    # resized_image = original_image.resize((300, 300), Image.Resampling.LANCZOS)
    # photo = ImageTk.PhotoImage(resized_image)
    
    # This line fixed image not appearing
    # cybot_sensors.photo = photo

    # image_label = tk.Label(cybot_sensors, image=photo)
    # image_label.grid(row=0, column=1)

def fetch_data():
    global fig, ax, canvas, circle

    # Get plot from cybot_scan_data.py
    fig, ax = cybot_scan_data.cybot_display_plot()

    # Add initial Cybot at 0, 0
    circle = pl.Circle((cy_x, cy_y), 35, transform=ax.transData._b, color="blue", alpha=0.4)
    ax.add_artist(circle)

    # Add plot to frame: polar_plot
    canvas = FigureCanvasTkAgg(fig, master=polar_plot)
    canvas.draw()
    canvas.get_tk_widget().grid(row=0, column=0, sticky="nsew")

    cybot_table_data.ping_table_data(scan_data)
    cybot_sensor_data.update_cybot_sensors()

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