# This tool takes "OSEK-Builder.xlsx" as input and generated *.oil files
# as output.

import sys
import os

import ob_utils
from ob_globals import TaskParams, CntrParams, AlarmParams, ISR_Params
from common import print_info, import_or_install
import oil

import colorama
from colorama import Fore, Back, Style


# Functions
# ------------------
def validate_column(wb, sheetname, col_header):
    col, row = ob_utils.locate_heading_column(col_header, wb, sheetname)
    if col == -1:
        print(Fore.RED + "Error: can't locate \""+col_header+"\" in sheet \"" + sheetname + "\"")
        print(Style.RESET_ALL)
        return None

    return col, row


def parse_task_data(wb, sheetname):
    # parameters / columns that has comma separated entries
    specltasks = ["AUTOSTART", "RESOURCE", "EVENT", "MESSAGE"]
    # find out all column numbers of all the column titles and add to a list
    param_cols, hrow = ob_utils.locate_cols(wb, sheetname, TaskParams)
    if param_cols == None:
        print("Error: Task Column location failed!")
        return None
    
    sheet = wb[sheetname]
    active_rows = len(sheet[param_cols[TaskParams[0]]])
    TaskData = [dict() for x in range(hrow, active_rows)]
    task_count = 0
    for i in range(hrow, active_rows):
        row = i + 1
        for tsk_item in TaskParams:
            if tsk_item in specltasks:
                lvalue = str(sheet[param_cols[tsk_item]+str(row)].value).split(",")
                stripped = [s.strip() for s in lvalue]
                TaskData[task_count][tsk_item] = stripped
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
        row = i + 1
        AppModes.append(sheet[chr(attr_col)+str(row)].value.strip())
    
    #print(AppModes)
    return AppModes



def read_os_attribute(sheet, sheetname, attr_col, attr):
    col, row = ob_utils.locate_text_in_column(attr, sheet, attr_col)
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
    if attr_row == -1:
        print("Warning: validate_column() return row = -1")

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
    OsData["irq_stack"]  = read_os_attribute(sheet, sheetname, attr_col, "IRQ_STACK_SIZE")

    return OsData



def parse_counters(wb, sheetname):
    attr_col, attr_row = validate_column(wb, sheetname, CntrParams[0])
    if attr_col == None:
        print("Error: sheet validation failed ("+ sheetname +")")
        return None
    # find out all column numbers of all the column titles and add to a list
    param_cols, hrow = ob_utils.locate_cols(wb, sheetname, CntrParams)
    if hrow == -1:
        print("Warning: parse_counters.ob_utils.locate_cols return a negative row!")
    if param_cols == None:
        print("Error: Task Column location failed!")
        return None
    
    sheet = wb[sheetname]
    active_rows = len(sheet[chr(attr_col)])
    Counters = [dict() for x in range(attr_row, active_rows)]
    cntr_count = 0
    for i in range(attr_row, active_rows):
        row = i + 1
        for param in CntrParams:
            Counters[cntr_count][param] = sheet[param_cols[param]+str(row)].value
        cntr_count += 1

    return Counters



def parse_alarms(wb, sheetname, Counters):
    attr_col, attr_row = validate_column(wb, sheetname, AlarmParams[0])
    if attr_col == None:
        print("Error: sheet validation failed ("+ sheetname +")")
        return None
    # find out all column numbers of all the column titles and add to a list
    param_cols, hrow = ob_utils.locate_cols(wb, sheetname, AlarmParams)
    if hrow == -1:
        print("Warning: parse_alarms.ob_utils.locate_cols returned a negative row!")
    if param_cols == None:
        print("Error: Task Column location failed!")
        return None
    
    sheet = wb[sheetname]
    active_rows = len(sheet[chr(attr_col)])
    Alarms = [dict() for x in range(attr_row, active_rows)]
    alrm_count = 0
    for i in range(attr_row, active_rows):
        row = i + 1
        for param in AlarmParams:
            if param == "COUNTER":
                cnt_num = sheet[param_cols[param]+str(row)].value
                Alarms[alrm_count][param] = Counters[int(cnt_num)-1][CntrParams[0]]
            else:
                Alarms[alrm_count][param] = sheet[param_cols[param]+str(row)].value
        alrm_count += 1
    
    return Alarms



def parse_ISRs(wb, sheetname):
    attr_col, attr_row = validate_column(wb, sheetname, ISR_Params[0])
    if attr_col == None:
        print("Error: sheet validation failed ("+ sheetname +")")
        return None
    # find out all column numbers of all the column titles and add to a list
    param_cols, hrow = ob_utils.locate_cols(wb, sheetname, ISR_Params)
    if hrow == -1:
        print("Warning: parse_ISRs.ob_utils.locate_cols returned a negative row!")
    if param_cols == None:
        print("Error: Task Column location failed!")
        return None
    
    sheet = wb[sheetname]
    active_rows = len(sheet[chr(attr_col)])
    ISRs = [dict() for x in range(attr_row, active_rows)]
    cntr_count = 0
    for i in range(attr_row, active_rows):
        row = i + 1
        for param in ISR_Params:
            ISRs[cntr_count][param] = sheet[param_cols[param]+str(row)].value
        cntr_count += 1

    return ISRs



def main(wb):
    OsData = None
    AppModes = None
    TaskData = None
    Counters = None
    Alarms = None
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
        if "ALARM" in sheetname:
            print_info("Parsing Alarms")
            Alarms = parse_alarms(wb, sheetname, Counters)
        if "ISR" in sheetname:
            print_info("Parsing ISRs")
            ISRs = parse_ISRs(wb, sheetname)

    print_info("Generating OIL file")
    oil_path = oil.print_output(OsData, AppModes, TaskData, Counters, Alarms, ISRs)
    print(Fore.LIGHTMAGENTA_EX + "OIL file generated in \"" + oil_path + "\"")


def print_usage(prog):
    print("Usage:\n\t python " + prog + " tools/OSEK-Builder.xlsx")


if __name__ == '__main__':
    cmd_args = len(sys.argv)
    if cmd_args < 2:
        print(Fore.RED + "Error: Input excel document not passed as argument!", Style.RESET_ALL)
        print_usage(sys.argv[0])
        exit(-1)
    
    # check and import pre-requisites
    import_or_install("openpyxl")
    import_or_install("colorama")

    excelfile = sys.argv[1]
    if "xlsx" != excelfile.split(".")[-1]:
        print(Fore.RED + "Error: Input file is not an excel file!", Style.RESET_ALL)
    else:
        print_info("Opening " + excelfile)
        print(Fore.CYAN, "\033[F")
        wb = ob_utils.open_excel_file(excelfile)
        print(Style.RESET_ALL, "\033[F")
        print_info("Parsing" + excelfile)
        main(wb)
