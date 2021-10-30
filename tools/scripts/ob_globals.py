
# OSEK Builder Global Variables (OB Globals)
# ------------------------------------------
# list of column titles in TASK tab of OSEX-Builder.xlsx
TaskParams = ["Task Name", "PRIORITY", "SCHEDULE", "ACTIVATION", "AUTOSTART",
    "RESOURCE", "EVENT", "MESSAGE", "STACK_SIZE"]
TNMI = 0
PRII = 1
SCHI = 2
ACTI = 3
ATSI = 4
RESI = 5
EVTI = 6
MSGI = 7
STSZ = 8


# list of column titles in TASK tab of OSEX-Builder.xlsx
CntrParams = ["Counter Name", "MINCYCLE", "MAXALLOWEDVALUE", "TICKSPERBASE",
     "TICKDURATION"]
CNME = 0

# Column titles for Alarms
AlarmParams = ["Alarm Name", "COUNTER", "Action-Type", "arg1", "arg2", "IsAutostart",
	"ALARMTIME", "CYCLETIME", "APPMODE[]" ]
ANME = 0
ACNT = 1
AAAT = 2
AAT1 = 3
AAT2 = 4
AIAS = 5
ATIM = 6
ACYT = 7
AAPM = 8

# Column titles for ISRs
ISR_Params = ["ISR Name", "CATEGORY", "RESOURCE", "MESSAGE"]

# FreeOSEK Parameters
OS_Params = ["IRQ_STACK_SIZE", "TASK_STACK_SIZE"]


if __name__ == '__main__':
	print("OSEK Builder Globals")
