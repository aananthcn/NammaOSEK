# This tool takes "OSEK-Builder.xlsx" as input and generated *.oil files
# as output.

import sys
import utils
import os

import colorama
from colorama import Fore, Back, Style

# Global Variables
CntrParams = ["Counter Name", "MINCYCLE", "MAXALLOWEDVALUE", "TICKSPERBASE"]


# Functions
def print_info(text):
    print(Fore.BLUE, "\bInfo:", text, Style.RESET_ALL)


def validate_column(wb, sheetname, col_header):
    col, row = utils.locate_heading_column(col_header, wb, sheetname)
    if col == -1:
        print(Fore.RED + "Error: can't locate \""+col_header+"\" in sheet \"" + sheetname + "\"")
        print(Style.RESET_ALL)
        return None

    return col, row


def parse_task_data(wb, sheetname):
    # list of column titles in TASK tab of OSEX-Builder.xlsx
    taskparams = ["Task Name", "Priority", "Schedule", "Activation", "Autostart",
    "Resource", "Event", "Message"]
    # parameters / columns that has comma separated entries
    specltasks = ["autostart", "resource", "event", "message"]
    # find out all column numbers of all the column titles and add to a list
    param_cols, hrow = utils.locate_cols(wb, sheetname, taskparams)
    if param_cols == None:
        print("Error: Task Column location failed!")
        return None
    
    sheet = wb[sheetname]
    active_rows = len(sheet[param_cols[taskparams[0]]])
    TaskData = [dict() for x in range(hrow, active_rows)]
    task_count = 0
    for i in range(hrow, active_rows):
        row = i + hrow
        for tsk_item in taskparams:
            if tsk_item.lower() in specltasks:
                lvalue = str(sheet[param_cols[tsk_item]+str(row)].value).split(",")
                TaskData[task_count][tsk_item] = lvalue
            else:
                TaskData[task_count][tsk_item] = sheet[param_cols[tsk_item]+str(row)].value
        task_count += 1
    
    #print(TaskData)
    return TaskData


def parse_appmode_data(wb, sheetname):
    attr_col, attr_row = validate_column(wb, sheetname, "APPMODE")
    if attr_col == None:
        print("Error: sheet validation failed ("+ sheetname +")")
        return None

    sheet = wb[sheetname]
    AppModes = []
    active_rows = len(sheet[chr(attr_col)])
    for i in range(attr_row, active_rows):
        row = i + attr_row
        AppModes.append(sheet[chr(attr_col)+str(row)].value.strip())
    
    #print(AppModes)
    return AppModes



def read_os_attribute(sheet, sheetname, attr_col, attr):
    col, row = utils.locate_text_in_column(attr, sheet, attr_col)
    if row == -1:
        print("Error: can't locate \"CPU\" in sheet \"" + sheetname + "\"")
        return
    attr_val = sheet[chr(col+1)+str(row)].value
    if attr_val == True:
        attr_val = "TRUE"
    if attr_val == False:
        attr_val = "FALSE"
    
    return attr_val



def parse_os_data(wb, sheetname):
    attr_col, attr_row = validate_column(wb, sheetname, "Attribute Name")
    if attr_col == None:
        print("Error: sheet validation failed ("+ sheetname +")")
        return None
    sheet = wb[sheetname]
    OsData   = {}

    # read os data
    OsData["cpu"]      = read_os_attribute(sheet, sheetname, attr_col, "CPU")
    OsData["os_name"]  = read_os_attribute(sheet, sheetname, attr_col, "OS NAME")
    OsData["status"]   = read_os_attribute(sheet, sheetname, attr_col, "STATUS")
    OsData["strthk"]   = read_os_attribute(sheet, sheetname, attr_col, "STARTUPHOOK")
    OsData["errhk"]    = read_os_attribute(sheet, sheetname, attr_col, "ERRORHOOK")
    OsData["shdnhk"]   = read_os_attribute(sheet, sheetname, attr_col, "SHUTDOWNHOOK")
    OsData["pretskhk"] = read_os_attribute(sheet, sheetname, attr_col, "PRETASKHOOK")
    OsData["postskhk"] = read_os_attribute(sheet, sheetname, attr_col, "POSTTASKHOOK")
    OsData["ugtsrvid"] = read_os_attribute(sheet, sheetname, attr_col, "USEGETSERVICEID")
    OsData["uparacc"]  = read_os_attribute(sheet, sheetname, attr_col, "USEPARAMETERACCESS")
    OsData["uressch"]  = read_os_attribute(sheet, sheetname, attr_col, "USERESSCHEDULER")

    return OsData



def parse_counters(wb, sheetname):
    # list of column titles in TASK tab of OSEX-Builder.xlsx
    attr_col, attr_row = validate_column(wb, sheetname, CntrParams[0])
    if attr_col == None:
        print("Error: sheet validation failed ("+ sheetname +")")
        return None
    # find out all column numbers of all the column titles and add to a list
    param_cols, hrow = utils.locate_cols(wb, sheetname, CntrParams)
    if param_cols == None:
        print("Error: Task Column location failed!")
        return None
    
    sheet = wb[sheetname]
    active_rows = len(sheet[chr(attr_col)])
    Counters = [dict() for x in range(attr_row, active_rows)]
    cntr_count = 0
    for i in range(attr_row, active_rows):
        row = attr_row + i
        for param in CntrParams:
            Counters[cntr_count][param] = sheet[param_cols[param]+str(row)].value
        cntr_count += 1

    return Counters




def print_oil_item_type1(f, task, tkey, okey, indent):
    count = len(task[tkey])
    item0 = task[tkey][0]
    if count > 1 or not (item0 == "None" or item0 == "NONE"):
        for res in task[tkey]:
            f.write(indent*"\t" +okey+" = "+ res +";\n")



def print_oil_output(OsData, AppMode, TaskData, Counters):
    # open or create output file
    path = "/".join(os.path.abspath(__file__).split("/")[0:-2])
    oil_file_name = OsData["os_name"]+"-"+OsData["cpu"]+".oil"
    full_filename = path+"/"+oil_file_name
    f = open(full_filename, "w")

    # start of CPU
    indent = 0
    f.write("CPU " + OsData["cpu"] + " {\n")
        
    # write OS data
    indent += 1
    f.write(indent*"\t" + " OS " + OsData["os_name"] + " {\n")
    indent += 1
    f.write(indent*"\t" + "STATUS = " + OsData["status"] + ";\n")
    f.write(indent*"\t" + "STARTUPHOOK = " + OsData["strthk"] + ";\n")
    f.write(indent*"\t" + "ERRORHOOK = " + OsData["errhk"] + ";\n")
    f.write(indent*"\t" + "SHUTDOWNHOOK = " + OsData["shdnhk"] + ";\n")
    f.write(indent*"\t" + "PRETASKHOOK = " + OsData["pretskhk"] + ";\n")
    f.write(indent*"\t" + "POSTTASKHOOK = " + OsData["postskhk"] + ";\n")
    f.write(indent*"\t" + "USEGETSERVICEID = " + OsData["ugtsrvid"] + ";\n")
    f.write(indent*"\t" + "USEPARAMETERACCESS = " + OsData["uparacc"] + ";\n")
    f.write(indent*"\t" + "USERESSCHEDULER = " + OsData["uressch"] + ";\n")
    indent -= 1
    f.write(indent*"\t" + "};\n")

    # write tasks
    for task in TaskData:
        f.write(indent*"\t"+ "TASK " + task['Task Name'] + " {\n")
        indent += 1
        f.write(indent*"\t" +"PRIORITY = " + str(task['Priority']) + ";\n")
        f.write(indent*"\t" +"SCHEDULE = " + task['Schedule'] + ";\n")
        f.write(indent*"\t" +"ACTIVATION = " + str(task['Activation']) + ";\n")

        # AUTOSTART and APPMODEs
        appmodes = len(task['Autostart'])
        autostrt = task['Autostart'][0]
        if appmodes == 1 and (autostrt == "None" or autostrt == "NONE"):
	        f.write(indent*"\t" +"AUTOSTART = FALSE;\n")
        else:
            f.write(indent*"\t" +"AUTOSTART = TRUE {\n")
            indent += 1
            for mode in AppMode:
                f.write(indent*"\t" +"APPMODE = "+ mode +";\n")
            indent -= 1
            f.write(indent*"\t" + "};\n")
        
        # type1 == like Resource, Event, Message
        print_oil_item_type1(f, task, 'Resource', 'RESOURCE', indent)
        print_oil_item_type1(f, task, 'Event', 'EVENT', indent)
        print_oil_item_type1(f, task, 'Message', 'MESSAGE', indent)
            
        # End of Task
        indent -= 1
        f.write(indent*"\t" + "};\n")
    
    # Start of COUNTERS
    for cntr in Counters:
        # Start of Counters
        f.write(indent*"\t"+ "COUNTER " + cntr[CntrParams[0]] + " {\n")
        indent += 1
        f.write(indent*"\t" +"MINCYCLE = " + str(cntr[CntrParams[1]]) + ";\n")
        f.write(indent*"\t" +"MAXALLOWEDVALUE = " + str(cntr[CntrParams[2]]) + ";\n")
        f.write(indent*"\t" +"TICKSPERBASE = " + str(cntr[CntrParams[3]]) + ";\n")
        # End of Counters
        indent -= 1
        f.write(indent*"\t" + "};\n")

    #end of CPU
    indent -= 1
    f.write(indent*"\t" + "};\n")
    f.close()
    return full_filename


def main(wb):
    OsData = None
    AppModes = None
    TaskData = None
    Counters = None
    for sheetname in wb.sheetnames:
        if "OS" in sheetname:
            print_info("Parsing OS attributes")
            OsData = parse_os_data(wb, sheetname)
            if OsData == None:
                print(Fore.RED, "Error: OsData parse failure!")
        if "APPMODE" in sheetname:
            print_info("Parsing AppMode data")
            AppModes = parse_appmode_data(wb, sheetname)
            if AppModes == None:
                print(Fore.RED, "Error: AppMode parse failure!")
        if "TASK" in sheetname:
            print_info("Parsing Task data")
            TaskData = parse_task_data(wb, sheetname)
            if TaskData == None:
                print(Fore.RED, "Error: TaskData parse failure!")
        if "COUNTER" in sheetname:
            print_info("Parsing Counters")
            Counters = parse_counters(wb, sheetname)
        

    print_info("Generating OIL file")
    oil_path = print_oil_output(OsData, AppModes, TaskData, Counters)
    print(Fore.GREEN + "OIL file generated in \"" + oil_path + "\"")


def print_usage(prog):
    print("Usage:\n\t python " + prog + " tools/OSEK-Builder.xlsx")


if __name__ == '__main__':
    cmd_args = len(sys.argv)
    if cmd_args < 2:
        print(Fore.RED + "Error: Input excel document not passed as argument!", Style.RESET_ALL)
        print_usage(sys.argv[0])
        exit(-1)
    
    # check and import pre-requisites
    utils.import_or_install("openpyxl")
    utils.import_or_install("colorama")

    excelfile = sys.argv[1]
    if "xlsx" != excelfile.split(".")[-1]:
        print(Fore.RED + "Error: Input file is not an excel file!", Style.RESET_ALL)
    else:
        print_info("Opening " + excelfile)
        print(Fore.CYAN, "\033[F")
        wb = utils.open_excel_file(excelfile)
        print(Style.RESET_ALL, "\033[F")
        print_info("Parsing" + excelfile)
        main(wb)
