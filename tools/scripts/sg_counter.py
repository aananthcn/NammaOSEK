from common import *
from ob_globals import *


C_Counter_Type = "typedef struct {\n\
\tchar* name;\n\
\tu32 mincycle;\n\
\tu32 maxallowedvalue;\n\
\tu32 ticksperbase;\n\
\tu32 tickduration;\n\
} OsCounterType;\n\n"

def generate_code(path, Counters):
    print_info("Generating code for counters")

    # create header file
    filename = path + "/" + "sg_counter.h"
    f = open(filename, "w")
    f.write("#ifndef ACN_OSEK_SG_COUNTER_H\n")
    f.write("#define ACN_OSEK_SG_COUNTER_H\n")
    f.write("#include <osek.h>\n")
    f.write("\n\n#define OS_MAX_COUNTERS    ("+str(len(Counters))+")\n\n")
    f.write(C_Counter_Type)
    f.write("\n\n#endif\n")
    f.close

    # create source file
    filename = path + "/" + "sg_counter.c"
    f = open(filename, "w")
    f.write("#include \"sg_counter.h\"\n")
    f.write("\n\nOsCounterType OsCounters[] =  {\n")
    for i, cntr in enumerate(Counters):
        f.write("\t{\n")
        f.write("\t\t.name = \""+ cntr[CntrParams[0]] + "\",\n")
        f.write("\t\t.mincycle = "+ str(cntr[CntrParams[1]]) + ",\n")
        f.write("\t\t.maxallowedvalue = "+ str(cntr[CntrParams[2]]) + ",\n")
        f.write("\t\t.ticksperbase = "+ str(cntr[CntrParams[3]]) + ",\n")
        f.write("\t\t.tickduration = "+ str(cntr[CntrParams[4]]) + "\n")
        f.write("\t}")
        if i+1 == len(Counters):
            f.write("\n")
        else:
            f.write(",\n")
    f.write("};")
    f.close
