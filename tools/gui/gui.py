import tkinter as tk
from tkinter import messagebox
from tkinter import ttk

import gui_os.os_tab as gui_os_tab
import gui_os.am_tab as gui_am_tab


def about():
    messagebox.showinfo("OSEK Builder", "This tool is developed to replace the OSEK-Builder.xlsx and to set path for AUTOSAR development")
    
    
def add_menus(rv):
    menubar = tk.Menu(rv, background='#ff8000', foreground='black', activebackground='white', activeforeground='black')  
    file = tk.Menu(menubar, tearoff=0, background='#ffcc99', foreground='black')  
    #file.add_separator()  
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
    


    

def show_os_config(view):
    tabControl = ttk.Notebook(view)
    
    os_tab = ttk.Frame(tabControl)
    am_tab = ttk.Frame(tabControl)
    
    tabControl.add(os_tab, text ='OS Configs')
    tabControl.add(am_tab, text ='AppModes')
    tabControl.pack(expand = 1, fill ="both")
    
    
    gui_os_tab.draw_os_tab(os_tab)
    gui_am_tab.draw_app_mode_tab(am_tab)
        
    
def main():
    # Create the main window
    root_view = tk.Tk()
    root_view.title("OSEK Builder")
    add_menus(root_view)
    root_view.state('zoomed')


    # Run forever!
    root_view.mainloop()


if __name__ == '__main__':
    main()