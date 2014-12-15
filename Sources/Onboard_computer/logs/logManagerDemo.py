from logManager import *
import time

print "Start demo program.\n"
logM = logManager()
logM.set_name("demolog")
logM.write_log("This is a log !")
logM.write_log("This is another log !")
time.sleep(2)
logM.write_log("2 second later log !")
print "Stop demo program.\n"