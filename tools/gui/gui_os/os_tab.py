import tkinter as tk
from tkinter import ttk


def draw_os_tab(tab):
    #CPU / SoC - Label + Edit-box
    label = tk.Label(tab, text="CPU / SoC name")
    label.grid(row=1, column=1, sticky="w")
    textb = tk.Entry(tab,text="Entry", width=30)
    textb.grid(row=1, column=2)
 
    # OS Name - Label + Edit-box
    label = tk.Label(tab, text="Image Name")
    label.grid(row=2, column=1, sticky="w")
    textb = tk.Entry(tab, text="Entry", width=30)
    textb.grid(row=2, column=2)
 
    # OSEK Standard - Label + Combo-box
    label = tk.Label(tab, text="OSEK Standard")
    label.grid(row=3, column=1, sticky="w")
    osekstd = ttk.Combobox(tab, width=27)
    osekstd['values'] = ("STANDARD", "EXTENDED")
    osekstd.current()
    osekstd.grid(row=3, column=2)

    # STARTUPHOOK - Label + Combo-box
    label = tk.Label(tab, text="STARTUPHOOK")
    label.grid(row=4, column=1, sticky="w")
    osekstd = ttk.Combobox(tab, width=27)
    osekstd['values'] = ("FALSE", "TRUE")
    osekstd.current()
    osekstd.grid(row=4, column=2)
 
    # ERRORHOOK - Label + Combo-box
    label = tk.Label(tab, text="ERRORHOOK")
    label.grid(row=5, column=1, sticky="w")
    osekstd = ttk.Combobox(tab, width=27)
    osekstd['values'] = ("FALSE", "TRUE")
    osekstd.current()
    osekstd.grid(row=5, column=2)

    # SHUTDOWNHOOK - Label + Combo-box
    label = tk.Label(tab, text="SHUTDOWNHOOK")
    label.grid(row=6, column=1, sticky="w")
    osekstd = ttk.Combobox(tab, width=27)
    osekstd['values'] = ("FALSE", "TRUE")
    osekstd.current()
    osekstd.grid(row=6, column=2)

    # PRETASKHOOK - Label + Combo-box
    label = tk.Label(tab, text="PRETASKHOOK")
    label.grid(row=7, column=1, sticky="w")
    osekstd = ttk.Combobox(tab, width=27)
    osekstd['values'] = ("FALSE", "TRUE")
    osekstd.current()
    osekstd.grid(row=7, column=2)

    # POSTTASKHOOK - Label + Combo-box
    label = tk.Label(tab, text="POSTTASKHOOK")
    label.grid(row=8, column=1, sticky="w")
    osekstd = ttk.Combobox(tab, width=27)
    osekstd['values'] = ("FALSE", "TRUE")
    osekstd.current()
    osekstd.grid(row=8, column=2)

    # USEGETSERVICEID - Label + Combo-box
    label = tk.Label(tab, text="USEGETSERVICEID")
    label.grid(row=9, column=1, sticky="w")
    osekstd = ttk.Combobox(tab, width=27)
    osekstd['values'] = ("FALSE", "TRUE")
    osekstd.current()
    osekstd.grid(row=9, column=2)

    # USEPARAMETERACCESS - Label + Combo-box
    label = tk.Label(tab, text="USEPARAMETERACCESS ")
    label.grid(row=10, column=1, sticky="w")
    osekstd = ttk.Combobox(tab, width=27)
    osekstd['values'] = ("FALSE", "TRUE")
    osekstd.current()
    osekstd.grid(row=10, column=2)

    # USERESSCHEDULER - Label + Combo-box
    label = tk.Label(tab, text="USERESSCHEDULER")
    label.grid(row=11, column=1, sticky="w")
    osekstd = ttk.Combobox(tab, width=27)
    osekstd['values'] = ("FALSE", "TRUE")
    osekstd.current()
    osekstd.grid(row=11, column=2)

    # OS_STACK_SIZE - Label + Edit-box
    label = tk.Label(tab, text="OS STACK SIZE")
    label.grid(row=12, column=1, sticky="w")
    textb = tk.Entry(tab, text="Entry", width=30)
    textb.grid(row=12, column=2)
    
    # OS_CTX_SAVE_SZ - Label + Edit-box
    label = tk.Label(tab, text="OS CONTEXT SAVE SIZE")
    label.grid(row=13, column=1, sticky="w")
    textb = tk.Entry(tab, text="Entry", width=30)
    textb.grid(row=13, column=2)

    # IRQ_STACK_SIZE - Label + Edit-box
    label = tk.Label(tab, text="IRQ STACK SIZE")
    label.grid(row=14, column=1, sticky="w")
    textb = tk.Entry(tab, text="Entry", width=30)
    textb.grid(row=14, column=2)