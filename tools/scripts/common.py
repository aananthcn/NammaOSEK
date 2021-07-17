import colorama
from colorama import Fore, Back, Style

def print_info(text):
    print(Fore.BLUE, "\bInfo:", text, Style.RESET_ALL)