import tkinter as tk
from tkinter import ttk

N_AppModes = 1
AppModes = None
MaxAppModes = 16
HeaderSize = 1
HeaderObjs = 2 #Objects / widgets that are part of the header and shouldn't be destroyed

AM_StrVar = []

def n_appmodes(tab, modes):
    global N_AppModes
    N_AppModes = int(modes.get())
    for i, item in enumerate(tab.winfo_children()):
        if i >= HeaderObjs:
            item.destroy()
    update_app_mode_tab(tab)
    

def draw_app_mode_tab(tab, appmodes):
    global N_AppModes, AppModeObjs, AM_StrVar, AppModes

    AppModes = appmodes
    N_AppModes = len(appmodes)
    n_strvar = N_AppModes + 1
    for i in range(n_strvar):
        AM_StrVar.insert(i, tk.StringVar())
    
    #Number of modes - Label + Spinbox
    label = tk.Label(tab, text="Number of Modes ")
    label.grid(row=1, column=1, sticky="w")
    spinb = tk.Spinbox(tab, width=28, values=tuple(range(1,MaxAppModes+1)), textvariable=AM_StrVar[0], 
                       command=lambda: n_appmodes(tab, AM_StrVar[0]))
    AM_StrVar[0].set(N_AppModes)
    spinb.grid(row=1, column=2)
    update_app_mode_tab(tab)    

        
def update_app_mode_tab(tab):
    global N_AppModes, AppModeObjs, AM_StrVar, AppModes

    # StrVar memory allocation checks
    n_am_strvar = len(AM_StrVar)-1
    if N_AppModes > n_am_strvar:
        for i in range(N_AppModes - n_am_strvar):
            AM_StrVar.insert(len(AM_StrVar), tk.StringVar())
            AppModes.insert(len(AppModes), "")
    elif n_am_strvar > N_AppModes:
        for i in range(n_am_strvar - N_AppModes):
            del AM_StrVar[-1]
            del AppModes[-1]

    # Draw new objects
    for i in range(1, N_AppModes+1):
        label = tk.Label(tab, text="Mode "+str(i)+": ")
        label.grid(row=HeaderSize+i, column=1, sticky="w")
        entry = tk.Entry(tab, width=30, textvariable=AM_StrVar[i])
        AM_StrVar[i].set(AppModes[i-1])
        entry.grid(row=HeaderSize+i, column=2)