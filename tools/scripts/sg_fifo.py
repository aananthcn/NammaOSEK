from common import print_info
from ob_globals import TaskParams, TNMI, PRII, ACTI

import colorama
from colorama import Fore, Back, Style


def populate_queue(f, qstr, maxPrio, fifoInfo):
    f.write("const OsFifoType* "+qstr+"Queue[] = {\n")
    for prio in range(maxPrio+1):
        if str(prio) in fifoInfo:
            f.write("\t&"+qstr+"Fifo_"+str(prio))
        else:
            f.write("\tNULL")
        if prio != maxPrio:
            f.write(",\n")
        else:
            f.write("\n")
    f.write("};\n\n")


def generate_code(path, Tasks):
    maxPrio = 0
    fifoInfo = {}
    for task in Tasks:
        prio = int(task[TaskParams[PRII]])
        if  prio > maxPrio:
            maxPrio = prio
        if str(task[TaskParams[PRII]]) not in fifoInfo:
            fifoInfo[task[TaskParams[PRII]]] = int(task[TaskParams[ACTI]])
        else:
            fifoInfo[task[TaskParams[PRII]]] += int(task[TaskParams[ACTI]])


    # create header file
    filename = path + "/" + "sg_fifo.h"
    hf = open(filename, "w")
    hf.write("#ifndef ACN_OSEK_SG_FIFO_H\n")
    hf.write("#define ACN_OSEK_SG_FIFO_H\n")
    hf.write("\n#include <os_fifo.h>\n")
    hf.write("\n\n#define SG_FIFO_QUEUE_MAX_LEN   ("+str(maxPrio+1)+")\n\n")

    # print queue declarations
    #hf.write("extern const OsFifoType* WaitingQueue[];\n")
    #hf.write("extern const OsFifoType* SuspendedQueue[];\n")
    hf.write("extern const OsFifoType* ReadyQueue[];\n")
    #hf.write("extern const OsFifoType* RunningQueue[];\n")


    # create source file
    filename = path + "/" + "sg_fifo.c"
    cf = open(filename, "w")
    cf.write("#include <stddef.h>\n")
    cf.write("#include \"sg_fifo.h\"\n")
    cf.write("\n/* Allocate Buffers in RAM */\n")

    # 1st level declarations and definitions
    for prio in range(maxPrio+1):
        qsize = 0
        if str(prio) in fifoInfo:
            qsize = fifoInfo[str(prio)]
        
        # print queue definitions
        if qsize > 0:
            #cf.write("OsTaskType* WaitingTasks_"+str(prio)+"["+str(qsize)+"];\n")
            #cf.write("OsTaskType* SuspendedTasks_"+str(prio)+"["+str(qsize)+"];\n")
            cf.write("OsTaskType* ReadyTasks_"+str(prio)+"["+str(qsize)+"];\n")
    cf.write("OsTaskType* RunningTasks[1];\n")


    # 2nd level - define FIFOs
    cf.write("\n\n/* Allocate FIFO queues in RAM */")
    for prio in range(maxPrio+1):
        qsize = 0
        if str(prio) in fifoInfo:
            qsize = fifoInfo[str(prio)]
        if qsize == 0:
            continue
        #cf.write("\nOsFifoType WaitingFifo_"+str(prio)+" = {\n")
        #cf.write("\t.task = WaitingTasks_"+str(prio)+",\n")
        #cf.write("\t.size = "+str(qsize)+",\n")
        #cf.write("\t.head = 0,\n")
        #cf.write("\t.tail = 0,\n")
        #cf.write("#ifdef DEBUG\n")
        #cf.write("\t.name = \"WaitingFifo_"+str(prio)+"\",\n")
        #cf.write("#endif\n")
        #cf.write("\t.full = false\n")
        #cf.write("};\n")
        #cf.write("\nOsFifoType SuspendedFifo_"+str(prio)+" = {\n")
        #cf.write("\t.task = SuspendedTasks_"+str(prio)+",\n")
        #cf.write("\t.size = "+str(qsize)+",\n")
        #cf.write("\t.head = 0,\n")
        #cf.write("\t.tail = 0,\n")
        #cf.write("#ifdef DEBUG\n")
        #cf.write("\t.name = \"SuspendedFifo_"+str(prio)+"\",\n")
        #cf.write("#endif\n")
        #cf.write("\t.full = false\n")
        #cf.write("};\n")
        cf.write("\nOsFifoType ReadyFifo_"+str(prio)+" = {\n")
        cf.write("\t.task = ReadyTasks_"+str(prio)+",\n")
        cf.write("\t.size = "+str(qsize)+",\n")
        cf.write("\t.head = 0,\n")
        cf.write("\t.tail = 0,\n")
        cf.write("#ifdef DEBUG\n")
        cf.write("\t.name = \"ReadyFifo_"+str(prio)+"\",\n")
        cf.write("#endif\n")
        cf.write("\t.full = false\n")
        cf.write("};\n")
        cf.write("\n")

    # RunningFifo is a special case, as there can be max 1 task running at any
    # given point of time. Hence Queue is not generated for RunningFifo

    # populate waiting queue
    cf.write("\n\n/* Prioritized OSEK FIFO queues in Flash */\n")
    #populate_queue(cf, "Waiting", maxPrio, fifoInfo)
    #populate_queue(cf, "Suspended", maxPrio, fifoInfo)
    populate_queue(cf, "Ready", maxPrio, fifoInfo)


    hf.write("\n\n#endif\n")
    hf.close()
    cf.close()
