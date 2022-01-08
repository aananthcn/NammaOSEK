import tkinter as tk
from tkinter import ttk


class TaskStr:
    id = 0
    name = None
    prio = None
    schedule = None
    activation = None
    stack_sz = None

    def __init__(self, id):
        self.id = id
        self.name = tk.StringVar()
        self.prio = tk.StringVar()
        self.schedule = tk.StringVar()
        self.activation = tk.StringVar()
        self.stack_sz = tk.StringVar()

    def __del__(self):
        del self.name
        del self.prio
        del self.schedule
        del self.activation
        del self.stack_sz


class TaskTab:
    n_tasks = 1
    max_tasks = 1024
    n_tasks_str = None
    tasks_str = []
    events = []
    tasks = None
    HeaderObjs = 12 #Objects / widgets that are part of the header and shouldn't be destroyed
    HeaderSize = 3
    prf = None  # Parent Frame
    cvf = None  # Canvas Frame
    cv  = None  # Canvas
    sb  = None  # Scrollbar
    mnf = None  # Main Frame - where the widgets are scrolled

    def __init__(self, tasks):
        self.tasks = tasks
        self.n_tasks = len(self.tasks)
        self.n_tasks_str = tk.StringVar()
        for i in range(self.n_tasks):
            self.tasks_str.insert(i, TaskStr(i))

    def __del__(self):
        del self.n_tasks_str
        del self.tasks_str[:]

    def update_tasks(self, mstr):
        self.n_tasks = int(mstr.get())
        print("Update tasks: "+ str(self.n_tasks))        
        for i, item in enumerate(self.mnf.winfo_children()):
            if i >= self.HeaderObjs:
                item.destroy()
        self.update()

    def draw(self, tab):
        tab.grid_rowconfigure(0, weight=1)
        tab.columnconfigure(0, weight=1)
        self.prf = tk.Frame(tab)
        self.prf.grid(sticky="news")

        # Create a frame for the canvas with non-zero row&column weights
        self.cvf = tk.Frame(self.prf)
        self.cvf.grid(row=2, column=0, pady=(5, 0), sticky='nw')
        self.cvf.grid_rowconfigure(0, weight=1)
        self.cvf.grid_columnconfigure(0, weight=1)

        # Set grid_propagate to False to allow canvas frame resizing later
        self.cvf.grid_propagate(False)

        # Add a canvas in that frame
        self.cv = tk.Canvas(self.cvf)
        self.cv.grid(row=0, column=0, sticky="news")

        # Link a scrollbar to the canvas
        self.sb = tk.Scrollbar(self.cvf, orient="vertical", command=self.cv.yview)
        self.sb.grid(row=0, column=1, sticky='ns')
        self.cv.configure(yscrollcommand=self.sb.set)

        # Create a frame to draw task table
        self.mnf = tk.Frame(self.cv)
        self.cv.create_window((0, 0), window=self.mnf, anchor='nw')

        #Number of modes - Label + Spinbox
        label = tk.Label(self.mnf, text="No. of Tasks:")
        label.grid(row=0, column=0, sticky="w")
        spinb = tk.Spinbox(self.mnf, width=10, textvariable=self.n_tasks_str, command=lambda: self.update_tasks(self.n_tasks_str),
                    values=tuple(range(1,self.max_tasks+1)))
        self.n_tasks_str.set(self.n_tasks)
        spinb.grid(row=0, column=1, sticky="w")

        # Update buttons frames idle tasks to let tkinter calculate buttons sizes
        self.mnf.update_idletasks()
        # Resize the main frame to show contents for FULL SCREEN (Todo: scroll bars won't work in reduced size window)
        canvas_w = tab.winfo_screenwidth()-self.sb.winfo_width()
        canvas_h = tab.winfo_screenheight()-(spinb.winfo_height()*6)
        # print("screen: "+str(tab.winfo_screenwidth())+" x "+str(tab.winfo_screenheight()))
        # print("canvas: "+str(canvas_w)+" x "+str(canvas_h))
        self.cvf.config(width=canvas_w, height=canvas_h)

        # Table heading
        label = tk.Label(self.mnf, text=" ")
        label.grid(row=2, column=0, sticky="w")
        label = tk.Label(self.mnf, text="Task Name")
        label.grid(row=2, column=1, sticky="w")
        label = tk.Label(self.mnf, text="PRIORITY")
        label.grid(row=2, column=2, sticky="we")
        label = tk.Label(self.mnf, text="PREMPTION")
        label.grid(row=2, column=3, sticky="we")
        label = tk.Label(self.mnf, text="ACTIVATION")
        label.grid(row=2, column=4, sticky="we")
        label = tk.Label(self.mnf, text="AUTOSTART[]")
        label.grid(row=2, column=5, sticky="we")
        label = tk.Label(self.mnf, text="EVENT[]")
        label.grid(row=2, column=6, sticky="we")
        label = tk.Label(self.mnf, text="RESOURCE[]")
        label.grid(row=2, column=7, sticky="we")
        label = tk.Label(self.mnf, text="MESSAGE[]")
        label.grid(row=2, column=8, sticky="we")
        label = tk.Label(self.mnf, text="Stack Size")
        label.grid(row=2, column=9, sticky="we")

        self.update()


    def update(self):
        # Tune memory allocations based on number of rows or boxes
        n_tasks_str = len(self.tasks_str)
        if self.n_tasks > n_tasks_str:
            for i in range(self.n_tasks - n_tasks_str):
                self.tasks_str.insert(len(self.tasks_str), TaskStr(n_tasks_str+i))
                self.tasks.insert(len(self.tasks), self.tasks[-1])
        elif n_tasks_str > self.n_tasks:
            for i in range(n_tasks_str - self.n_tasks):
                del self.tasks_str[-1]
                del self.tasks[-1]

        print("n_tasks_str = "+ str(n_tasks_str) + ", n_tasks = " + str(self.n_tasks))
        # Draw new objects
        for i in range(0, self.n_tasks):
            label = tk.Label(self.mnf, text="Task "+str(i)+": ")
            label.grid(row=self.HeaderSize+i, column=0, sticky="e")
            
            # Task Name
            entry = tk.Entry(self.mnf, width=30, textvariable=self.tasks_str[i].name)
            self.tasks_str[i].name.set(self.tasks[i]["Task Name"])
            entry.grid(row=self.HeaderSize+i, column=1)

            # PRIORITY
            entry = tk.Entry(self.mnf, width=10, textvariable=self.tasks_str[i].prio, justify='center')
            self.tasks_str[i].prio.set(self.tasks[i]["PRIORITY"])
            entry.grid(row=self.HeaderSize+i, column=2)

            # SCHEDULE
            cmbsel = ttk.Combobox(self.mnf, width=8, textvariable=self.tasks_str[i].schedule, state="readonly")
            cmbsel['values'] = ("NON", "FULL")
            self.tasks_str[i].schedule.set(self.tasks[i]["SCHEDULE"])
            cmbsel.current()
            cmbsel.grid(row=self.HeaderSize+i, column=3)

            # ACTIVATION
            entry = tk.Entry(self.mnf, width=11, textvariable=self.tasks_str[i].activation, justify='center')
            self.tasks_str[i].activation.set(self.tasks[i]["ACTIVATION"])
            entry.grid(row=self.HeaderSize+i, column=4)

            # AUTOSTART[]
            select = tk.Button(self.mnf, width=10, text="SELECT")
            select.grid(row=self.HeaderSize+i, column=5)

            # EVENT[]
            select = tk.Button(self.mnf, width=10, text="SELECT")
            select.grid(row=self.HeaderSize+i, column=6)

            # RESOURCE[]
            select = tk.Button(self.mnf, width=10, text="SELECT")
            select.grid(row=self.HeaderSize+i, column=7)

            # MESSAGE[]
            select = tk.Button(self.mnf, width=10, text="SELECT")
            select.grid(row=self.HeaderSize+i, column=8)
            
            # STACK_SIZE
            entry = tk.Entry(self.mnf, width=11, textvariable=self.tasks_str[i].stack_sz, justify='center')
            self.tasks_str[i].stack_sz.set(self.tasks[i]["STACK_SIZE"])
            entry.grid(row=self.HeaderSize+i, column=9)

        # Set the self.cv scrolling region
        self.cv.config(scrollregion=self.cv.bbox("all"))

    def extract_event(self, task):
        if "EVENT" in task:
            for evt in task["EVENT"]:
                if evt not in self.tasks:
                    self.events.append(evt)
        return task