# Tkinter GUI library
import tkinter as tk 
#CyBot plot imports
import Roomba_Bomb.python_gui.final_gui.cybot_data as cybot_data
from matplotlib.backends.backend_tkagg import FigureCanvasTkAgg
import cybot_table_data

def main():
    gui_init()
    data_init()
    window.mainloop()

def gui_init():
    # Made global window so quit function (send_quit) can access
    global window
    #Make global frames so that data can be displayed from data_init()
    global scan_data, polar_plot, RT_cybot_visual, cybot_actions

    window = tk.Tk() # Create a Tk GUI Window
    window.title("CPRE-2880 Project Final GUI")
    window.geometry("500x500")

    #Using grid formation and .grid() is easier than .pack()
    window.rowconfigure([0, 1], weight=1)
    window.columnconfigure(0, weight=1)
    window.columnconfigure(1, weight=2)

    #Initialize frames for data visualization
    scan_data = tk.Frame(window, bg="gray")
    polar_plot = tk.Frame(window, bg="gray")
    RT_cybot_visual = tk.Frame(window, bg="gray")
    cybot_actions = tk.Frame(window, bg="gray")
    
    #Place frames in grid
    scan_data.grid(row=0, column=0, rowspan=2, sticky="nsew")
    polar_plot.grid(row=0, column=1, sticky="nsew")
    RT_cybot_visual.grid(row=1, column=1, sticky="nsew")
    cybot_actions.grid(row=1, column=1, sticky="nsew")

    #Allow for items in frames to resize with window
    scan_data.grid_rowconfigure(0, weight=1)
    scan_data.grid_columnconfigure(0, weight=1)
    polar_plot.grid_rowconfigure(0, weight=1)
    polar_plot.grid_columnconfigure(0, weight=1)
    RT_cybot_visual.grid_rowconfigure(0, weight=1)
    RT_cybot_visual.grid_columnconfigure(0, weight=1)
    cybot_actions.grid_rowconfigure(0, weight=1)
    cybot_actions.grid_columnconfigure(0, weight=1)

def data_init():
    #Polar Plot
    fig = cybot_data.cybot_display_plot()
    plot = FigureCanvasTkAgg(fig, master=polar_plot)

    #Data Tables
    cybot_table_data.ping_table_data(scan_data)

    plot.draw()
    plot.get_tk_widget().grid(sticky="nsew")

#Main
main()
