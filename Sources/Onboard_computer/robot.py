## @file Robot.py
#  @brief Robot module.
#  @author Loic Dalloz
#  @version 1.2
#
import time
import os

from network.network import network
from network.serial_manager import *
from network.station_message import messages_sol
from logs.logManager import logManager

## Robot module.
#  Fields :
#       - self.x        => robot x position.
#       - self.y        => robot y position.
#       - self.tetha    => robot angle.
#
#       - self.net_module     => network manager instance.
#       - self.json_module    => json manager instance.
#	- self.serial_manager => serial manager to connect with the arduino
#
#       - self.is_running       => running state.
#
#	- self.logs => log manager object
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
        self.ai_mode = self.AI_MANUAL
        self.state_mode = self.STATE_WAIT

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
	self.logs.write_log("Start serial manager.")
	try:
	    self.serial_manager = Serial_Manager("/dev/ttyACM0", 9600)
	except Exception, e:
	    print "Connection to arduino failed !\nSee logs file."
	    self.logs.write_log("Fatal error: connection to arduino failed :\n\t" + str(e))
	    exit()
        self.logs.write_log("End start routine.")

        
    ## Run the main program.
    #  @param self The object pointer.
    #  @return A boolean True if ok to continue, False if quit.
    def loop_function(self):
        # Read network message if any.
	received = False
	r_x = 0
	r_y = 0
	r_t = 0
        datas = self.net_module.read()
        if datas != "" and ord(datas[0]) == 123: # If we have a '{' to start the message (a JSON message).
            r_x, r_y, r_t, r_messages = self.json_module.decode_message(datas)
            self.custom_messages_eater(r_messages)
	    received = True

        # Add some threatments here.
	if self.state_mode == self.STATE_SCAN:
	    self.scan_tick()    
	elif self.state_mode == self.STATE_MOVE:
	    if received == True:
	        self.move_tick((r_x, r_y, r_t))

        # Send the infos to the base station
        #self.json_module.add_custom_message("Info", "Everything ok !")
        if self.json_module.get_pending_messages() > 0:
            self.net_module.send_to_base(self.json_module.build_message((self.x, self.y), self.tetha, (50, 50), 30, "") + " ")

        return self.is_running

    ## Scan tick.
    #  @param self The object pointer.
    def scan_tick(self):
	pass

    ## Move tick.
    #  @param self The object pointer.
    #  @param joystick_movement Tuple of joystick informations.
    def move_tick(self, joystick_movement):
	if self.ai_mode == self.AI_MANUAL:
	    data1 = joystick_movement[0]*10
	    data2 = joystick_movement[1]*10
	    data3 = joystick_movement[2]*10
	    byte1 = 0
	    byte2 = 0
	    byte3 = 0
	    byte4 = 0
	    byte5 = data3
	    if data1 > 100:
		byte1 = data1-100
		byte4 = data1-100
	    elif data1 < 100:
		byte2 = 100-data1
		byte3 = 100-data1
	    if data2 > 100:
		byte1 = data2-100
		byte2 = data2-100
	    elif data2 < 100:
		byte4 = 100 - data2
		byte3 = 100 - data2
	    self.serial_manager.send(str(self.serial_manager.create_move_frame(byte1, byte2, byte3, byte4, byte5)))
	elif self.ai_mode == self.AI_AUTO:
	    pass

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
    def custom_messages_eater(self, messages):
        for message in messages:
            if message[0] == "system":
                if message[1] == "stop": # system stop
                    self.logs.write_log("Stop message received")
                    self.is_running = False
                    self.json_module.add_custom_message("system", "stop")
                elif message[1] == "hello": # system hello
                    self.logs.write_log("Hello message received ! Respond hello.")
                    self.json_module.add_custom_message("system", "hello")
                    self.send_state()
                    self.send_ai()
            elif message[0] == "set_state":
                if message[1] == "scan": # set_state scan
                    if self.state_mode == self.STATE_MOVE:
                        self.logs.write_log("Send the stop frame to robot.")
                        self.serial_manager.send(str(self.serial_manager.create_stop_frame()))
                    self.logs.write_log("Set scan mode (from network)")
                    self.state_mode = self.STATE_SCAN
                    self.json_module.add_custom_message("state_info", "scan")
                elif message[1] == "move": # set_state move
		            self.set_moving()
                elif message[1] == "wait": # set_state wait
                    if self.state_mode == self.STATE_MOVE:
                        self.logs.write_log("Send the stop frame to robot.")
                        self.serial_manager.send(str(self.serial_manager.create_stop_frame()))
                    self.logs.write_log("Set waiting mode (from network)")
                    self.state_mode = self.STATE_WAIT
                    self.json_module.add_custom_message("state_info", "wait")
            elif message[0] == "set_ai":
                if message[1] == "manual": # set_ai manual
                    self.logs.write_log("Set manual AI (from network)")
                    self.ai_mode = self.AI_MANUAL
                    self.json_module.add_custom_message("ai_info", "manual")
                elif message[1] == "auto": # set_ai auto
                    self.logs.write_log("Set auto AI (from network)")
                    self.ai_mode = self.AI_AUTO
                    self.json_module.add_custom_message("ai_info", "auto")
                elif message[1] == "follow_wall": # set_ai follow_wall
                    self.logs.write_log("Set follow AI (from network)")
                    self.ai_mode = self.AI_FOLLOW
                    self.json_module.add_custom_message("ai_info", "follow_wall")
                elif message[1] == "find_target": # set_ai find_target
                    self.logs.write_log("Set find AI (from network)")
                    self.ai_mode = self.AI_FIND
                    self.json_module.add_custom_message("ai_info", "find_target")
            elif message[0] == "get":
                if message[1] == "state": # get state
		            self.send_state()
                elif message[1] == "ai": # get ai
		            self.send_ai()

    ## Send current state to the network
    #  @param self The object pointer.
    def send_state(self):
        self.logs.write_log("Return current state to network")
        if self.state_mode == self.STATE_SCAN:
            self.json_module.add_custom_message("state_info", "scan")
        elif self.state_mode == self.STATE_MOVE:
            self.json_module.add_custom_message("state_info", "move")
        elif self.state_mode == self.STATE_WAIT:
            self.json_module.add_custom_message("state_info", "wait")

    ## Send ai state to the network
    #  @param self The object pointer.
    def send_ai(self):
        self.logs.write_log("Return current AI to network")
        if self.ai_mode == self.AI_MANUAL:
            self.json_module.add_custom_message("ai_info", "manual")
        elif self.ai_mode == self.AI_AUTO:
            self.json_module.add_custom_message("ai_info", "auto")
        elif self.ai_mode == self.AI_FOLLOW:
            self.json_module.add_custom_message("ai_info", "follow_wall")
        elif self.ai_mode == self.AI_FIND:
            self.json_module.add_custom_message("ai_info", "find_target")

    ## Set in moving mode.
    #  @param The object pointer.
    def set_moving(self):
        self.logs.write_log("Set move mode (from network)")
        self.state_mode = self.STATE_MOVE
        self.serial_manager.send(str(self.serial_manager.create_start_frame(0)))
        self.json_module.add_custom_message("state_info", "move")