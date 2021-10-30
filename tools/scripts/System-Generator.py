# This tool takes *.oil file generated by OSEK-Builder tool as input and 
# generates .c/.h/.mk files as output.
import sys
import os

from common import import_or_install, print_info
from ob_globals import TaskParams, CntrParams, AlarmParams, ISR_Params, TNMI, OS_Params

import sg_counter
import sg_tasks
import sg_alarms
import sg_appmodes
import sg_events
import sg_messages
import sg_resources
import sg_fifo
import sg_os_param

import colorama
from colorama import Fore, Back, Style

Counters = []
Alarms = []
Tasks = []
AppModes = []



def print_usage(prog):
    print("Usage:\n\t python " + prog + " tools/oil-files/*.oil")



def parse_alarm_autostart(oil_lines, line_num, alarms):
    line = oil_lines[line_num]
    alarms[AlarmParams[5]] = line.replace('=', '{').split('{')[1].strip()
    line_num += 1
    while "};" not in oil_lines[line_num]:
        line = oil_lines[line_num]
        if "ALARMTIME" in line.split():
            alarms[AlarmParams[6]] = line.replace('=', ';').split(';')[1].strip()
        if "CYCLETIME" in line.split():
            alarms[AlarmParams[7]] = line.replace('=', ';').split(';')[1].strip()
        if "APPMODE" in line.split():
            try:
                alarms[AlarmParams[8]].append(line.replace('=', ';').split(';')[1].strip())
            except KeyError:
                alarms[AlarmParams[8]] = []
                alarms[AlarmParams[8]].append(line.replace('=', ';').split(';')[1].strip())
        line_num += 1

    return line_num, alarms



def parse_alarm_action(oil_lines, line_num, alarms):
    line = oil_lines[line_num]
    alarms[AlarmParams[2]] = line.replace('=', '{').split('{')[1].strip()
    line_num += 1
    while "};" not in oil_lines[line_num]:
        line = oil_lines[line_num]
        if "TASK" in line.split():
            alarms[AlarmParams[3]] = line.replace('=', ';').split(';')[1].strip()
        if "EVENT" in line.split():
            alarms[AlarmParams[4]] = line.replace('=', ';').split(';')[1].strip()
        if "ALARMCALLBACKNAME" in line.split():
            alarms[AlarmParams[3]] = line.replace('=', ';').split(';')[1].strip()
        line_num += 1

    return line_num, alarms



def parse_alarms(oil_lines, line_num):
    alarms = {}
    alarms[AlarmParams[0]] = oil_lines[line_num].split()[1]
    line_num += 1
    while "};" not in oil_lines[line_num]:
        line = oil_lines[line_num]
        if AlarmParams[1] in line:
            alarms[AlarmParams[1]] = line.replace('=', ';').split(';')[1].strip()
        if "ACTION" in line.split() and "{" in line:
            line_num, alarms = parse_alarm_action(oil_lines, line_num, alarms)
        if "AUTOSTART" in line.split() and "{" in line:
            line_num, alarms = parse_alarm_autostart(oil_lines, line_num, alarms)
        elif "AUTOSTART" in line.split() and "FALSE" in line:
            alarms[AlarmParams[5]] = line.replace('=', ';').split(';')[1].strip()

        line_num += 1

    return line_num, alarms



def parse_task_autostart(oil_lines, line_num, task):
    line = oil_lines[line_num]
    task[TaskParams[4]] = line.replace('=', '{').split('{')[1].strip()
    line_num += 1
    while "};" not in oil_lines[line_num]:
        line = oil_lines[line_num]
        if "APPMODE" in line.split():
            app_mode = line.replace('=', ';').split(';')[1].strip()
            if app_mode not in AppModes:
                AppModes.append(app_mode)
            try:
                task["AUTOSTART_APPMODE"].append(app_mode)
            except KeyError:
                task["AUTOSTART_APPMODE"] = []
                task["AUTOSTART_APPMODE"].append(app_mode)
        line_num += 1

    return line_num, task



def parse_tasks_item_list(line, task, param):
    if TaskParams[param] in line.split():
        try:
            task[TaskParams[param]].append(line.replace('=', ';').split(';')[1].strip())
        except KeyError:
            task[TaskParams[param]] = []
            task[TaskParams[param]].append(line.replace('=', ';').split(';')[1].strip())
    return task



def parse_tasks(oil_lines, line_num):
    tasks = {}
    tasks[TaskParams[TNMI]] = oil_lines[line_num].split()[1]
    line_num += 1
    while "};" not in oil_lines[line_num]:
        line = oil_lines[line_num]
        if TaskParams[1] in line:
            tasks[TaskParams[1]] = line.replace('=', ';').split(';')[1].strip()
        if TaskParams[2] in line:
            tasks[TaskParams[2]] = line.replace('=', ';').split(';')[1].strip()
        if TaskParams[3] in line:
            tasks[TaskParams[3]] = line.replace('=', ';').split(';')[1].strip()
        if "AUTOSTART" in line.split() and "{" in line:
            line_num, tasks = parse_task_autostart(oil_lines, line_num, tasks)
        elif "AUTOSTART" in line.split() and "FALSE" in line:
            tasks[TaskParams[4]] = line.replace('=', ';').split(';')[1].strip()
        tasks = parse_tasks_item_list(line, tasks, 5)
        tasks = parse_tasks_item_list(line, tasks, 6)
        tasks = parse_tasks_item_list(line, tasks, 7)
        line_num += 1

    return line_num, tasks



def parse_counter(oil_lines, line_num):
    cntr = {}
    cntr[CntrParams[0]] = oil_lines[line_num].split()[1]
    line_num += 1
    while "};" not in oil_lines[line_num]:
        line = oil_lines[line_num]
        if CntrParams[1] in line:
            cntr[CntrParams[1]] = int(line.replace('=', ';').split(';')[1])
        if CntrParams[2] in line:
            cntr[CntrParams[2]] = line.replace('=', ';').split(';')[1].strip()
        if CntrParams[3] in line:
            cntr[CntrParams[3]] = int(line.replace('=', ';').split(';')[1])
        if CntrParams[4] in line:
            cntr[CntrParams[4]] = int(line.replace('=', ';').split(';')[1])
        line_num += 1
    return line_num, cntr



def parse_os_params(oil_lines, line_num):
    params = {}
    line_num += 1
    while "};" not in oil_lines[line_num]:
        line = oil_lines[line_num]
        if OS_Params[0] in line:
            params[OS_Params[0]] = line.replace('=', ';').split(';')[1].strip()
        if OS_Params[1] in line:
            params[OS_Params[1]] = line.replace('=', ';').split(';')[1].strip()
        line_num += 1
    return line_num, params



def main(of):
    path = "/".join(os.path.abspath(__file__).split("/")[0:-2]) + "/src"
    if not os.path.exists(path):
        print_info("Creating source file directory " + path)
        os.mkdir(path)
    
    print_info("Parsing " + oilfile)
    oil_lines = of.readlines()
    total_lines = len(oil_lines)
    line_num = 1
    while line_num < total_lines:
        words = oil_lines[line_num].split()
        if "COUNTER" in words and "{" in oil_lines[line_num]:
            line_num, cntr = parse_counter(oil_lines, line_num)
            Counters.append(cntr)
        if "ALARM" in words and "{" in oil_lines[line_num]:
            line_num, alrms = parse_alarms(oil_lines, line_num)
            Alarms.append(alrms)
        if "TASK" in words and "{" in oil_lines[line_num]:
            line_num, task = parse_tasks(oil_lines, line_num)
            Tasks.append(task)
        if "FreeOSEK_PARAMS" in words and "{" in oil_lines[line_num]:
            line_num, os_params = parse_os_params(oil_lines, line_num)
        line_num += 1

    sg_counter.generate_code(path, Counters)
    sg_appmodes.generate_code(path, AppModes, Tasks)
    sg_events.generate_code(path, Tasks)
    sg_messages.generate_code(path, Tasks)
    sg_resources.generate_code(path, Tasks)
    sg_tasks.generate_code(path, Tasks)
    sg_alarms.generate_code(path, Alarms, Counters, Tasks)
    sg_fifo.generate_code(path, Tasks)
    sg_os_param.generate_code(path, os_params)



if __name__ == '__main__':
    cmd_args = len(sys.argv)
    if cmd_args < 2:
        print(Fore.RED + "Error: Input OIL file not passed as argument!", Style.RESET_ALL)
        print_usage(sys.argv[0])
        exit(-1)
    
    # check and import pre-requisites
    import_or_install("colorama")

    oilfile = sys.argv[1]
    print_info("Opening " + oilfile)
    if "oil" != oilfile.split(".")[-1]:
        print(Fore.RED + "Error: Input file is not an OIL file!", Style.RESET_ALL)
    
    if not os.path.exists(oilfile):
        print(Fore.RED + "Error: Can't open OIL file: \""+oilfile+"\"!", Style.RESET_ALL)
    else:
        of = open(oilfile, "r")
        print(Style.RESET_ALL, "\033[F")
        main(of)
