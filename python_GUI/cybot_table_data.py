
def ping_table_data(tree, parent_frame, angle, ping_data, ir_data):
    # Data Tables
    angle_degrees = angle
    ping = ping_data
    ir = ir_data

    for item in tree.get_children(): #clear table data
        tree.delete(item)
    # Create Treeview
    tree.heading("Angle", text="Angle (degrees)")
    tree.heading("Ping", text="Ping (cm)")
    tree.heading("IR", text="IR (cm)")
    tree.column("Angle", anchor="center", width=100)
    tree.column("Ping", anchor="center", width=100)
    tree.column("IR", anchor="center", width=100)
    # Add Treeview (table) to parent frame/window grid
    tree.grid(sticky="nsew")

    # Insert data
    for angle, ping_val, ir_val in zip(angle_degrees, ping, ir):
        tree.insert("", "end", values=(angle, ping_val, ir_val))
