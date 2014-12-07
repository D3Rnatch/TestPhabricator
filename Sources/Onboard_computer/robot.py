## @file Robot.py
#  @brief Robot module.
#  @author Loic Dalloz
#  @version 1.1
#
import time
import os

from network.network import network
from network.station_message import messages_sol

## Robot module.
#  Fields :
#       - self.x        => robot x position.
#       - self.y        => robot y position.
#       - self.tetha    => robot angle.
#
#       - self.net_module   => network manager instance.
#       - self.json_module  => json manager instance.
#
#       - self.is_running      => running state.
#       - self.need_to_send => if sending a message to the base is needed.
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
        self.is_running = True
        self.need_to_send = False
        
    ## Run the main program.
    #  @param self The object pointer.
    #  @return A boolean True if ok to continue, False if quit.
    def loop_function(self):
        # Read network message if any.
        datas = self.net_module.read()
        if datas != "" and ord(datas[0]) == 123: # If we have a '{' to start the message (a JSON message).
            r_x, r_y, r_t, r_message_t, r_message_c = self.json_module.decode_message(datas)
            self.custom_messages_eater(r_message_t, r_message_c)

        # Add some threatments here.
        # TODO

        # Send the infos to the base station
        #self.json_module.add_custom_message("Info", "Everything ok !")
        if self.need_to_send == True:
            self.net_module.send_to_base(self.json_module.build_message((self.x, self.y), self.tetha, (50, 50), 30, ""))
            self.need_to_send = False

        return self.is_running

    ## Stop routine.
    #  @param self The object pointer.
    def stop_routine(self):
        self.net_module.close()

    ## Customs messages manager.
    #  @param self The object pointer.
    #  @param message_type The message type received.
    #  @param message_content The received message.
    def custom_messages_eater(self, message_type, message_content):
        if message_type == "system":
                if message_content == "stop":
                    self.is_running = False
                    self.json_module.add_custom_message("system", "stop")
                    self.need_to_send = True
                elif message_content == "hello":
                    self.json_module.add_custom_message("system", "hello")
                    self.need_to_send = True
