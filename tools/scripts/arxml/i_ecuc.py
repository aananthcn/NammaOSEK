import os
import sys
import xml.etree.ElementTree as ET

# Let us use the System Generator functions to parse ARXML and Generate code
sys.path.insert(0, os.getcwd()+"/tools/scripts")
import scripts.System_Generator as sg


###############################################################################
# Import ECUC

def tag_uri_and_name(elem):
    if elem.tag[0] == "{":
        uri, ignore, tag = elem.tag[1:].partition("}")
    else:
        uri = None
        tag = elem.tag
    return uri, tag


def get_tag(elem):
   uri, tag = tag_uri_and_name(elem)
   return tag


def get_param(elem):
   tag = None
   value = None
   for item in list(elem):
      if get_tag(item) == "DEFINITION-REF":
         tag = str(item.text).split("/")[-1]
      if get_tag(item) == "VALUE":
         value = item.text
   return tag, value



def get_param_list(ctnr):
   plist = []
   for elem in list(ctnr):
      if get_tag(elem) == "PARAMETER-VALUES":
         for param in list(elem):
            if get_tag(param)[-11:] == "PARAM-VALUE":
               tag, val = get_param(param)
               item = { "tag":tag, "val":val }
               plist.append(item)
   return plist



def get_ecuc_tree(root):
   ecuc_conf = None
   container = None
   if get_tag(root) == "AUTOSAR":
      for item in list(root):
         if get_tag(item) == "AR-PACKAGES":
            for pkg in list(item):
               if get_tag(pkg) == "AR-PACKAGE":
                  for elem in list(pkg):
                     #if get_tag(elem) == "SHORT-NAME":
                     #   sg.OS_Cfgs["CPU"] = elem.text
                     if get_tag(elem) == "ELEMENTS":
                        for conf in list(elem):
                           if get_tag(conf) == "ECUC-MODULE-CONFIGURATION-VALUES":
                              ecuc_conf = conf
                              break

   for item in list(ecuc_conf):
      if get_tag(item) == "CONTAINERS":
         container = item
         break

   return ecuc_conf, container



def get_dref_from_container(ctnr):
   dref = None
   for elem in list(ctnr):
      if get_tag(elem) == "DEFINITION-REF":
         dref = elem.text
         break
   return dref


def parse_OsHooks(ctnr):
   plist = get_param_list(ctnr)
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
   plist = get_param_list(ctnr)
   for lst in plist:
      if lst["tag"] == "OsHookStackSize":
         sg.OS_Cfgs["OS_STACK_SIZE"] = lst["val"]



def parse_FreeOsekParams(ctnr):
   plist = get_param_list(ctnr)
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
      if get_tag(elem) == "PARAMETER-VALUES":
         plist = get_param_list(ctnr)
         for lst in plist:
            if lst["tag"] == "OsStatus":
               sg.OS_Cfgs["STATUS"] = lst["val"]
      if get_tag(elem) == "SUB-CONTAINERS":
         for ctnr in list(elem):
            dref = get_dref_from_container(ctnr)
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
      dref = get_dref_from_container(cv)
      if dref == "/AUTOSAR/EcucDefs/Os/OsOS":
         parse_oscfg(cv)
   


def import_arxml(filepath):
   if not os.path.exists(filepath):
       print("Error: file \""+ filepath + "\" doesn't exist!")
       return
   parse_arxml(filepath)   



if __name__ == '__main__':
   print("export.py::__main__")