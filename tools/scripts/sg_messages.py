from common import print_info
from ob_globals import TaskParams

import colorama
from colorama import Fore, Back, Style


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


