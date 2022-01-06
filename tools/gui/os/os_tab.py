import tkinter as tk
from tkinter import ttk

N_Entries = 15
OS_Entries = []

def draw_os_tab(tab, OS_Cfgs):
    for i in range(N_Entries):
        OS_Entries.insert(i, tk.StringVar())
        
    print(OS_Cfgs)
    
    # 1) CPU / SoC - Label + Edit-box
    label = tk.Label(tab, text="CPU / SoC name")
    label.grid(row=1, column=1, sticky="w")
    textb = tk.Entry(tab,text="Entry", width=30, textvariable=OS_Entries[0])
    OS_Entries[0].set(OS_Cfgs["CPU"])
    textb.grid(row=1, column=2)
 
    # 2) OS Name - Label + Edit-box
    label = tk.Label(tab, text="Image Name")
    label.grid(row=2, column=1, sticky="w")
    textb = tk.Entry(tab, text="Entry", width=30, textvariable=OS_Entries[1])
    OS_Entries[1].set(OS_Cfgs["OS"])
    textb.grid(row=2, column=2)
 
    # 3) OSEK Standard - Label + Combo-box
    label = tk.Label(tab, text="OSEK Standard")
    label.grid(row=3, column=1, sticky="w")
    osekstd = ttk.Combobox(tab, width=27, textvariable=OS_Entries[2])
    osekstd['values'] = ("STANDARD", "EXTENDED")
    OS_Entries[2].set(OS_Cfgs["STATUS"])
    osekstd.current()
    osekstd.grid(row=3, column=2)

    # 4) STARTUPHOOK - Label + Combo-box
    label = tk.Label(tab, text="STARTUPHOOK")
    label.grid(row=4, column=1, sticky="w")
    osekstd = ttk.Combobox(tab, width=27, textvariable=OS_Entries[3])
    osekstd['values'] = ("FALSE", "TRUE")
    OS_Entries[3].set(OS_Cfgs["STARTUPHOOK"])
    osekstd.current()
    osekstd.grid(row=4, column=2)
 
    # 5) ERRORHOOK - Label + Combo-box
    label = tk.Label(tab, text="ERRORHOOK")
    label.grid(row=5, column=1, sticky="w")
    osekstd = ttk.Combobox(tab, width=27, textvariable=OS_Entries[4])
    osekstd['values'] = ("FALSE", "TRUE")
    OS_Entries[4].set(OS_Cfgs["ERRORHOOK"])
    osekstd.current()
    osekstd.grid(row=5, column=2)

    # 6) SHUTDOWNHOOK - Label + Combo-box
    label = tk.Label(tab, text="SHUTDOWNHOOK")
    label.grid(row=6, column=1, sticky="w")
    osekstd = ttk.Combobox(tab, width=27, textvariable=OS_Entries[5])
    osekstd['values'] = ("FALSE", "TRUE")
    OS_Entries[5].set(OS_Cfgs["SHUTDOWNHOOK"])
    osekstd.current()
    osekstd.grid(row=6, column=2)

    # 7) PRETASKHOOK - Label + Combo-box
    label = tk.Label(tab, text="PRETASKHOOK")
    label.grid(row=7, column=1, sticky="w")
    osekstd = ttk.Combobox(tab, width=27, textvariable=OS_Entries[6])
    osekstd['values'] = ("FALSE", "TRUE")
    OS_Entries[6].set(OS_Cfgs["PRETASKHOOK"])
    osekstd.current()
    osekstd.grid(row=7, column=2)

    # 8) POSTTASKHOOK - Label + Combo-box
    label = tk.Label(tab, text="POSTTASKHOOK")
    label.grid(row=8, column=1, sticky="w")
    osekstd = ttk.Combobox(tab, width=27, textvariable=OS_Entries[7])
    osekstd['values'] = ("FALSE", "TRUE")
    OS_Entries[7].set(OS_Cfgs["POSTTASKHOOK"])
    osekstd.current()
    osekstd.grid(row=8, column=2)

    # 9) USEGETSERVICEID - Label + Combo-box
    label = tk.Label(tab, text="USEGETSERVICEID")
    label.grid(row=9, column=1, sticky="w")
    osekstd = ttk.Combobox(tab, width=27, textvariable=OS_Entries[8])
    osekstd['values'] = ("FALSE", "TRUE")
    OS_Entries[8].set(OS_Cfgs["USEGETSERVICEID"])
    osekstd.current()
    osekstd.grid(row=9, column=2)

    # 10) USEPARAMETERACCESS - Label + Combo-box
    label = tk.Label(tab, text="USEPARAMETERACCESS ")
    label.grid(row=10, column=1, sticky="w")
    osekstd = ttk.Combobox(tab, width=27, textvariable=OS_Entries[9])
    osekstd['values'] = ("FALSE", "TRUE")
    OS_Entries[9].set(OS_Cfgs["USEPARAMETERACCESS"])
    osekstd.current()
    osekstd.grid(row=10, column=2)

    # 11) USERESSCHEDULER - Label + Combo-box
    label = tk.Label(tab, text="USERESSCHEDULER")
    label.grid(row=11, column=1, sticky="w")
    osekstd = ttk.Combobox(tab, width=27, textvariable=OS_Entries[10])
    osekstd['values'] = ("FALSE", "TRUE")
    OS_Entries[10].set(OS_Cfgs["USERESSCHEDULER"])
    osekstd.current()
    osekstd.grid(row=11, column=2)

    # 12) OS_STACK_SIZE - Label + Edit-box
    label = tk.Label(tab, text="OS STACK SIZE")
    label.grid(row=12, column=1, sticky="w")
    textb = tk.Entry(tab, text="Entry", width=30, textvariable=OS_Entries[11])
    OS_Entries[11].set(OS_Cfgs["OS_STACK_SIZE"])
    textb.grid(row=12, column=2)
    
    # 13) OS_CTX_SAVE_SZ - Label + Edit-box
    label = tk.Label(tab, text="OS CONTEXT SAVE SIZE")
    label.grid(row=13, column=1, sticky="w")
    textb = tk.Entry(tab, text="Entry", width=30, textvariable=OS_Entries[12])
    OS_Entries[12].set(OS_Cfgs["OS_CTX_SAVE_SZ"])
    textb.grid(row=13, column=2)

    # 14) IRQ_STACK_SIZE - Label + Edit-box
    label = tk.Label(tab, text="IRQ STACK SIZE")
    label.grid(row=14, column=1, sticky="w")
    textb = tk.Entry(tab, text="Entry", width=30, textvariable=OS_Entries[13])
    OS_Entries[13].set(OS_Cfgs["IRQ_STACK_SIZE"])
    textb.grid(row=14, column=2)
    
    # 15) TASK_STACK_SIZE - Label + Edit-box
    label = tk.Label(tab, text="TASK STACK SIZE")
    label.grid(row=14, column=1, sticky="w")
    textb = tk.Entry(tab, text="Entry", width=30, textvariable=OS_Entries[14])
    OS_Entries[14].set(OS_Cfgs["TASK_STACK_SIZE"])
    textb.grid(row=14, column=2)