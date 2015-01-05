## @file station_message.py
#  @brief JSON messages for the station.
#  @author Loic Dalloz.
#  @version 1.2
#
import json
import base64
import cv2
import numpy

## JSON management for the station communication
#  Fields:
#    - self.message_queue      => message to send queue 
#    - self.my_message_type    => message type for custom messages to send
#    - self.my_message_content => message content for next custom message to send
#
class messages_sol:
    ## Constructor.
    #  @param sel The object pointer.
    def __init__(self):
        self.my_message_type = ""
        self.my_message_content = ""
        self.message_queue = list()

    ## Construct the JSON message to send.
    #  @param self The object pointer.
    #  @param position The robot position in the form (x, y).
    #  @param robot_angle The robot's Z angle.
    #  @param batteries_state The batteries states in the form (batterie_1_state, batterie_2_state).
    #  @param scanner_angle The scanner's angle from the standby position.
    #  @param image The image from the scanner. Must be a string encoded in base 64 (see function encode_image()).
    #  @see encode_image()
    #  @return The message string ready to be sent.
    def build_message(self, position, robot_angle, batteries_state, scanner_angle, image):

        if image != "":
            scanner_message = ',"scanner":{"angle":%s,"image":%s}' % (str(scanner_angle), str(image))
        else:
            scanner_message = ""

        self.pop_message()
        if self.my_message_type != "":
            custom_message = ',"message":{"type":"%s","content":"%s"}' % (str(self.my_message_type), str(self.my_message_content))
        else:
            custom_message = ""

        message_string = '{"robot":{"X":%s,"Y":%s,"R":%s,"batteries":[{"batterie":1,"value":%s},{"batterie":2,"value":%s}]}%s%s}' % (str(position[0]), str(position[1]), str(robot_angle), str(batteries_state[0]), str(batteries_state[1]),scanner_message, custom_message)
        
        return message_string

    ## Add a command in the queue.
    #  @param self The object pointer
    #  @param message_type The custom message type in a string format.
    #  @param message_content The custom message content.
    def add_custom_message(self, message_type, message_content):
        if message_type == "system":
            self.message_queue.insert(0, (message_type, message_content))
        else:
            self.message_queue.append((message_type, message_content))

    ## Encode a binary image in base64.
    #  @param self The object pointer.
    #  @param image The image to be encoded.
    #  @return The encoded image in string format.
    def encode_image(self, image):
        ret, data = cv2.imencode('.jpeg', image)
        return base64.b64encode(data)

    ## Decode a message received from the station.
    #  @param self The object pointer.
    #  @param message The received string.
    #  @return (X, Y, T, messages).
    def decode_message(self, message):
        received_messages = message.split(' ')
        messages = list()
        for received_message in received_messages:
            try:
                decoded_message = json.loads(received_message)
            except Exception, e:
                print "Error while trying to load JSON : \n\t%s\n\n\tmessage :\n\t%s\n\n" % (e, received_message)
            try:
                robot_x = decoded_message['robot']['X']
                robot_y = decoded_message['robot']['Y']
                robot_t = decoded_message['robot']['T']
            except Exception, e:
                print "Error : missing %s in received message.\n\tEvery values in robot set to 0.\n\n" % (e)
                robot_x = 0
                robot_y = 0
                robot_t = 0

            try:
                message_type = decoded_message['message']['type']
                message_content = decoded_message['message']['content']
                messages.append((message_type, message_content))
            except Exception, e:
                #print "Error : missing %s in received message.\n\tEmpty message.\n\n" % (e)
                message_type = ""
                message_content = ""
       
        return (robot_x, robot_y, robot_t, messages)

    ## Pop the next message to send in built in containers (can be used to dismiss a message but it shouldn't be done).
    #  @param self The object pointer.
    def pop_message(self):
        if len(self.message_queue) > 0:
            next_message = self.message_queue.pop(0)
            self.my_message_type = next_message[0]
            self.my_message_content = next_message[1]
        else:
            self.my_message_content = ""
            self.my_message_type = ""

    ## Get pending messages number.
    #  @param self The object pointer.
    #  @return Pending messages number.
    def get_pending_messages(self):
	return len(self.message_queue)
