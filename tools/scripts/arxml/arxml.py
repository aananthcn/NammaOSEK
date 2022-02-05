import os
import xml.etree.ElementTree as ET


def export(path):
   if not os.path.exists(path):
      os.makedirs(path)
   
   root = ET.Element("AUTOSAR")
   tree = ET.ElementTree(root)
   arpkgs = ET.SubElement(root, "AR-PACKAGES")
   arpkg = ET.SubElement(arpkgs, "AR-PACKAGE")

   print("export.py::export in "+ path)
   ET.indent(tree, space="\t", level=0)
   tree.write(path+"/output.arxml", encoding="utf-8", xml_declaration=True)



if __name__ == '__main__':
   print("export.py::__main__")