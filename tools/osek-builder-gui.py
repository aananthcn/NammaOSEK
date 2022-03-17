import os
import sys


# Let us use the System Generator functions to parse OIL and Generate code
sys.path.insert(0, os.getcwd()+"/tools/scripts")
import scripts.System_Generator as sg
import scripts.oil as oil
import scripts.arxml.main as arxml


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
MenuBar = None
FileMenu = None
OIL_FileName = None
ArXml_FileName = None
AppTitle = "FreeOSEK-AUTOSAR Builder"
RootView = None
CurTab = OsTab = AmTab = CtrTab = MsgTab = ResTab = TskTab = AlmTab = IsrTab = None


# Functions

def about():
    messagebox.showinfo("OSEK Builder", "This tool is developed to replace the OSEK-Builder.xlsx and to set path for AUTOSAR development")



def show_os_tab_switch(event):
    global MainWindow, CurTab, OsTab, AmTab, CtrTab, MsgTab, ResTab, TskTab, AlmTab, IsrTab 
    if MainWindow.tab(MainWindow.select(), "text").strip() == "OS Configs":
        CurTab = OsTab
        CurTab.backup_data()  # take the lastest stack size updates from Task tab.
        CurTab.update()
    if MainWindow.tab(MainWindow.select(), "text").strip() == "AppModes":
        CurTab = AmTab
    if MainWindow.tab(MainWindow.select(), "text").strip() == "Counters":
        CurTab = CtrTab
    if MainWindow.tab(MainWindow.select(), "text").strip() == "Messages":
        CurTab = MsgTab
    if MainWindow.tab(MainWindow.select(), "text").strip() == "Resources":
        CurTab = ResTab
    if MainWindow.tab(MainWindow.select(), "text").strip() == "Tasks":
        CurTab = TskTab
    if MainWindow.tab(MainWindow.select(), "text").strip() == "Alarms":
        CurTab = AlmTab
        CurTab.update()
    if MainWindow.tab(MainWindow.select(), "text").strip() == "ISRs":
        CurTab = IsrTab


    
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
    


def new_file():
    global RootView
    global OsTab, AmTab, CtrTab, MsgTab, ResTab, TskTab, AlmTab, IsrTab

    sg.sg_reset()
    show_os_config(RootView)
    FileMenu.entryconfig("Export to OIL", state="normal")
    FileMenu.entryconfig("Export to ARXML", state="normal")



def open_oil_file(fpath):
    global RootView, OIL_FileName, AppTitle

    init_dir = os.getcwd()
    if os.path.exists(os.getcwd()+"/output/oil-files"):
        init_dir = os.getcwd()+"/output/oil-files"

    if fpath == None:
        OIL_FileName = filedialog.askopenfilename(initialdir=init_dir)
    else:
        OIL_FileName = fpath

    if RootView != None:
        RootView.title(AppTitle + " [" + str(OIL_FileName).split("/")[-1] +"]")

    # Make System Generator to parse, so that we can use the content in GUI.
    sg.parse(OIL_FileName)
    show_os_config(RootView)
    FileMenu.entryconfig("Export to ARXML", state="normal")
    FileMenu.entryconfig("Export to OIL", state="normal")



def save_oil_file():
    global OsTab, AmTab, CtrTab, MsgTab, ResTab, TskTab, AlmTab, IsrTab

    file_exts = [('Oil Files', '*.oil')]
    saved_filename = filedialog.asksaveasfile(initialdir=os.getcwd()+"/output/oil-files", filetypes = file_exts, defaultextension = file_exts)
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


def generate_code():
    if 0 == sg.generate_code():
        messagebox.showinfo("OSEK Builder", "Code Generated Successfully!")
    else:
        messagebox.showinfo("OSEK Builder", "Code Generation Failed!")



def arxml_export():
    global CurTab

    file_exts = [('ARXML Files', '*.arxml')]
    saved_filename = filedialog.asksaveasfile(initialdir=os.getcwd()+"/output/arxml", filetypes = file_exts, defaultextension = file_exts)
    if saved_filename == None:
        messagebox.showinfo("OSEK Builder", "File to save is not done correctly, saving aborted!")
        return

    RootView.title(AppTitle + " [" + str(saved_filename.name).split("/")[-1] +"]")
    CurTab.backup_data()
    arxml.export_arxml(saved_filename.name)



def open_arxml_file(fpath):
    global RootView, ArXml_FileName, AppTitle

    init_dir = os.getcwd()
    if os.path.exists(os.getcwd()+"/output/arxml"):
        init_dir = os.getcwd()+"/output/arxml"

    if fpath == None:
        ArXml_FileName = filedialog.askopenfilename(initialdir=init_dir)
    else:
        ArXml_FileName = fpath

    if RootView != None:
        RootView.title(AppTitle + " [" + str(ArXml_FileName).split("/")[-1] +"]")

    # Import/Parse ARXML file, so that we can use the content in GUI.
    arxml.import_arxml(ArXml_FileName)
    show_os_config(RootView)
    FileMenu.entryconfig("Export to ARXML", state="normal")
    FileMenu.entryconfig("Export to OIL", state="normal")



###############################################################################
# Fuction: add_menus
# args: rv - root view
#    
def add_menus(rv):
    global MenuBar, FileMenu
    MenuBar = tk.Menu(rv, background='#ff8000', foreground='black', activebackground='white', activeforeground='black')
    FileMenu = tk.Menu(MenuBar, tearoff=0)
    FileMenu.add_command(label="New", command=new_file)
    FileMenu.add_command(label="Open OIL File", command=lambda: open_oil_file(None))
    FileMenu.add_command(label="Open ARXML File", command=lambda: open_arxml_file(None))
    FileMenu.add_separator()
    FileMenu.add_command(label="Export to OIL", command=save_oil_file, state="disabled")
    FileMenu.add_command(label="Export to ARXML", command=arxml_export, state="disabled")
    FileMenu.add_separator()
    FileMenu.add_command(label="Exit", command=rv.quit)
    MenuBar.add_cascade(label="File", menu=FileMenu)

    view = tk.Menu(MenuBar, tearoff=0)
    view.add_command(label="OS Config", command=lambda: show_os_config(rv))
    MenuBar.add_cascade(label="View", menu=view)

    gen = tk.Menu(MenuBar, tearoff=0)
    gen.add_command(label="Generate Source", command=generate_code)
    MenuBar.add_cascade(label="Generate", menu=gen)

    help = tk.Menu(MenuBar, tearoff=0)
    help.add_command(label="About", command=about)
    MenuBar.add_cascade(label="Help", menu=help)
    
    rv.config(menu=MenuBar)



def textBox():
    print(textb.get())


def init_view_setup(fpath, ftype):
    if ftype == None or fpath == None:
        return
    elif ftype == "oil":
        open_oil_file(fpath)
    elif ftype == "arxml":
        open_arxml_file(fpath)
    else:
        print("Unsupported filetype argument provided!")



def main(fpath, ftype):
    global RootView, AppTitle
    
    # Create the main window
    RootView = tk.Tk()
    RootView.title(AppTitle + " [uninitialized]")
    add_menus(RootView)
    RootView.attributes('-zoomed', True)

    # setup init view
    init_view_setup(fpath, ftype)

    # Run forever!
    RootView.mainloop()


#
# Arguments to osek-builder-gui.py if invoked from command-line
#
# -f: "file name with path to open"
# -t: filetype ["oil", "arxml"]
#
if __name__ == '__main__':
    filepath = None
    filetype = None

    # collect the arguments if it is passed as arguments
    if "-f" in sys.argv:
        filepath = sys.argv[sys.argv.index("-f") + 1]
        filepath = os.path.abspath(filepath)
        filepath = filepath.replace(os.sep, '/')
        # set source code generation path
        srcpath = filepath.split("FreeOSEK")[0]+"FreeOSEK/tools/src"
        sg.set_source_file_path(srcpath)
    if "-t" in sys.argv:
        filetype = sys.argv[sys.argv.index("-t") + 1]
    
    # let us start the GUI
    main(fpath=filepath, ftype=filetype)