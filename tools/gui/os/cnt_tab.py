import tkinter as tk
from tkinter import ttk

N_Counters = 1
MaxCounters = 16
HeaderSize = 2
HeaderObjs = 9 #Objects / widgets that are part of the header and shouldn't be destroyed


def n_Counters(tab, modes):
    global N_Counters
    N_Counters = int(modes.get())
    for i, item in enumerate(tab.winfo_children()):
        if i > HeaderObjs:
            item.destroy()
    update_counters_tab(tab)
    

def draw_counters_tab(tab):
    global N_Counters, AppModeObjs
    N_Counters_str = tk.StringVar(value=N_Counters)
    
    #Number of modes - Label + Spinbox
    label = tk.Label(tab, text="Number of Counters ")
    label.grid(row=1, column=1, sticky="w")
    spinb = tk.Spinbox(tab, width=10, values=tuple(range(1,MaxCounters+1)), 
                       command=lambda: n_Counters(tab, N_Counters_str), textvariable=N_Counters_str)
    spinb.grid(row=1, column=2, sticky="w")
    
    # Table heading
    label = tk.Label(tab, text=" ")
    label.grid(row=2, column=1, sticky="w")
    label = tk.Label(tab, text="Counter Name")
    label.grid(row=2, column=2, sticky="w")
    label = tk.Label(tab, text="MINCYCLE")
    label.grid(row=2, column=3, sticky="we")
    label = tk.Label(tab, text="MAXALLOWEDVALUE")
    label.grid(row=2, column=4, sticky="we")
    label = tk.Label(tab, text="TICKSPERBASE")
    label.grid(row=2, column=5, sticky="we")
    label = tk.Label(tab, text="TICKDURATION (ns)")
    label.grid(row=2, column=6, sticky="we")
    label = tk.Label(tab, text="Comments")
    label.grid(row=2, column=7, sticky="w")
    
    update_counters_tab(tab)    

        
def update_counters_tab(tab):
    global N_Counters, AppModeObjs
    
    # Draw new objects
    for i in range(1, N_Counters+1):
        label = tk.Label(tab, text="Counter "+str(i)+" : ")
        label.grid(row=HeaderSize+i, column=1, sticky="e")
        entry = tk.Entry(tab, width=30) # Counter Name
        entry.grid(row=HeaderSize+i, column=2)
        entry = tk.Entry(tab, width=15) # MINCYCLE
        entry.grid(row=HeaderSize+i, column=3)
        entry = tk.Entry(tab, width=20) # MAXALLOWEDVALUE
        entry.grid(row=HeaderSize+i, column=4)
        entry = tk.Entry(tab, width=15) # TICKSPERBASE
        entry.grid(row=HeaderSize+i, column=5)
        entry = tk.Entry(tab, width=20) # TICKDURATION (ns)
        entry.grid(row=HeaderSize+i, column=6)
        entry = tk.Entry(tab, width=40) # Comments
        entry.grid(row=HeaderSize+i, column=7)