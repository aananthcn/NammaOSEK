from common import print_info
from ob_globals import OS_Params

import colorama
from colorama import Fore, Back, Style


def generate_code(path, OsParams):
    print_info("Generating code for FreeOSEK Parameters")

    # create stack definitions
    filename = path + "/" + "sg_stack.lds"
    lf = open(filename, "w")
    for param in OS_Params:
        lf.write(param + " = " + OsParams[param] + ";\n")
    
    lf.close()
