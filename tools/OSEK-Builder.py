# This tool takes "OSEK-Builder.xlsx" as input and generated *.oil files
# as output.

import sys
import utils

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
    cpu = os_name = status = strthk = errhk = sdnhk = ""
    prtskhk = potskhk = ugtsvid = uparacc = usrsch = ""

    # validate "OS" sheet and find column to read attributes
    col, row = utils.locate_heading_column("Attribute Name", wb, sheetname)
    if col == -1:
        print("Error: can't locate \"Attribute Name\" in sheet \"" + sheetname + "\"")
        return
    attr_col = col
    os_sheet = wb[sheetname]

    # read CPU
    cpu     = read_os_attribute(os_sheet, sheetname, attr_col, "CPU")
    os_name = read_os_attribute(os_sheet, sheetname, attr_col, "OS NAME")
    status  = read_os_attribute(os_sheet, sheetname, attr_col, "STATUS")
    strthk  = read_os_attribute(os_sheet, sheetname, attr_col, "STARTUPHOOK")
    errhk   = read_os_attribute(os_sheet, sheetname, attr_col, "ERRORHOOK")
    sdnhk   = read_os_attribute(os_sheet, sheetname, attr_col, "SHUTDOWNHOOK")
    prtskhk = read_os_attribute(os_sheet, sheetname, attr_col, "PRETASKHOOK")
    potskhk = read_os_attribute(os_sheet, sheetname, attr_col, "POSTTASKHOOK")
    ugtsvid = read_os_attribute(os_sheet, sheetname, attr_col, "USEGETSERVICEID")
    uparacc = read_os_attribute(os_sheet, sheetname, attr_col, "USEPARAMETERACCESS")
    usrsch  = read_os_attribute(os_sheet, sheetname, attr_col, "USERESSCHEDULER")

    print("OS:" + cpu, os_name, status, strthk, errhk, sdnhk, prtskhk, potskhk, ugtsvid, uparacc, usrsch)

    # read OS name

def main(wb):
    for sheetname in wb.sheetnames:
        if "OS" in sheetname:
            parse_os_data(wb, sheetname)


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

    excelfile = sys.argv[1]
    if "xlsx" != excelfile.split(".")[-1]:
        print("Error: Input file is not an excel file!")
    else:
        wb = utils.open_excel_file(excelfile)
        main(wb)
