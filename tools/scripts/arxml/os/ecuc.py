import os
import sys
import xml.etree.ElementTree as ET

# Let us use the System Generator functions to parse ARXML and Generate code
sys.path.insert(0, os.getcwd()+"/tools/scripts")
import scripts.System_Generator as sg
import scripts.arxml.lib as lib


###############################################################################
# Import ECUC

def get_ecuc_tree(root):
   ecuc_conf = None
   container = None
   if lib.get_tag(root) == "AUTOSAR":
      for item in list(root):
         if lib.get_tag(item) == "AR-PACKAGES":
            for pkg in list(item):
               if lib.get_tag(pkg) == "AR-PACKAGE":
                  for elem in list(pkg):
                     if lib.get_tag(elem) == "ELEMENTS":
                        for conf in list(elem):
                           if lib.get_tag(conf) == "ECUC-MODULE-CONFIGURATION-VALUES":
                              ecuc_conf = conf
                              break

   for item in list(ecuc_conf):
      if lib.get_tag(item) == "CONTAINERS":
         container = item
         break

   return ecuc_conf, container



def parse_OsHooks(ctnr):
   plist = lib.get_param_list(ctnr)
   for lst in plist:
      if lst["tag"] == "OsErrorHook":
         sg.OS_Cfgs["ERRORHOOK"] = lst["val"]
      if lst["tag"] == "OsPostTaskHook":
         sg.OS_Cfgs["POSTTASKHOOK"] = lst["val"]
      if lst["tag"] == "OsPreTaskHook":
         sg.OS_Cfgs["PRETASKHOOK"] = lst["val"]
      if lst["tag"] == "OsShutdownHook":
         sg.OS_Cfgs["SHUTDOWNHOOK"] = lst["val"]
      if lst["tag"] == "OsStartupHook":
         sg.OS_Cfgs["STARTUPHOOK"] = lst["val"]
      if lst["tag"] == "OsProtectionHook":
         sg.OS_Cfgs["OsProtectionHook"] = lst["val"]



def parse_OsHookStack(ctnr):
   plist = lib.get_param_list(ctnr)
   for lst in plist:
      if lst["tag"] == "OsHookStackSize":
         sg.OS_Cfgs["OS_STACK_SIZE"] = lst["val"]



def parse_FreeOsekParams(ctnr):
   plist = lib.get_param_list(ctnr)
   for lst in plist:
      if lst["tag"] == "OsName":
         sg.OS_Cfgs["OS"] = lst["val"]
      if lst["tag"] == "CpuName":
         sg.OS_Cfgs["CPU"] = lst["val"]
      if lst["tag"] == "IrqStackSize":
         sg.OS_Cfgs["IRQ_STACK_SIZE"] = lst["val"]
      if lst["tag"] == "ContextSaveSize":
         sg.OS_Cfgs["OS_CTX_SAVE_SZ"] = lst["val"]
      if lst["tag"] == "AppTasksSize":
         sg.OS_Cfgs["TASK_STACK_SIZE"] = lst["val"]



def parse_oscfg(ctnr):
   for elem in list(ctnr):
      if lib.get_tag(elem) == "PARAMETER-VALUES":
         plist = lib.get_param_list(ctnr)
         for lst in plist:
            if lst["tag"] == "OsStatus":
               sg.OS_Cfgs["STATUS"] = lst["val"]
      if lib.get_tag(elem) == "SUB-CONTAINERS":
         for ctnr in list(elem):
            dref = lib.get_dref_from_container(ctnr)
            if dref == "/AUTOSAR/EcucDefs/Os/OsOS/OsHooks":
               parse_OsHooks(ctnr)
            if dref == "/AUTOSAR/EcucDefs/Os/OsOS/OsHookStack":
               parse_OsHookStack(ctnr)
            if dref == "/AUTOSAR/EcucDefs/Os/OsOS/FreeOsekParams":
               parse_FreeOsekParams(ctnr)



def parse_arxml(filepath):
   tree = ET.parse(filepath)
   root = tree.getroot()
   modconf, cntainr = get_ecuc_tree(root)
   for cv in cntainr:
      dref = lib.get_dref_from_container(cv)
      if dref == "/AUTOSAR/EcucDefs/Os/OsOS":
         parse_oscfg(cv)
      if dref == "/AUTOSAR/EcucDefs/Os/OsAppMode":
         tag, val = lib.get_param(cv)
         print(tag)



if __name__ == '__main__':
   print("import.py::__main__")