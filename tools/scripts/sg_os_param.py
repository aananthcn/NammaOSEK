from common import print_info
from ob_globals import OS_Params

import colorama
from colorama import Fore, Back, Style


def generate_code(path, OsParams):
    print_info("Generating code for FreeOSEK Parameters")

    # create stack definitions
    filename = path + "/" + "sg_stack.lds"
    lf = open(filename, "w")
    lf.write(OS_Params[0] + " = " + OsParams[OS_Params[0]] + ";\n")
    lf.write(OS_Params[1] + " = " + OsParams[OS_Params[1]] + ";\n")
    
    lf.close()
