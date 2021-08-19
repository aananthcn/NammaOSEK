from common import print_info
from ob_globals import AlarmParams, ANME, AAAT

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
    hf.write("extern AppAlarmType AppAlarms[];\n")

    # create source file
    filename = path + "/" + "sg_alarms.c"
    cf = open(filename, "w")
    cf.write("#include <stddef.h>\n")
    cf.write("#include <stdbool.h>\n")
    cf.write("#include \"sg_alarms.h\"\n")
    cf.write("\n\n/*   A L A R M S   D E F I N I T I O N S   */\n")
    cf.write("const AppAlarmType AppAlarms[] = {\n")
    for i, alarm in enumerate(Alarms):
        cf.write("\t{\n")
        cf.write("\t\t.name = "+alarm[AlarmParams[ANME]]+",\n")
        cf.write("\t\t.cntr_id = "+str(i)+",\n")
        cf.write("\t\t.aat = "+AAT_PyList[alarm[AlarmParams[AAAT]]]+",\n")

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