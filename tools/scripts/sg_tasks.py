from common import *
from ob_globals import *

import colorama
from colorama import Fore, Back, Style


SchTypes = {
    "NON" : "NON_PREMPTIVE",
    "FULL" : "PREMPTIVE_TSK"
}

OsTaskType_str = "\n\ntypedef void (*TaskFuncType)(void);\n\
\n\
typedef struct {\n\
    TaskType id;\n\
    TaskFuncType handler;\n\
    u32 priority;\n\
    u8 sch_type;\n\
    u32 activation;\n\
    bool autostart;\n\
    const AppModeType** app_modes;\n\
    u32 n_app_modes;\n\
    MessageType** msg;\n\
    u32 n_msg;\n\
    ResourceType** res;\n\
    u32 n_res;\n\
    EventMaskType** evt_msk;\n\
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
        # app_modes
        if "AUTOSTART_APPMODE" in task:
            hf.write("#define "+task[TaskParams[0]].upper()+"_APPMODE_MAX\t("+
                str(len(task["AUTOSTART_APPMODE"]))+")\n")
        else:
            hf.write("#define "+task[TaskParams[0]].upper()+"_APPMODE_MAX\t(0)\n")

        # msg, res, evt
        for i in range(5, 8):
            if TaskParams[i] in task:
                hf.write("#define "+task[TaskParams[0]].upper()+"_"+TaskParams[i]+"_MAX\t("+
                    str(len(task[TaskParams[i]]))+")\n")
            else:
                hf.write("#define "+task[TaskParams[0]].upper()+"_"+TaskParams[i]+"_MAX\t(0)\n")

        # end of for loop
        hf.write("\n")


def print_task_appmodes(cf, Tasks):
    cf.write("\n/*  Task AppModes */\n")
    for task in Tasks:
        if "AUTOSTART_APPMODE" in task:
            cf.write("const AppModeType "+task[TaskParams[0]]+"_AppModes[] = {\n")
            max_i = len(task["AUTOSTART_APPMODE"])
            i = 0
            for m in task["AUTOSTART_APPMODE"]:
                i += 1
                cf.write("\t"+m)
                if i != max_i:
                    cf.write(",\n")
                else:
                    cf.write("\n")
            cf.write("};\n")
        else:
            cf.write("const AppModeType* "+task[TaskParams[0]]+"_AppModes = NULL;\n")


def print_task_events(path, Tasks):
    print_info("Generating code for Tasks events")

    # create header file
    filename = path + "/" + "sg_events.h"
    hf = open(filename, "w")
    hf.write("#ifndef ACN_OSEK_SG_EVENTS_H\n")
    hf.write("#define ACN_OSEK_SG_EVENTS_H\n")
    hf.write("\n#include <osek.h>\n\n")
    hf.write("\n/* OS_EVENT: This macro function allows users to get event mask using\n   the name of the event (passed as 2nd parameter) configured in the\n   OSEK-Builder.xlsx */")
    hf.write("\n#define OS_EVENT(task, event)   (EVENT_MASK_##task##event)\n\n")
    # print event masks macros
    for task in Tasks:
        hf.write("\n/*  Event Masks for "+task[TaskParams[0]]+"  */\n")
        if TaskParams[6] in task:
            for i, event in enumerate(task[TaskParams[6]]):
                hf.write("#define EVENT_MASK_"+task[TaskParams[0]]+"_"+
                    event+"\t("+"0x{:016x}".format(1<<i)+")\n")

    # print event mask array declarations & definitions
    filename = path + "/" + "sg_events.c"
    cf = open(filename, "w")
    cf.write("#include <osek.h>\n")
    cf.write("#include \"sg_events.h\"\n\n")
    for task in Tasks:
        hf.write("\n/*  Event array for "+task[TaskParams[0]]+"  */\n")
        if TaskParams[6] in task:
            hf.write("extern const EventMaskType "+task[TaskParams[0]]
                +"_EventMasks[];\n")
                #+"_EventMasks["+str(len(task[TaskParams[6]]))+"];\n")
            cf.write("\nconst EventMaskType "+task[TaskParams[0]]+"_EventMasks[] = {\n")
            for i, event in enumerate(task[TaskParams[6]]):
                cf.write("\tEVENT_MASK_"+task[TaskParams[0]]+"_"+event)
                if i+1 < len(task[TaskParams[6]]):
                    cf.write(",\n")
                else:
                    cf.write("\n")
            cf.write("};\n")
    hf.write("\n\n#endif\n")
    hf.close()
    cf.close()


def print_task_messages(hf, cf, Tasks):
    banner = "\n/*  Task Messages */\n"
    cf.write(banner)
    messages = []
    for task in Tasks:
        if TaskParams[7] in task:
            cf.write("MessageType "+task[TaskParams[0]]+"_Messages[] = {\n")
            max_i = len(task[TaskParams[7]])
            i = 0
            for m in task[TaskParams[7]]:
                i += 1
                cf.write("\t"+m)
                if m not in messages:
                    messages.append(m)
                if i != max_i:
                    cf.write(",\n")
                else:
                    cf.write("\n")
            cf.write("};\n")
        else:
            cf.write("const MessageType* "+task[TaskParams[0]]+"_Messages = NULL;\n")

    if len(messages) > 0:
        hf.write(banner)
        for m in messages:
            hf.write("extern MessageType "+m+";\n")



def generate_code(path, Tasks, AppModes):
    print_info("Generating code for Tasks")

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
    hf.write(OsTaskType_str)

    # create source file
    filename = path + "/" + "sg_tasks.c"
    cf = open(filename, "w")
    cf.write("#include <stddef.h>\n")
    cf.write("#include \"sg_tasks.h\"\n")
    cf.write("\n\n/*   T A S K   D E F I N I T I O N   */\n")
    print_task_appmodes(cf, Tasks)
    print_task_events(path, Tasks)
    #print_task_messages(hf, cf, Tasks)
    cf.write("\nconst OsTaskType OsTaskList["+str(len(Tasks))+"] = {\n")
    for i, task in enumerate(Tasks):
        cf.write("\t{\n")
        if "AUTOSTART_APPMODE" in task:
            cf.write("\t\t.app_modes = (const AppModeType **) &"+task[TaskParams[0]]+"_AppModes\n")
        else:
             cf.write("\t\t.app_modes = NULL\n")
        cf.write("\t}")
        if i+1 < len(Tasks):
            cf.write(",\n")
        else:
            cf.write("\n")
    cf.write("};\n")

    # Tasks is still in construction hence print it
    print(Tasks)
    print("\nTASKs in construction\n\n")

    cf.close()
    hf.write("\n\n#endif\n")
    hf.close()
