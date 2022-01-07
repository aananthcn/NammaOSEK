import tkinter as tk
from tkinter import ttk

class OsTab:
    N_StrVar = 15
    OS_StrVar = []
    OsCfg = None

    def __init__(self, osCfg):
        self.OsCfg = osCfg
        for i in range(self.N_StrVar):
            self.OS_StrVar.insert(i, tk.StringVar())

    def __del__(self):
        for obj in self.OS_StrVar:
            del obj
        self.OsCfg = None

    def draw(self, tab):
        # 1) CPU / SoC - Label + Edit-box
        label = tk.Label(tab, text="CPU / SoC name")
        label.grid(row=1, column=1, sticky="w")
        textb = tk.Entry(tab,text="Entry", width=30, textvariable=self.OS_StrVar[0])
        self.OS_StrVar[0].set(self.OsCfg["CPU"])
        textb.grid(row=1, column=2)
    
        # 2) OS Name - Label + Edit-box
        label = tk.Label(tab, text="Image Name")
        label.grid(row=2, column=1, sticky="w")
        textb = tk.Entry(tab, text="Entry", width=30, textvariable=self.OS_StrVar[1])
        self.OS_StrVar[1].set(self.OsCfg["OS"])
        textb.grid(row=2, column=2)
    
        # 3) OSEK Standard - Label + Combo-box
        label = tk.Label(tab, text="OSEK Standard")
        label.grid(row=3, column=1, sticky="w")
        osekstd = ttk.Combobox(tab, width=27, textvariable=self.OS_StrVar[2], state="readonly")
        osekstd['values'] = ("STANDARD", "EXTENDED")
        self.OS_StrVar[2].set(self.OsCfg["STATUS"])
        osekstd.current()
        osekstd.grid(row=3, column=2)

        # 4) STARTUPHOOK - Label + Combo-box
        label = tk.Label(tab, text="STARTUPHOOK")
        label.grid(row=4, column=1, sticky="w")
        osekstd = ttk.Combobox(tab, width=27, textvariable=self.OS_StrVar[3], state="readonly")
        osekstd['values'] = ("FALSE", "TRUE")
        self.OS_StrVar[3].set(self.OsCfg["STARTUPHOOK"])
        osekstd.current()
        osekstd.grid(row=4, column=2)
    
        # 5) ERRORHOOK - Label + Combo-box
        label = tk.Label(tab, text="ERRORHOOK")
        label.grid(row=5, column=1, sticky="w")
        osekstd = ttk.Combobox(tab, width=27, textvariable=self.OS_StrVar[4], state="readonly")
        osekstd['values'] = ("FALSE", "TRUE")
        self.OS_StrVar[4].set(self.OsCfg["ERRORHOOK"])
        osekstd.current()
        osekstd.grid(row=5, column=2)

        # 6) SHUTDOWNHOOK - Label + Combo-box
        label = tk.Label(tab, text="SHUTDOWNHOOK")
        label.grid(row=6, column=1, sticky="w")
        osekstd = ttk.Combobox(tab, width=27, textvariable=self.OS_StrVar[5], state="readonly")
        osekstd['values'] = ("FALSE", "TRUE")
        self.OS_StrVar[5].set(self.OsCfg["SHUTDOWNHOOK"])
        osekstd.current()
        osekstd.grid(row=6, column=2)

        # 7) PRETASKHOOK - Label + Combo-box
        label = tk.Label(tab, text="PRETASKHOOK")
        label.grid(row=7, column=1, sticky="w")
        osekstd = ttk.Combobox(tab, width=27, textvariable=self.OS_StrVar[6], state="readonly")
        osekstd['values'] = ("FALSE", "TRUE")
        self.OS_StrVar[6].set(self.OsCfg["PRETASKHOOK"])
        osekstd.current()
        osekstd.grid(row=7, column=2)

        # 8) POSTTASKHOOK - Label + Combo-box
        label = tk.Label(tab, text="POSTTASKHOOK")
        label.grid(row=8, column=1, sticky="w")
        osekstd = ttk.Combobox(tab, width=27, textvariable=self.OS_StrVar[7], state="readonly")
        osekstd['values'] = ("FALSE", "TRUE")
        self.OS_StrVar[7].set(self.OsCfg["POSTTASKHOOK"])
        osekstd.current()
        osekstd.grid(row=8, column=2)

        # 9) USEGETSERVICEID - Label + Combo-box
        label = tk.Label(tab, text="USEGETSERVICEID")
        label.grid(row=9, column=1, sticky="w")
        osekstd = ttk.Combobox(tab, width=27, textvariable=self.OS_StrVar[8], state="readonly")
        osekstd['values'] = ("FALSE", "TRUE")
        self.OS_StrVar[8].set(self.OsCfg["USEGETSERVICEID"])
        osekstd.current()
        osekstd.grid(row=9, column=2)

        # 10) USEPARAMETERACCESS - Label + Combo-box
        label = tk.Label(tab, text="USEPARAMETERACCESS ")
        label.grid(row=10, column=1, sticky="w")
        osekstd = ttk.Combobox(tab, width=27, textvariable=self.OS_StrVar[9], state="readonly")
        osekstd['values'] = ("FALSE", "TRUE")
        self.OS_StrVar[9].set(self.OsCfg["USEPARAMETERACCESS"])
        osekstd.current()
        osekstd.grid(row=10, column=2)

        # 11) USERESSCHEDULER - Label + Combo-box
        label = tk.Label(tab, text="USERESSCHEDULER")
        label.grid(row=11, column=1, sticky="w")
        osekstd = ttk.Combobox(tab, width=27, textvariable=self.OS_StrVar[10], state="readonly")
        osekstd['values'] = ("FALSE", "TRUE")
        self.OS_StrVar[10].set(self.OsCfg["USERESSCHEDULER"])
        osekstd.current()
        osekstd.grid(row=11, column=2)

        # 12) OS_STACK_SIZE - Label + Edit-box
        label = tk.Label(tab, text="OS STACK SIZE")
        label.grid(row=12, column=1, sticky="w")
        textb = tk.Entry(tab, text="Entry", width=30, textvariable=self.OS_StrVar[11])
        self.OS_StrVar[11].set(self.OsCfg["OS_STACK_SIZE"])
        textb.grid(row=12, column=2)
        
        # 13) OS_CTX_SAVE_SZ - Label + Edit-box
        label = tk.Label(tab, text="OS CONTEXT SAVE SIZE")
        label.grid(row=13, column=1, sticky="w")
        textb = tk.Entry(tab, text="Entry", width=30, textvariable=self.OS_StrVar[12])
        self.OS_StrVar[12].set(self.OsCfg["OS_CTX_SAVE_SZ"])
        textb.grid(row=13, column=2)

        # 14) IRQ_STACK_SIZE - Label + Edit-box
        label = tk.Label(tab, text="IRQ STACK SIZE")
        label.grid(row=14, column=1, sticky="w")
        textb = tk.Entry(tab, text="Entry", width=30, textvariable=self.OS_StrVar[13])
        self.OS_StrVar[13].set(self.OsCfg["IRQ_STACK_SIZE"])
        textb.grid(row=14, column=2)
        
        # 15) TASK_STACK_SIZE - Label + Edit-box
        label = tk.Label(tab, text="TASK STACK SIZE")
        label.grid(row=14, column=1, sticky="w")
        textb = tk.Entry(tab, text="Entry", width=30, textvariable=self.OS_StrVar[14], state="readonly")
        self.OS_StrVar[14].set(self.OsCfg["TASK_STACK_SIZE"])
        textb.grid(row=14, column=2)