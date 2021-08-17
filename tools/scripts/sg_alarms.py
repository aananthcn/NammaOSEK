from common import print_info
from ob_globals import TaskParams

import colorama
from colorama import Fore, Back, Style


C_AlarmAction_Type = "\n\ntypedef enum {\n\
    AAT_ACTIVATETASK,\n\
    AAT_SETEVENT,\n\
    AAT_ALARMCALLBACK,\n\
    AAT_MAX_TYPE\n\
} AlarmActionType;\n"

C_Alarm_Type = "\n\ntypedef struct {\n\
    char* name;                     /* short name of alarm */ \n\
    AlarmType counter_id;           /* OS Counter ID (= index of OsCounters + 1) */ \n\
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

    print(Alarms)
    print("Alarms generate_code, is under construction\n")
    hf.write("\n\n#endif\n")
    hf.close()