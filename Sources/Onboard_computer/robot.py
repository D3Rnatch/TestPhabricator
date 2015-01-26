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
from Scanner_laser.scanner import Scanner
from mapping.mapping import mapping

## Robot module.
#
#  /!\ WARNING /!\ HUGE class do not, in any circumstances, look at the code (unless you need it, and in this case, good luck).
#  
#  - var x Robot x position.
#  - var y Robot y position.
#  - var tetha Robot angle.
#  - var net_module Network manager instance.
#  - var json_module Json manager instance.
#  - var serial_manager Serial manager to connect with the arduino.
#  - var scaner_module Scaner manager instance.
#  - var mapping_module Mapping manager instance.
#  - var is_running Running state.
#  - var logs Log manager object.
#  - var logs_arduino Logs arduino communication.
#  - var state_mode Current robot state mode.
#  - var ai_mode Current robot ai mode.
#
#  - var scan_angle Current Scanner angle.
#  - var STATE_SCAN Scanning state number.
#  - var STATE_MOVE Moving state number.
#  - var STATE_WAIT Waiting state number.
#  - var AI_MANUAL Ai in manual mode.
#  - var AI_AUTO Ai in auto mode.
#  - var AI_FOLLOW Ai in wall following mode.
#  - var AI_FIND Ai in finding mode.
#  - var GET_ODO Read the odometry from the arduino.
#  - var GET_READY Read the ready frame.
#  - var GET_BATTERY Read battery states.
#
class Robot:

    ## @brief Scanning state number.
    STATE_SCAN = 0 
    ## @brief Move state number.
    STATE_MOVE = 1
    ## @brief Waiting state number.
    STATE_WAIT = 2

    ## @brief Manual AI number.
    AI_MANUAL = 0
    ## @brief Auto AI number.
    AI_AUTO = 1
    ## @brief Follow walls AI number.
    AI_FOLLOW = 2
    ## @brief Find target AI number.
    AI_FIND = 3

    ## @brief Read odometry from the arduino frame ID.
    GET_ODO = 1
    ## @brief Get ready frame from the arduino frame ID.
    GET_READY = 0
    ## @brief Read battery from the arduino frame ID.
    GET_BATTERY = 2

    ## Constructor.
    #  @param self The object pointer.
    def __init__(self):
        self.x = 0
        self.y = 0
        self.tetha = 0
        self.logs = logManager()
        self.logs.set_name("system")
	self.logs_arduino = logManager()
	self.logs_arduino.set_name("arduino_com")
	self.logs_angle = logManager()
	self.logs_angle.set_name("angle")
        self.ai_mode = self.AI_MANUAL
        self.state_mode = self.STATE_WAIT
	self.scan_angle = 0

    ## Start routine for the robot.
    #  @param self The object pointer.
    def start_routine(self):
        self.logs.write_log("Start routine.")
        self.logs.write_log("Start network manager.")
        self.net_module = network(5000)
        self.net_module.start()
        self.logs.write_log("Start json parser manager.")
        self.json_module = messages_sol()
	self.logs.write_log("Start scaner manager.")
	self.scaner_module = Scanner()
	self.scaner_module.load_configs()
	self.scaner_module.start_module()
	self.logs.write_log("Start mapping manager.")
	self.mapping_module = mapping((100, 100), 10)
	self.x = 50
	self.y = 50
        self.logs.write_log("Start some variables.")
        self.is_running = True
        self.logs.write_log("Start serial manager.")
        try:
            self.serial_manager = Serial_Manager("/dev/ttyACM0", 115200)
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
        self.logs_arduino.write_log("Scan tick.")
	frame = self.send_to_arduino(self.serial_manager.create_update_scaner_frame(self.scan_angle))
	self.scan_angle = self.scan_angle + 15
	if self.scan_angle == 0 :
	    time.sleep(1)	
	time.sleep(0.1)
	dist = self.scaner_module.get_plan_distance()
	if dist > 0:
	    self.mapping_module.update_map((self.x, self.y, self.tetha), (self.scan_angle, dist))
	if self.scan_angle > 180:
	    self.scan_angle = 0
	    self.set_wait()

    ## Move tick.
    #  @param self The object pointer.
    #  @param joystick_movement Tuple of joystick informations.
    def move_tick(self, joystick_movement):
    	ret = self.read_from_arduino(self.GET_ODO)
        self.logs_arduino.write_log("Understood: " + str(ret[0]) + " : " + str(ret[1]) + " : " + str(ord(ret[2])-48))
	self.logs_angle.write_log(str(ord(ret[2])-48))
	if self.ai_mode == self.AI_MANUAL:
	    self.logs_arduino.write_log("Movement tick.")
	    data1 = joystick_movement[0]*10
	    data2 = joystick_movement[1]*10
	    data3 = joystick_movement[2]*10
	    byte1 = 0
	    byte2 = 0
	    byte3 = 0
	    byte4 = 0
	    if data3 > 100:
	    	data3 = 100
	    byte5 = data3
	    if byte5 > 126:
	        byte5 = 126
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
	    frame = self.send_to_arduino(self.serial_manager.create_move_frame(byte1, byte2, byte3, byte4, byte5))
	elif self.ai_mode == self.AI_AUTO:
	        pass

    ## Stop routine.
    #  @param self The object pointer.
    def stop_routine(self):
        self.logs.write_log("Stop routine.")
        self.logs.write_log("Stop network module.")
        self.net_module.close()
	self.logs.write_log("Stop scaner module.")
	self.scaner_module.close()
	self.logs.write_log("Save map and destroy module.")
	self.mapping_module.save("Saved_map.txt", (50, 50))
	self.mapping_module.destroy()
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
		    self.set_scanning()
                elif message[1] == "move": # set_state move
		    self.set_moving()
                elif message[1] == "wait": # set_state wait
		    self.set_wait()
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
    #  @param self The object pointer.
    def set_moving(self):
        self.logs.write_log("Set move mode.")
        self.state_mode = self.STATE_MOVE
        frame = self.send_to_arduino(self.serial_manager.create_start_frame(0))
        self.json_module.add_custom_message("state_info", "move")

    ## Set in scanning mode.
    #  @param self The object pointer.
    def set_scanning(self):
        if self.state_mode == self.STATE_MOVE:
            self.logs.write_log("Send the stop frame to robot.")
	    frame = self.send_to_arduino(self.serial_manager.create_stop_frame())
	    self.logs.write_log("Set scan mode.")
	    self.state_mode = self.STATE_SCAN
	    self.json_module.add_custom_message("state_info", "scan")

    ## Set in waiting mode.
    #  @param self The object pointer.
    def set_wait(self):
        if self.state_mode == self.STATE_MOVE:
            self.logs.write_log("Send the stop frame to robot.")
	    frame = self.send_to_arduino(self.serial_manager.create_stop_frame())
        self.logs.write_log("Set waiting mode")
        self.state_mode = self.STATE_WAIT
        self.json_module.add_custom_message("state_info", "wait")
	
    ## Send to the arduino.
    #  @param self The object pointer.
    #  @param frame The frame to send.
    #  @return Frame returned by th arduino.
    def send_to_arduino(self, frame):
        self.logs_arduino.write_log("Send: " + str(frame))
        self.serial_manager.send(str(frame))
        frame = self.serial_manager.read()
        self.logs_arduino.write_log("Received: " + str(frame) + "\n")
        return frame

    ## Read data from the arduino.
    #  @param self The object pointer.
    #  @param id Reading ID.
    #  @return Information requested (a tuple) None if nothing received.
    def read_from_arduino(self, id):
        frame = self.serial_manager.create_read_frame(id)
        self.logs_arduino.write_log("Send: " + str(frame))
        self.serial_manager.send(str(frame))
        frame2 = self.serial_manager.read()
        self.logs_arduino.write_log("Received: " + str(frame2))
        if id == self.GET_READY:
            return (frame2[1], frame2[2])
        elif id == self.GET_ODO:
            return (frame2[1], frame2[2], frame2[3])
        elif id == self.GET_BATTERY:
            return (frame2[1], frame2[2])
        else:
            return None
