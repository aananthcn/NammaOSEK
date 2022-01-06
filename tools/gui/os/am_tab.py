import tkinter as tk
from tkinter import ttk

N_AppModes = 1
MaxAppModes = 16
HeaderSize = 1
HeaderObjs = 2 #Objects / widgets that are part of the header and shouldn't be destroyed


def n_appmodes(tab, modes):
    global N_AppModes
    N_AppModes = int(modes.get())
    for i, item in enumerate(tab.winfo_children()):
        if i >= HeaderObjs:
            item.destroy()
    update_app_mode_tab(tab)
    

def draw_app_mode_tab(tab):
    global N_AppModes, AppModeObjs
    N_AppModes_str = tk.StringVar(value=N_AppModes)
    
    #Number of modes - Label + Spinbox
    label = tk.Label(tab, text="Number of Modes ")
    label.grid(row=1, column=1, sticky="w")
    spinb = tk.Spinbox(tab, width=28, values=tuple(range(1,MaxAppModes+1)),
                       command=lambda: n_appmodes(tab, N_AppModes_str), textvariable=N_AppModes_str)
    spinb.grid(row=1, column=2)
    update_app_mode_tab(tab)    

        
def update_app_mode_tab(tab):
    global N_AppModes, AppModeObjs
    
    # Draw new objects
    for i in range(1, N_AppModes+1):
        label = tk.Label(tab, text="Mode "+str(i)+": ")
        label.grid(row=HeaderSize+i, column=1, sticky="w")
        entry = tk.Entry(tab, width=30)
        entry.grid(row=HeaderSize+i, column=2)