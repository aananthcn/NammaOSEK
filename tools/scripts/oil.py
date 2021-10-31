import os

from ob_globals import TaskParams, CntrParams, AlarmParams, ISR_Params
from ob_globals import STSZ



def print_oil_item_type1(f, task, tkey, okey, indent):
    count = len(task[tkey])
    item0 = task[tkey][0]
    if count > 1 or not (item0 == "None" or item0 == "NONE"):
        for res in task[tkey]:
            f.write(indent*"\t" +okey+" = "+ res +";\n")



def print_output(OsData, AppMode, TaskData, Counters, Alarms, ISRs):
    # open or create output file
    path = "/".join(os.path.abspath(__file__).split("/")[0:-2]) + "/oil-files"
    oil_file_name = OsData["os_name"]+"-"+OsData["cpu"]+".oil"
    full_filename = path+"/"+oil_file_name
    f = open(full_filename, "w")

    # start of CPU body (root)
    indent = 0
    f.write("CPU " + OsData["cpu"] + " {\n")
        
    # start of OS data body
    indent += 1
    f.write(indent*"\t" + "OS " + OsData["os_name"] + " {\n")
    indent += 1
    f.write(indent*"\t" + "STATUS = " + OsData["status"] + ";\n")
    f.write(indent*"\t" + "STARTUPHOOK = " + OsData["strthk"] + ";\n")
    f.write(indent*"\t" + "ERRORHOOK = " + OsData["errhk"] + ";\n")
    f.write(indent*"\t" + "SHUTDOWNHOOK = " + OsData["shdnhk"] + ";\n")
    f.write(indent*"\t" + "PRETASKHOOK = " + OsData["pretskhk"] + ";\n")
    f.write(indent*"\t" + "POSTTASKHOOK = " + OsData["postskhk"] + ";\n")
    f.write(indent*"\t" + "USEGETSERVICEID = " + OsData["ugtsrvid"] + ";\n")
    f.write(indent*"\t" + "USEPARAMETERACCESS = " + OsData["uparacc"] + ";\n")
    f.write(indent*"\t" + "USERESSCHEDULER = " + OsData["uressch"] + ";\n")
    indent -= 1
    f.write(indent*"\t" + "};\n")

    # write tasks
    for task in TaskData:
        # Start of TaskData body
        f.write(indent*"\t"+ "TASK " + task[TaskParams[0]] + " {\n")
        indent += 1
        f.write(indent*"\t" +"PRIORITY = " + str(task[TaskParams[1]]) + ";\n")
        f.write(indent*"\t" +"SCHEDULE = " + task[TaskParams[2]] + ";\n")
        f.write(indent*"\t" +"ACTIVATION = " + str(task[TaskParams[3]]) + ";\n")

        # AUTOSTART and APPMODEs
        appmodes = len(task[TaskParams[4]])
        autostrt = task[TaskParams[4]][0]
        if appmodes == 1 and (autostrt == "None" or autostrt == "NONE"):
	        f.write(indent*"\t" +"AUTOSTART = FALSE;\n")
        else:
            f.write(indent*"\t" +"AUTOSTART = TRUE {\n")
            indent += 1
            for mode in task[TaskParams[4]]:
                f.write(indent*"\t" +"APPMODE = "+ AppMode[int(mode)-1] +";\n")
            indent -= 1
            f.write(indent*"\t" + "};\n")
        
        # type1 == like Resource, Event, Message
        print_oil_item_type1(f, task, TaskParams[5], 'RESOURCE', indent)
        print_oil_item_type1(f, task, TaskParams[6], 'EVENT', indent)
        print_oil_item_type1(f, task, TaskParams[7], 'MESSAGE', indent)
            
        # End of TaskData body
        indent -= 1
        f.write(indent*"\t" + "};\n")
    
    # Start of COUNTER
    for cntr in Counters:
        # Start of Counters
        f.write(indent*"\t"+ "COUNTER " + cntr[CntrParams[0]] + " {\n")
        indent += 1
        f.write(indent*"\t" +"MINCYCLE = " + str(cntr[CntrParams[1]]) + ";\n")
        f.write(indent*"\t" +"MAXALLOWEDVALUE = " + str(cntr[CntrParams[2]]) + ";\n")
        f.write(indent*"\t" +"TICKSPERBASE = " + str(cntr[CntrParams[3]]) + ";\n")
        f.write(indent*"\t" +"TICKDURATION = " + str(cntr[CntrParams[4]]) + "; /* nsec */\n")
        # End of Counters
        indent -= 1
        f.write(indent*"\t" + "};\n")

    # Start of ALARM
    for alrm in Alarms:
        # Start of Alarms body
        f.write(indent*"\t"+ "ALARM " + alrm[AlarmParams[0]] + " {\n")
        indent += 1
        f.write(indent*"\t" +"COUNTER = " + str(alrm[AlarmParams[1]]) + ";\n")
        # Start of Actions
        f.write(indent*"\t"+ "ACTION = " + alrm[AlarmParams[2]] + " {\n")
        indent += 1
        if alrm[AlarmParams[2]] == "ACTIVATETASK":
	        f.write(indent*"\t" +"TASK = " + str(alrm[AlarmParams[3]]) + ";\n")
        if alrm[AlarmParams[2]] == "SETEVENT":
	        f.write(indent*"\t" +"TASK = " + str(alrm[AlarmParams[3]]) + ";\n")
	        f.write(indent*"\t" +"EVENT = " + str(alrm[AlarmParams[4]]) + ";\n")
        if alrm[AlarmParams[2]] == "ALARMCALLBACK":
	        f.write(indent*"\t" +"ALARMCALLBACKNAME = \"" + str(alrm[AlarmParams[3]]) + "\";\n")
        # End of Actions
        indent -= 1
        f.write(indent*"\t" + "};\n")

        if alrm[AlarmParams[5]] == False:
            f.write(indent*"\t" +"AUTOSTART = FALSE;\n")
        else:
            # Start of Autostart
            f.write(indent*"\t"+ "AUTOSTART = TRUE {\n")
            indent += 1
            f.write(indent*"\t" +"ALARMTIME = " + str(alrm[AlarmParams[6]]) + ";\n")
            f.write(indent*"\t" +"CYCLETIME = " + str(alrm[AlarmParams[7]]) + ";\n")
            alrm_app_modes = str(alrm[AlarmParams[8]]).split(',')
            for mode in alrm_app_modes:
                f.write(indent*"\t" +"APPMODE = " + str(AppMode[int(mode)-1]) + ";\n")
            # End of Autostart
            indent -= 1
            f.write(indent*"\t" + "};\n")
        # End of Alarms body
        indent -= 1
        f.write(indent*"\t" + "};\n")

    # Start of ISRs
    for isr in ISRs:
        # Start of ISR body 
        f.write(indent*"\t"+ "ISR " + isr[ISR_Params[0]] + " {\n")
        indent += 1
        f.write(indent*"\t" +"CATEGORY = " + str(isr[ISR_Params[1]]) + ";\n")
        if isr[ISR_Params[2]] != None:
	        f.write(indent*"\t" +"RESOURCE = " + str(isr[ISR_Params[2]]) + ";\n")
        if isr[ISR_Params[3]] != None:
	        f.write(indent*"\t" +"MESSAGE = " + str(isr[ISR_Params[3]]) + ";\n")
        # End of ISR body
        indent -= 1
        f.write(indent*"\t" + "};\n")

    # Print FreeOSEK_PARAMS
    f.write(indent*"\n\t"+ "FreeOSEK_PARAMS {\n")
    indent += 1
    # Start of FreeOSEK_PARAMS body
    f.write(indent*"\t" +"OS_STACK_SIZE = " + str(OsData["os_stack"]) + ";\n")
    f.write(indent*"\t" +"IRQ_STACK_SIZE = " + str(OsData["irq_stack"]) + ";\n")
    task_stack_size = 0
    for task in TaskData:
        task_stack_size += int(task[TaskParams[STSZ]])
    f.write(indent*"\t" +"TASK_STACK_SIZE = " + str(task_stack_size) + ";\n")
    # End of FreeOSEK_PARAMS body
    indent -= 1
    f.write(indent*"\t" + "};\n")

    #end of CPU body (root)
    indent -= 1
    f.write(indent*"\t" + "};\n")
    f.close()
    return full_filename

