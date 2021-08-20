from common import print_info
from ob_globals import AlarmParams, ANME, AAAT, AAT1, AAT2, AIAS, ATIM, ACYT

import colorama
from colorama import Fore, Back, Style


C_AlarmAction_Type = "\n\ntypedef enum {\n\
    AAT_ACTIVATETASK,\n\
    AAT_SETEVENT,\n\
    AAT_ALARMCALLBACK,\n\
    AAT_MAX_TYPE\n\
} AlarmActionType;\n"


AAT_PyList = {
    "ACTIVATETASK" : "AAT_ACTIVATETASK",
    "SETEVENT" : "AAT_SETEVENT",
    "ALARMCALLBACK" : "AAT_ALARMCALLBACK"
}


C_Alarm_Type = "\n\ntypedef struct {\n\
    char* name;                     /* short name of alarm */ \n\
    AlarmType cntr_id;              /* OS Counter ID (= index of OsCounters + 1) */ \n\
    AlarmActionType aat;            /* Refer enum AlarmActionType */ \n\
    void* aat_arg1;                 /* arg1: task_name | callback_fun */\n\
    void* aat_arg2;                 /* arg2: event | NULL */\n\
    bool is_autostart;              /* does this alarm start at startup? */\n\
    u32 alarmtime;                  /* when does it expire? */\n\
    u32 cycletime;                  /* cyclic time - for repetition */\n\
    const AppModeType** appmodes;   /* alarm active in which modes? */\n\
    u32 n_appmodes;                 /* how may appmodes for this entry? */\n\
} AppAlarmType;\n\n"


def generate_code(path, Alarms):
    print_info("Generating code for Alarms")

    # create header file
    filename = path + "/" + "sg_alarms.h"
    hf = open(filename, "w")
    hf.write("#ifndef ACN_OSEK_SG_ALARMS_H\n")
    hf.write("#define ACN_OSEK_SG_ALARMS_H\n")
    hf.write("#include <osek.h>\n")
    hf.write(C_AlarmAction_Type)
    hf.write(C_Alarm_Type)
    hf.write("extern const AppAlarmType AppAlarms[];\n")

    # create source file
    filename = path + "/" + "sg_alarms.c"
    cf = open(filename, "w")
    cf.write("#include <stddef.h>\n")
    cf.write("#include <stdbool.h>\n")
    cf.write("#include \"sg_alarms.h\"\n")
    cf.write("#include \"sg_appmodes.h\"\n")
    cf.write("#include \"sg_tasks.h\"\n")

    # define app mode structure and macros first
    cf.write("\n\n#define TRUE    true\n#define FALSE    false");
    cf.write("\n\n/*   A P P M O D E S   F O R   A L A R M S   */\n")
    for alarm in Alarms:
        if "APPMODE[]" in alarm:
            max_i = len(alarm["APPMODE[]"])
            cf.write("#define ALARM_"+alarm[AlarmParams[ANME]].upper()+"_APPMODES_MAX ("+str(max_i)+")\n")
            cf.write("const AppModeType Alarm_"+alarm[AlarmParams[ANME]]+"_AppModes[] = {\n")
            i = 0
            for m in alarm["APPMODE[]"]:
                i += 1
                cf.write("\t"+m)
                if i != max_i:
                    cf.write(",\n")
                else:
                    cf.write("\n")
            cf.write("};\n\n")
            hf.write("extern const AppModeType Alarm_"+alarm[AlarmParams[ANME]]+"_AppModes[];\n")

    # define the alarms configured in OSEK builder or oil file
    cf.write("\n\n/*   A L A R M S   D E F I N I T I O N S   */\n")
    cf.write("const AppAlarmType AppAlarms[] = {\n")
    for i, alarm in enumerate(Alarms):
        cf.write("\t{\n")
        cf.write("\t\t.name = \""+alarm[AlarmParams[ANME]]+"\",\n")
        cf.write("\t\t.cntr_id = "+str(i)+",\n")
        cf.write("\t\t.aat = "+AAT_PyList[alarm[AlarmParams[AAAT]]]+",\n")
        if AlarmParams[AAT1] in alarm:
            cf.write("\t\t.aat_arg1 = "+str(alarm[AlarmParams[AAT1]]).replace('"','')+",\n")
        else:
	        cf.write("\t\t.aat_arg1 = NULL,\n")
        if AlarmParams[AAT2] in alarm:
            cf.write("\t\t.aat_arg2 = "+alarm[AlarmParams[AAT2]]+",\n")
        else:
	        cf.write("\t\t.aat_arg2 = NULL,\n")
        cf.write("\t\t.is_autostart = "+alarm[AlarmParams[AIAS]]+",\n")
        if AlarmParams[ATIM] in alarm:
            cf.write("\t\t.alarmtime = "+str(alarm[AlarmParams[ATIM]])+",\n")
        else:
	        cf.write("\t\t.alarmtime = 0,\n")
        if AlarmParams[ACYT] in alarm:
            cf.write("\t\t.cycletime = "+str(alarm[AlarmParams[ACYT]])+",\n")
        else:
	        cf.write("\t\t.cycletime = 0,\n")

        if "APPMODE[]" in alarm:
            cf.write("\t\t.n_appmodes = ALARM_"+alarm[AlarmParams[ANME]].upper()+"_APPMODES_MAX,\n")
            cf.write("\t\t.appmodes = Alarm_"+alarm[AlarmParams[ANME]]+"_AppModes\n")
        else:
            cf.write("\t\t.n_appmodes = 0,\n")
            cf.write("\t\t.appmodes = NULL\n")

        cf.write("\t}")
        if i+1 < len(Alarms):
            cf.write(",\n")
        else:
            cf.write("\n")
    cf.write("};\n")
    print(Alarms)
    print("Alarms generate_code, is under construction\n")
    hf.write("\n\n#endif\n")
    hf.close()
