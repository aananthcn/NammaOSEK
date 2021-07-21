from common import *
from ob_globals import *

import colorama
from colorama import Fore, Back, Style


SchTypes = {
    "NON" : "NON_PREMPTIVE",
    "FULL" : "PREMPTIVE_TSK"
}

TaskDefBlock = "\n\ntypedef void (*TaskFuncType)(void);\n\
\n\
typedef struct {\n\
    TaskType id;\n\
    TaskFuncType handler;\n\
    u32 priority;\n\
    u8 sch_type;\n\
    bool autostart;\n\
    AppModeType app_modes[tOS_MODES_MAX];\n\
} OsTaskType;\n\
\n\
extern OsTaskType OsTaskList[];\n\n"


def print_app_mode(hf, am):
    hf.write("\n\nenum eAppModeType {\n")
    for m in am:
        hf.write("\t"+m+",\n")
    hf.write("\tOS_MODES_MAX\n")
    hf.write("};\n")


def generate_code(path, Tasks, AppModes):
    print(Tasks)
    print(AppModes)
    print_info("Generating code for Tasks")

    # create header file
    filename = path + "/" + "sg_tasks.h"
    hf = open(filename, "w")
    hf.write("#ifndef ACN_OSEK_SG_TASKS_H\n")
    hf.write("#define ACN_OSEK_SG_TASKS_H\n")
    hf.write("\n#include <osek.h>\n")
    print_app_mode(hf, AppModes)
    hf.write(TaskDefBlock)

    # create source file
    filename = path + "/" + "sg_tasks.c"
    cf = open(filename, "w")
    cf.write("#include \"sg_tasks.h\"\n")

    cf.close()
    hf.write("\n\n#endif\n")
    hf.close()
