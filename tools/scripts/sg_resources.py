from common import print_info
from ob_globals import TaskParams, TNMI, RESI

import colorama
from colorama import Fore, Back, Style


OsResMapType_str = "\n\n\n\
typedef struct {\n\
    ResourceType* res;\n\
    u32 n_tasks;\n\
    const TaskType* task_ids;\n\
} OsResMapType;\n\
\n\
extern const OsResMapType _OsResList[];\n"


def generate_code(path, Tasks):
    # create header file
    filename = path + "/" + "sg_resources.h"
    hf = open(filename, "w")
    hf.write("#ifndef ACN_OSEK_SG_RESOURCES_H\n")
    hf.write("#define ACN_OSEK_SG_RESOURCES_H\n")
    hf.write("\n#include <osek.h>")

    # create source file
    filename = path + "/" + "sg_resources.c"
    cf = open(filename, "w")
    cf.write("#include <stddef.h>\n")
    cf.write("#include \"sg_resources.h\"\n")
    cf.write("#include \"sg_tasks.h\"\n")

    # collect & create all individual resources
    Resources = []
    hf.write("\n\n\n#define RES(x)  RES_##x\n")
    hf.write("\ntypedef enum {\n")
    for task in Tasks:
        if TaskParams[RESI] in task:
            for r in task[TaskParams[RESI]]:
                if r not in Resources:
                    Resources.append(r)
                    hf.write("\tRES_"+r+",\n")
    hf.write("\tMAX_RESOURCE_ID\n} OsResourcesType;")
    hf.write(OsResMapType_str)

    # Resources in RAM
    cf.write("\n\n\n/* Resources Definitions in RAM */\n")
    for res in Resources:
        cf.write("ResourceType " + str(res) + ";\n")

    # Const Resource Structure for OS kernel
    comment = "\n\n/*  Resources lists for Tasks */\n"
    cf.write(comment)
    ResTaskList = []
    for res in Resources:
        # Parse and collect tasks associated with "res"
        TaskData = {}
        task_cnt = 0
        task_lst = []
        for task in Tasks:
            if TaskParams[RESI] in task:
                for r in task[TaskParams[RESI]]:
                    if str(r) == str(res):
                        task_cnt += 1
                        task_lst.append(task[TaskParams[TNMI]])
        TaskData["res"] = str(res)
        TaskData["n_tasks"] = task_cnt
        TaskData["tasks"] = task_lst
        ResTaskList.append(TaskData)

    # Create C structures with the parsed tasks
    for rt in ResTaskList:
        cf.write("const TaskType "+ rt["res"]+"_tasks[] = {\n")
        for t in rt["tasks"]:
            cf.write("\tTASK_"+t.upper()+"_ID,\n")
        cf.write("};\n\n")

    cf.write("const OsResMapType _OsResList[MAX_RESOURCE_ID] = {\n")
    for rt in ResTaskList:
        cf.write("\t{\n\t\t.res = &"+rt["res"]+",\n")
        cf.write("\t\t.n_tasks = "+ str(rt["n_tasks"])+",\n")
        cf.write("\t\t.task_ids = " + rt["res"]+"_tasks\n")
        cf.write("\t},\n")
    cf.write("};\n")

    hf.write("\n\n#endif\n")
    hf.close()
    cf.close()


