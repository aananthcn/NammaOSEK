import os
import sys


# Let us use the System Generator functions to parse OIL and Generate code
sys.path.insert(0, os.getcwd()+"/tools/scripts")
import scripts.System_Generator as sg
import scripts.oil as oil

import tkinter as tk
from tkinter import messagebox
from tkinter import ttk
from tkinter import filedialog

import gui.os.os_tab as gui_os_tab
import gui.os.am_tab as gui_am_tab
import gui.os.cnt_tab as gui_cr_tab
import gui.os.msg_tab as gui_ms_tab
import gui.os.res_tab as gui_rs_tab
import gui.os.tsk_tab as gui_tk_tab
import gui.os.alm_tab as gui_al_tab
import gui.os.isr_tab as gui_ir_tab



# Globals
MainWindow = None
OIL_FileName = None
AppTitle = "OSEK Builder"
RootView = None
OsTab = AmTab = CtrTab = MsgTab = ResTab = TskTab = AlmTab = IsrTab = None


# Functions

def about():
    messagebox.showinfo("OSEK Builder", "This tool is developed to replace the OSEK-Builder.xlsx and to set path for AUTOSAR development")


def show_os_tab_switch(event):
    global MainWindow, AlmTab, OsTab, TskTab
    if MainWindow.tab(MainWindow.select(), "text").strip() == "Alarms":
        AlmTab.update()
    if MainWindow.tab(MainWindow.select(), "text").strip() == "OS Configs":
        TskTab.backup_data()  # take the lastest stack size updates from Task tab.
        OsTab.update()

    
def show_os_config(view):
    global MainWindow, OsTab, AmTab, CtrTab, MsgTab, ResTab, TskTab, AlmTab, IsrTab

    if MainWindow != None:
        for widget in MainWindow.winfo_children():
            widget.destroy()
        MainWindow.destroy()
    MainWindow = ttk.Notebook(view)
    
    os_tab = ttk.Frame(MainWindow)
    am_tab = ttk.Frame(MainWindow)
    cr_tab = ttk.Frame(MainWindow)
    ms_tab = ttk.Frame(MainWindow)
    rs_tab = ttk.Frame(MainWindow)
    tk_tab = ttk.Frame(MainWindow)
    al_tab = ttk.Frame(MainWindow)
    ir_tab = ttk.Frame(MainWindow)
    
    MainWindow.add(os_tab, text ='OS Configs')
    MainWindow.add(am_tab, text =' AppModes ')
    MainWindow.add(cr_tab, text =' Counters ')
    MainWindow.add(ms_tab, text =' Messages ')
    MainWindow.add(rs_tab, text =' Resources ')
    MainWindow.add(tk_tab, text ='   Tasks   ')
    MainWindow.add(al_tab, text ='  Alarms  ')
    MainWindow.add(ir_tab, text ='   ISRs   ')
    MainWindow.pack(expand = 1, fill ="both")
    
    if OsTab != None:
        del OsTab
    OsTab = gui_os_tab.OsTab(sg.OS_Cfgs, sg.Tasks)
    OsTab.draw(os_tab)

    if AmTab != None:
        del AmTab
    AmTab = gui_am_tab.AmTab(sg.AppModes)
    AmTab.draw(am_tab)
    
    if CtrTab != None:
        del CtrTab
    CtrTab = gui_cr_tab.CounterTab(sg.Counters)
    CtrTab.draw(cr_tab)

    if MsgTab != None:
        del MsgTab
    MsgTab = gui_ms_tab.MessageTab(sg.Tasks)
    MsgTab.draw(ms_tab)

    if ResTab != None:
        del ResTab
    ResTab = gui_rs_tab.ResourceTab(sg.Tasks)
    ResTab.draw(rs_tab)

    if TskTab != None:
        del TskTab
    TskTab = gui_tk_tab.TaskTab(sg.Tasks, AmTab, ResTab, MsgTab)
    TskTab.draw(tk_tab)
    
    if AlmTab != None:
        del AlmTab
    AlmTab = gui_al_tab.AlarmTab(sg.Alarms, TskTab, AmTab, CtrTab)
    AlmTab.draw(al_tab)

    if IsrTab != None:
        del IsrTab
    IsrTab = gui_ir_tab.IsrTab(sg.ISRs, ResTab, MsgTab)
    IsrTab.draw(ir_tab)

    MainWindow.bind("<<NotebookTabChanged>>", show_os_tab_switch)
    

def open_file():
    global OIL_FileName, AppTitle

    init_dir = os.getcwd()
    if os.path.exists(os.getcwd()+"/tools/oil-files"):
        init_dir = os.getcwd()+"/tools/oil-files"

    OIL_FileName = filedialog.askopenfilename(initialdir=init_dir)
    if RootView != None:
        RootView.title(AppTitle + " [" + str(OIL_FileName).split("/")[-1] +"]")

    # Make System Generator to parse, so that we can use the content in GUI.
    sg.parse(OIL_FileName)
    show_os_config(RootView)


def save_oil_file():
    global OsTab, AmTab, CtrTab, MsgTab, ResTab, TskTab, AlmTab, IsrTab

    file_exts = [('Oil Files', '*.oil')]
    saved_filename = filedialog.asksaveasfile(initialdir=os.getcwd()+"/tools/oil-files", filetypes = file_exts, defaultextension = file_exts)
    if saved_filename == None:
        messagebox.showinfo("OSEK Builder", "File to save is not done correctly, saving aborted!")
        return

    RootView.title(AppTitle + " [" + str(saved_filename.name).split("/")[-1] +"]")

    # Do the stack memory calculation before save
    OsTab.update()

    # Backup GUI strings to System Generator global data
    OsTab.backup_data()
    AmTab.backup_data()
    CtrTab.backup_data()
    MsgTab.backup_data()
    ResTab.backup_data()
    TskTab.backup_data()
    AlmTab.backup_data()
    IsrTab.backup_data()
    oil.save_oil_file(saved_filename.name)


def generate_oil_file():
    if 0 == sg.generate_code():
        messagebox.showinfo("OSEK Builder", "Code Generated Successfully!")
    else:
        messagebox.showinfo("OSEK Builder", "Code Generation Failed!")



 
###############################################################################
# Fuction: add_menus
# args: rv - root view
#    
def add_menus(rv):
    menubar = tk.Menu(rv, background='#ff8000', foreground='black', activebackground='white', activeforeground='black')
    file = tk.Menu(menubar, tearoff=0)
    file.add_command(label="Open OIL file", command=open_file)
    file.add_command(label="Save As", command=save_oil_file)
    file.add_separator()
    file.add_command(label="Exit", command=rv.quit)
    menubar.add_cascade(label="File", menu=file)

    view = tk.Menu(menubar, tearoff=0)
    view.add_command(label="OS Config", command=lambda: show_os_config(rv))
    menubar.add_cascade(label="View", menu=view)

    gen = tk.Menu(menubar, tearoff=0)
    gen.add_command(label="Generate OIL file", command=generate_oil_file)
    menubar.add_cascade(label="Generate", menu=gen)

    help = tk.Menu(menubar, tearoff=0)
    help.add_command(label="About", command=about)
    menubar.add_cascade(label="Help", menu=help)
    
    rv.config(menu=menubar)



def textBox():
    print(textb.get())

    

def main():
    global RootView, AppTitle
    
    # Create the main window
    RootView = tk.Tk()
    RootView.title(AppTitle + " [uninitialized]")
    add_menus(RootView)
    RootView.state('zoomed')


    # Run forever!
    RootView.mainloop()


if __name__ == '__main__':
    main()