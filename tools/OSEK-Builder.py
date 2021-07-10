# This tool takes "OSEK-Builder.xlsx" as input and generated *.oil files
# as output.

import sys
import utils

import colorama
from colorama import Fore, Back, Style


def validate_column(wb, sheetname, col_header):
    col, row = utils.locate_heading_column(col_header, wb, sheetname)
    if col == -1:
        print("Error: can't locate \""+col_header+"\" in sheet \"" + sheetname + "\"")
        return None

    return col, row



def parse_appmode_data(wb, sheetname):
    attr_col, attr_row = validate_column(wb, sheetname, "APPMODE")
    if attr_col == None:
        return None

    sheet = wb[sheetname]
    AppModes = []
    active_rows = len(sheet[chr(attr_col)])
    for i in range(attr_row, active_rows):
        row = i + attr_row
        AppModes.append(sheet[chr(attr_col)+str(row)].value.strip())
    
    print(AppModes)
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

    print(OsData)
    return OsData


def main(wb):
    for sheetname in wb.sheetnames:
        if "OS" in sheetname:
            OsData = parse_os_data(wb, sheetname)
        if "APPMODE" in sheetname:
            AppModes = parse_appmode_data(wb, sheetname)


def print_usage(prog):
    print("Usage:\n\t python " + prog + " tools/OSEK-Builder.xlsx")


if __name__ == '__main__':
    cmd_args = len(sys.argv)
    if cmd_args < 2:
        print("Error: Input excel document not passed as argument!")
        print_usage(sys.argv[0])
        exit(-1)
    
    # check and import pre-requisites
    utils.import_or_install("openpyxl")
    utils.import_or_install("colorama")

    excelfile = sys.argv[1]
    if "xlsx" != excelfile.split(".")[-1]:
        print("Error: Input file is not an excel file!")
    else:
        print(Fore.BLUE)
        wb = utils.open_excel_file(excelfile)
        print(Style.RESET_ALL)
        main(wb)
