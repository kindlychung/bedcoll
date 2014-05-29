import os
from .test2 import sayhi

class sysutil():
    @staticmethod
    def modpath():
        return os.path.dirname(os.path.realpath(__file__))
    @staticmethod
    def cwd():
        return os.getcwd()
    @staticmethod
    def newsayhi():
        print("Indeed:")
        sayhi()