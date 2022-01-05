import tkinter as tk
from tkinter import ttk

N_AppModes = 1

def n_appmodes(tab, modes):
    global N_AppModes
    N_AppModes = int(modes.get())
    for i, item in enumerate(tab.winfo_children()):
        if i > 1:
            item.destroy()
    update_app_mode_tab(tab)
    

def draw_app_mode_tab(tab):
    global N_AppModes, AppModeObjs
    N_AppModes_str = tk.StringVar(value=N_AppModes)
    
    #Number of modes - Label + Spinbox
    label = tk.Label(tab, text="Number of Modes ")
    label.grid(row=1, column=1, sticky="w")
    spinb = tk.Spinbox(tab, width=27, values=tuple(range(1,17)), command=lambda: n_appmodes(tab, N_AppModes_str),
                       textvariable=N_AppModes_str)
    spinb.grid(row=1, column=2)
    
    for i in range(1, N_AppModes+1):
        label = tk.Label(tab, text="Mode "+str(i)+": ")
        label.grid(row=1+i, column=1, sticky="w")
        entry = tk.Entry(tab, width=30)
        entry.grid(row=1+i, column=2)

        
def update_app_mode_tab(tab):
    global N_AppModes, AppModeObjs
    print("Entering: update_app_mode_tab()")
    
    # Draw new objects
    for i in range(1, N_AppModes+1):
        label = tk.Label(tab, text="Mode "+str(i)+": ")
        label.grid(row=1+i, column=1, sticky="w")
        entry = tk.Entry(tab, width=30)
        entry.grid(row=1+i, column=2)