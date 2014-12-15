## @file logManager.py
#  @brief a simple log manager.
#  @author Loic Dalloz
#  @version 1.0
#
from time import gmtime, strftime


## log manager module.
#  Fields :
#       - self.fileName : log file name to write in.
#
class logManager:
    ## Constructor.
    #  @param self The object pointer.
    def __init__(self):
        self.fileName = ""

    ## Set the log name (auto add .logfile at the end).
    #  @param self The object pointer.
    #  @param name File name.
    def set_name(self, name):
        self.fileName = name + ".logfile"
        logFileToWrite = open(self.fileName, "w")
        logTime = strftime("%H:%M:%S", gmtime())
        logMessage = ("\t************************\n\t****    %s    ****\n\t************************\n\n") % (logTime)
        logFileToWrite.write(logMessage)
        logFileToWrite.close()
        
    ## Write a log in the log file.
    #  @param self The object pointer.
    #  @param log The log message.
    def write_log(self, log):
        logFileToWrite = open(self.fileName, "a")
        logTime = strftime("%H:%M:%S", gmtime())
        logMessage = ("[%s] => %s\n") % (logTime, log)
        logFileToWrite.write(logMessage)
        logFileToWrite.close()






