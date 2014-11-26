## @file Robot.py
#  @brief Robot module.
#  @author Loïc Dalloz
#  @version 1.0
#

## Robot module.
#  Fields :
#       - self.x        => robot x position
#       - self.y        => robot y position
#       - self.tetha    => robot angle
#
class Robot:
    ## Constructor.
    #  @param self The object pointer.
    def __init__(self):
        self.x = 0
        self.y = 0
        self.tetha = 0

    ## Start routine for the robot.
    #  @param self The object pointer.
    def start_routine(self):
        pass

    ## Run the main program.
    #  @param self The object pointer.
