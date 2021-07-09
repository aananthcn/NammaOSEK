# This tool takes "OSEK-Builder.xlsx" as input and generated *.oil files
# as output.

import sys
import utils


def parse_os_data(wb, sheetname):
    cpu = os_name = status = startuphook = errorhook = shutdownhook = ""
    pretaskhook = posttaskhook = usegetserviceid = useparameteraccess = ""
    useresscheduler = ""

    col, row = utils.locate_heading_column("Attribute Name", wb, sheetname)
    if col == -1:
        print("Error: can't locate \"Attribute Name\" in sheet \"" + sheetname + "\"")
        return
    attr_col = col
    os_sheet = wb[sheetname]

    col, row = utils.locate_text_in_column("CPU", os_sheet, attr_col)
    if row == -1:
        print("Error: can't locate \"CPU\" in sheet \"" + sheetname + "\"")
        return
    cpu = os_sheet[chr(col+1)+str(row)].value
    print("cpu = " + cpu)


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
