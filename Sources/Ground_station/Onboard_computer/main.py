## @file main.py
#  @brief Main program for the robot.
#  @author Loic Dalloz.
#  @version 1.0
#
from robot import Robot

# Start the robot !
print "Start..."
robot = Robot()
robot.start_routine()
print "Ready.\n"

# Loop on the robot task
loop = True
while (loop):
    loop = robot.loop_function()

# Close everything.
print "exit..."
robot.stop_routine()
print "Bye bye !\n\n"
