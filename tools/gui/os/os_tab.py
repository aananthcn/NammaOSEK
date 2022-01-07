import tkinter as tk
from tkinter import ttk

N_StrVar = 15
OS_StrVar = []

def draw_os_tab(tab, oscfg):
    for i in range(N_StrVar):
        OS_StrVar.insert(i, tk.StringVar())
        
    # 1) CPU / SoC - Label + Edit-box
    label = tk.Label(tab, text="CPU / SoC name")
    label.grid(row=1, column=1, sticky="w")
    textb = tk.Entry(tab,text="Entry", width=30, textvariable=OS_StrVar[0])
    OS_StrVar[0].set(oscfg["CPU"])
    textb.grid(row=1, column=2)
 
    # 2) OS Name - Label + Edit-box
    label = tk.Label(tab, text="Image Name")
    label.grid(row=2, column=1, sticky="w")
    textb = tk.Entry(tab, text="Entry", width=30, textvariable=OS_StrVar[1])
    OS_StrVar[1].set(oscfg["OS"])
    textb.grid(row=2, column=2)
 
    # 3) OSEK Standard - Label + Combo-box
    label = tk.Label(tab, text="OSEK Standard")
    label.grid(row=3, column=1, sticky="w")
    osekstd = ttk.Combobox(tab, width=27, textvariable=OS_StrVar[2], state="readonly")
    osekstd['values'] = ("STANDARD", "EXTENDED")
    OS_StrVar[2].set(oscfg["STATUS"])
    osekstd.current()
    osekstd.grid(row=3, column=2)

    # 4) STARTUPHOOK - Label + Combo-box
    label = tk.Label(tab, text="STARTUPHOOK")
    label.grid(row=4, column=1, sticky="w")
    osekstd = ttk.Combobox(tab, width=27, textvariable=OS_StrVar[3], state="readonly")
    osekstd['values'] = ("FALSE", "TRUE")
    OS_StrVar[3].set(oscfg["STARTUPHOOK"])
    osekstd.current()
    osekstd.grid(row=4, column=2)
 
    # 5) ERRORHOOK - Label + Combo-box
    label = tk.Label(tab, text="ERRORHOOK")
    label.grid(row=5, column=1, sticky="w")
    osekstd = ttk.Combobox(tab, width=27, textvariable=OS_StrVar[4], state="readonly")
    osekstd['values'] = ("FALSE", "TRUE")
    OS_StrVar[4].set(oscfg["ERRORHOOK"])
    osekstd.current()
    osekstd.grid(row=5, column=2)

    # 6) SHUTDOWNHOOK - Label + Combo-box
    label = tk.Label(tab, text="SHUTDOWNHOOK")
    label.grid(row=6, column=1, sticky="w")
    osekstd = ttk.Combobox(tab, width=27, textvariable=OS_StrVar[5], state="readonly")
    osekstd['values'] = ("FALSE", "TRUE")
    OS_StrVar[5].set(oscfg["SHUTDOWNHOOK"])
    osekstd.current()
    osekstd.grid(row=6, column=2)

    # 7) PRETASKHOOK - Label + Combo-box
    label = tk.Label(tab, text="PRETASKHOOK")
    label.grid(row=7, column=1, sticky="w")
    osekstd = ttk.Combobox(tab, width=27, textvariable=OS_StrVar[6], state="readonly")
    osekstd['values'] = ("FALSE", "TRUE")
    OS_StrVar[6].set(oscfg["PRETASKHOOK"])
    osekstd.current()
    osekstd.grid(row=7, column=2)

    # 8) POSTTASKHOOK - Label + Combo-box
    label = tk.Label(tab, text="POSTTASKHOOK")
    label.grid(row=8, column=1, sticky="w")
    osekstd = ttk.Combobox(tab, width=27, textvariable=OS_StrVar[7], state="readonly")
    osekstd['values'] = ("FALSE", "TRUE")
    OS_StrVar[7].set(oscfg["POSTTASKHOOK"])
    osekstd.current()
    osekstd.grid(row=8, column=2)

    # 9) USEGETSERVICEID - Label + Combo-box
    label = tk.Label(tab, text="USEGETSERVICEID")
    label.grid(row=9, column=1, sticky="w")
    osekstd = ttk.Combobox(tab, width=27, textvariable=OS_StrVar[8], state="readonly")
    osekstd['values'] = ("FALSE", "TRUE")
    OS_StrVar[8].set(oscfg["USEGETSERVICEID"])
    osekstd.current()
    osekstd.grid(row=9, column=2)

    # 10) USEPARAMETERACCESS - Label + Combo-box
    label = tk.Label(tab, text="USEPARAMETERACCESS ")
    label.grid(row=10, column=1, sticky="w")
    osekstd = ttk.Combobox(tab, width=27, textvariable=OS_StrVar[9], state="readonly")
    osekstd['values'] = ("FALSE", "TRUE")
    OS_StrVar[9].set(oscfg["USEPARAMETERACCESS"])
    osekstd.current()
    osekstd.grid(row=10, column=2)

    # 11) USERESSCHEDULER - Label + Combo-box
    label = tk.Label(tab, text="USERESSCHEDULER")
    label.grid(row=11, column=1, sticky="w")
    osekstd = ttk.Combobox(tab, width=27, textvariable=OS_StrVar[10], state="readonly")
    osekstd['values'] = ("FALSE", "TRUE")
    OS_StrVar[10].set(oscfg["USERESSCHEDULER"])
    osekstd.current()
    osekstd.grid(row=11, column=2)

    # 12) OS_STACK_SIZE - Label + Edit-box
    label = tk.Label(tab, text="OS STACK SIZE")
    label.grid(row=12, column=1, sticky="w")
    textb = tk.Entry(tab, text="Entry", width=30, textvariable=OS_StrVar[11])
    OS_StrVar[11].set(oscfg["OS_STACK_SIZE"])
    textb.grid(row=12, column=2)
    
    # 13) OS_CTX_SAVE_SZ - Label + Edit-box
    label = tk.Label(tab, text="OS CONTEXT SAVE SIZE")
    label.grid(row=13, column=1, sticky="w")
    textb = tk.Entry(tab, text="Entry", width=30, textvariable=OS_StrVar[12])
    OS_StrVar[12].set(oscfg["OS_CTX_SAVE_SZ"])
    textb.grid(row=13, column=2)

    # 14) IRQ_STACK_SIZE - Label + Edit-box
    label = tk.Label(tab, text="IRQ STACK SIZE")
    label.grid(row=14, column=1, sticky="w")
    textb = tk.Entry(tab, text="Entry", width=30, textvariable=OS_StrVar[13])
    OS_StrVar[13].set(oscfg["IRQ_STACK_SIZE"])
    textb.grid(row=14, column=2)
    
    # 15) TASK_STACK_SIZE - Label + Edit-box
    label = tk.Label(tab, text="TASK STACK SIZE")
    label.grid(row=14, column=1, sticky="w")
    textb = tk.Entry(tab, text="Entry", width=30, textvariable=OS_StrVar[14], state="readonly")
    OS_StrVar[14].set(oscfg["TASK_STACK_SIZE"])
    textb.grid(row=14, column=2)