import os
import sys


# Let us use the System Generator functions to parse OIL and Generate code
sys.path.insert(0, os.getcwd()+"/tools/scripts")
import scripts.System_Generator as sg

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



# Globals
MainWindow = None
OIL_FileName = None
AppTitle = "OSEK Builder"
RootView = None
OsTab = AmTab = CtrTab = MsgTab = ResTab = TskTab = None


# Functions

def about():
    messagebox.showinfo("OSEK Builder", "This tool is developed to replace the OSEK-Builder.xlsx and to set path for AUTOSAR development")


    
def show_os_config(view):
    global MainWindow, OsTab, AmTab, CtrTab, MsgTab, ResTab, TskTab

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
    
    MainWindow.add(os_tab, text ='OS Configs')
    MainWindow.add(am_tab, text =' AppModes ')
    MainWindow.add(cr_tab, text =' Counters ')
    MainWindow.add(ms_tab, text =' Messages ')
    MainWindow.add(rs_tab, text =' Resources ')
    MainWindow.add(tk_tab, text ='   Tasks   ')
    MainWindow.add(al_tab, text ='  Alarms  ')
    MainWindow.pack(expand = 1, fill ="both")
    
    if OsTab != None:
        del OsTab
    OsTab = gui_os_tab.OsTab(sg.OS_Cfgs)
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
    TskTab = gui_tk_tab.TaskTab(sg.Tasks, AmTab)
    TskTab.draw(tk_tab)
    

def open_file():
    global OIL_FileName, AppTitle
    OIL_FileName = filedialog.askopenfilename(initialdir=os.getcwd()+"/tools/oil-files")
    if RootView != None:
        RootView.title(AppTitle + " [" + str(OIL_FileName).split("/")[-1] +"]")
    sg.parse(OIL_FileName)
    show_os_config(RootView)

 
###############################################################################
# Fuction: add_menus
# args: rv - root view
#    
def add_menus(rv):
    menubar = tk.Menu(rv, background='#ff8000', foreground='black', activebackground='white', activeforeground='black')  
    file = tk.Menu(menubar, tearoff=0)  
    file.add_command(label="Open OIL file", command=open_file)  
    file.add_separator()  
    file.add_command(label="Exit", command=rv.quit)  
    menubar.add_cascade(label="File", menu=file)  

    view = tk.Menu(menubar, tearoff=0)  
    view.add_command(label="OS Config", command=lambda: show_os_config(rv))  
    menubar.add_cascade(label="View", menu=view)  

    gen = tk.Menu(menubar, tearoff=0)  
    gen.add_command(label="Generate OIL file")  
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