import tkinter as tk
from tkinter import ttk

class CounterTab:
    N_Counters = 1
    N_Counters_str = None
    MaxCounters = 16
    HeaderSize = 2
    HeaderObjs = 9 #Objects / widgets that are part of the header and shouldn't be destroyed

    Counters = None


    def __init__(self, cntrs):
        self.N_Counters_str = tk.StringVar(value=self.N_Counters)
        self.Counters = cntrs
        self.N_Counters = len(cntrs)


    def __del__(self):
        del self.N_Counters_str


    def update_ctrs(self, tab):
        self.N_Counters = self.N_Counters_str.get()
        for i, item in enumerate(tab.winfo_children()):
            if i > self.HeaderObjs:
                item.destroy()
        self.update(tab)
        

    def draw(self, tab):
        #Number of modes - Label + Spinbox
        label = tk.Label(tab, text="Number of Counters ")
        label.grid(row=1, column=1, sticky="w")
        spinb = tk.Spinbox(tab, width=10, values=tuple(range(1,self.MaxCounters+1)), 
                        command=lambda: self.update_ctrs(tab), textvariable=self.N_Counters_str)
        self.N_Counters_str.set(self.N_Counters)
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

        self.update(tab)


    def update(self, tab):
        # Draw new objects
        for i in range(1, int(self.N_Counters)+1):
            label = tk.Label(tab, text="Counter "+str(i)+" : ")
            label.grid(row=self.HeaderSize+i, column=1, sticky="e")
            entry = tk.Entry(tab, width=30) # Counter Name
            entry.grid(row=self.HeaderSize+i, column=2)
            entry = tk.Entry(tab, width=15) # MINCYCLE
            entry.grid(row=self.HeaderSize+i, column=3)
            entry = tk.Entry(tab, width=20) # MAXALLOWEDVALUE
            entry.grid(row=self.HeaderSize+i, column=4)
            entry = tk.Entry(tab, width=15) # TICKSPERBASE
            entry.grid(row=self.HeaderSize+i, column=5)
            entry = tk.Entry(tab, width=20) # TICKDURATION (ns)
            entry.grid(row=self.HeaderSize+i, column=6)
            entry = tk.Entry(tab, width=40) # Comments
            entry.grid(row=self.HeaderSize+i, column=7)