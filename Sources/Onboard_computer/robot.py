## @file Robot.py
#  @brief Robot module.
#  @author Loic Dalloz
#  @version 1.2
#
import time
import os

from network.network import network
from network.station_message import messages_sol
from logs.logManager import logManager

## Robot module.
#  Fields :
#       - self.x        => robot x position.
#       - self.y        => robot y position.
#       - self.tetha    => robot angle.
#
#       - self.net_module   => network manager instance.
#       - self.json_module  => json manager instance.
#
#       - self.is_running       => running state.
#       - self.need_to_send     => if sending a message to the base is needed.
#
#	    - self.logs => log manager object
#
#       - self.state_mode   => robot mode.
#       - self.ai_mode      => robot ai mode.
#
#  Defines :
#       - self.STATE_SCAN   => scanning state number.
#       - self.STATE_MOVE   => moving state number.
#       - self.STATE_WAIT   => waiting state number.
#
#       - self.AI_MANUAL    => ai in manual mode.
#       - self.AI_AUTO      => ai in auto mode.
#       - self.AI_FOLLOW    => ai in wall following mode.
#       - self.AI_FIND      => ai in finding mode.
#
class Robot:
    # defines :
    STATE_SCAN = 0
    STATE_MOVE = 1
    STATE_WAIT = 2

    AI_MANUAL = 0
    AI_AUTO = 1
    AI_FOLLOW = 2
    AI_FIND = 3

    ## Constructor.
    #  @param self The object pointer.
    def __init__(self):
        self.x = 0
        self.y = 0
        self.tetha = 0
        self.logs = logManager()
        self.logs.set_name("system")
        self.ai_mode = AI_MANUAL
        self.state_mode = STATE_WAIT

    ## Start routine for the robot.
    #  @param self The object pointer.
    def start_routine(self):
        self.logs.write_log("Start routine.")
        self.logs.write_log("Start network manager.")
        self.net_module = network(5000)
        self.net_module.start()
        self.logs.write_log("Start json parser manager.")
        self.json_module = messages_sol()
        self.logs.write_log("Start some variables.")
        self.is_running = True
        self.need_to_send = False
        self.logs.write_log("End start routine.")

        
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
        self.logs.write_log("Stop routine.")
        self.logs.write_log("Stop network module.")
        self.net_module.close()
        self.logs.write_log("Stop.")

    ## Customs messages manager.
    #  @param self The object pointer.
    #  @param message_type The message type received.
    #  @param message_content The received message.
    def custom_messages_eater(self, message_type, message_content):
        if message_type == "system":
            if message_content == "stop": # system stop
                self.is_running = False
                self.json_module.add_custom_message("system", "stop")
                self.need_to_send = True
            elif message_content == "hello": # system hello
                self.json_module.add_custom_message("system", "hello")
                self.need_to_send = True
        elif message_type == "set_state":
            if message_content == "scan": # set_state scan
                self.state_mode = STATE_SCAN
                self.json_module.add_custom_message("state_info", "scan")
                self.need_to_send = True
            elif message_content == "move": # set_state move
                self.state_mode = STATE_MOVE
                self.json_module.add_custom_message("state_info", "move")
                self.need_to_send = True
            elif message_content == "wait": # set_state wait
                self.state_mode = STATE_WAIT
                self.json_module.add_custom_message("state_info", "wait")
                self.need_to_send = True
        elif message_type == "set_ai":
            if message_content == "manual": # set_ai manual
                self.ai_mode = AI_MANUAL
                self.json_module.add_custom_message("ai_info", "manual")
                self.need_to_send = True
            elif message_content == "auto": # set_ai auto
                self.ai_mode = AI_AUTO
                self.json_module.add_custom_message("ai_info", "auto")
                self.need_to_send = True
            elif message_content == "follow_wall": # set_ai follow_wall
                self.ai_mode = AI_FOLLOW
                self.json_module.add_custom_message("ai_info", "follow_wall")
                self.need_to_send = True
            elif message_content == "find_target": # set_ai find_target
                self.ai_mode = AI_FIND
                self.json_module.add_custom_message("ai_info", "find_target")
                self.need_to_send = True
        elif message_type == "get":
            if message_content == "state": # get state
                if self.state_mode == STATE_SCAN:
                    self.json_module.add_custom_message("state_info", "scan")
                    self.need_to_send = True
                elif self.state_mode == STATE_MOVE:
                    self.json_module.add_custom_message("state_info", "move")
                    self.need_to_send = True
                elif self.state_mode == STATE_WAIT:
                    self.json_module.add_custom_message("state_info", "wait")
                    self.need_to_send = True
            elif message_content == "ai": # get ai
                if self.ai_mode == AI_MANUAL:
                    self.json_module.add_custom_message("ai_info", "manual")
                    self.need_to_send = True
                elif self.ai_mode == AI_AUTO:
                    self.json_module.add_custom_message("ai_info", "auto")
                    self.need_to_send = True
                elif self.ai_mode == AI_FOLLOW:
                    self.json_module.add_custom_message("ai_info", "follow_wall")
                    self.need_to_send = True
                elif self.ai_mode == AI_FIND:
                    self.json_module.add_custom_message("ai_info", "find_target")
                    self.need_to_send = True