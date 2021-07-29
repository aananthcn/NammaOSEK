from common import print_info
from ob_globals import TaskParams, TNMI, RESI

import colorama
from colorama import Fore, Back, Style


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

    # collect & create all individual resources
    comment = "\n\n/*  Resources described in OIL file */\n"
    cf.write(comment)
    hf.write(comment)
    messages = []
    for task in Tasks:
        if TaskParams[RESI] in task:
            for r in task[TaskParams[RESI]]:
                if r not in messages:
                    messages.append(r)
                    cf.write("ResourceType "+r+";\n")
                    hf.write("extern ResourceType "+r+";\n")

    # print tasks resources list
    comment = "\n\n/*  Resources lists for Tasks */\n"
    cf.write(comment)
    hf.write(comment)
    for task in Tasks:
        if TaskParams[RESI] in task:
            hf.write("extern ResourceType* "+task[TaskParams[TNMI]]+"_Resources[];\n")
            cf.write("ResourceType* "+task[TaskParams[TNMI]]+"_Resources[] = {\n")
            max_i = len(task[TaskParams[RESI]])
            i = 0
            for m in task[TaskParams[RESI]]:
                i += 1
                cf.write("\t&"+m)
                if i != max_i:
                    cf.write(",\n")
                else:
                    cf.write("\n")
            cf.write("};\n\n")

    hf.write("\n\n#endif\n")
    hf.close()
    cf.close()


