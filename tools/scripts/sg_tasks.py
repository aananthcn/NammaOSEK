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
    u32 activation;\n\
    bool autostart;\n\
    AppModeType** app_modes;\n\
    u32 n_app_modes;\n\
    MessageType** msg;\n\
    u32 n_msg;\n\
    ResourceType** res;\n\
    u32 n_res;\n\
    EventMaskType* evt_msk;\n\
    u32 n_evt;\n\
} OsTaskType;\n\
\n\
extern const OsTaskType OsTaskList[];\n\n"


def print_task_ids(hf, Tasks):
    hf.write("\n\nenum eTaskType {\n")
    for task in Tasks:
        hf.write("\tTASK_"+task[TaskParams[0]].upper()+"_ID,\n")
    hf.write("\tTASK_ID_MAX\n")
    hf.write("};\n")


def print_app_mode(hf, am):
    hf.write("\n\nenum eAppModeType {\n")
    for m in am:
        hf.write("\t"+m+",\n")
    hf.write("\tOS_MODES_MAX\n")
    hf.write("};\n")

def print_task_len_macros(hf, Tasks):
    hf.write("\n\n")
    for task in Tasks:
        if "AUTOSTART_APPMODE" in task:
            hf.write("#define "+task[TaskParams[0]].upper()+"_APPMODES\t("+
                str(len(task["AUTOSTART_APPMODE"]))+")\n")
        else:
            hf.write("#define "+task[TaskParams[0]].upper()+"_APPMODES\t(0)\n")



def generate_code(path, Tasks, AppModes):
    print_info("Generating code for Tasks")
    print("\n", AppModes, "\n")

    # create header file
    filename = path + "/" + "sg_tasks.h"
    hf = open(filename, "w")
    hf.write("#ifndef ACN_OSEK_SG_TASKS_H\n")
    hf.write("#define ACN_OSEK_SG_TASKS_H\n")
    hf.write("\n#include <osek.h>\n")
    hf.write("#include <osek_com.h>\n")
    print_app_mode(hf, AppModes)
    print_task_ids(hf, Tasks)
    print_task_len_macros(hf, Tasks)
    hf.write(TaskDefBlock)

    # create source file
    filename = path + "/" + "sg_tasks.c"
    cf = open(filename, "w")
    cf.write("#include \"sg_tasks.h\"\n")
    cf.write("\n\n/*   T A S K   D E F I N I T I O N   */")
    cf.write("\nconst OsTaskType OsTaskList["+str(len(Tasks))+"];\n")

    # Tasks is still in construction hence print it
    print(Tasks)
    print("\nTASKs in construction\n\n")

    cf.close()
    hf.write("\n\n#endif\n")
    hf.close()
