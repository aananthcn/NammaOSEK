from common import print_info
from ob_globals import TaskParams

import colorama
from colorama import Fore, Back, Style


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

