import os
import xml.etree.ElementTree as ET


###############################################################################
# Import ECUC



def import_arxml(filepath):
   if not os.path.exists(filepath):
       print("Error: file \""+ filepath + "\" doesn't exist!")
       return
   



if __name__ == '__main__':
   print("export.py::__main__")