## @file Robot.py
#  @brief Robot module.
#  @author Loic Dalloz
#  @version 1.0
#
import time
import os

from network.network import network
from network.station_message import messages_sol

## Robot module.
#  Fields :
#       - self.x        => robot x position
#       - self.y        => robot y position
#       - self.tetha    => robot angle
#
#       - self.net_module  => network manager instance
#       - self.json_module => json manager instance
#
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
        self.net_module = network(5000)
        self.net_module.start()
        self.json_module = messages_sol()
        
    ## Run the main program.
    #  @param self The object pointer.
    #  @return A boolean True if ok to continue, False if quit.
    def loop_function(self):
        # Read network message if any.
        return_value = True
        need_to_send = False
        datas = self.net_module.read()
        if datas != "" and ord(datas[0]) == 123: # If we have a '{' to start the message (a JSON message).
            r_x, r_y, r_t, r_message_t, r_message_c = self.json_module.decode_message(datas)
            if r_message_t == "system":
                if r_message_c == "stop":
                    return_value = False
                    self.json_module.add_custom_message("system", "stop")
                    need_to_send = True
                elif r_message_c == "hello":
                    self.json_module.add_custom_message("system", "hello")
                    need_to_send = True

        # Add some threatments here.
        # TODO

        # Send the infos to the base station
        #self.json_module.add_custom_message("Info", "Everything ok !")
        if need_to_send == True:
            self.net_module.send_to_base(self.json_module.build_message((self.x, self.y), self.tetha, (50, 50), 30, "test image string"))
            need_to_send = False

        return return_value

    ## Stop routine.
    #  @param self The object pointer.
    def stop_routine(self):
        self.net_module.close()
