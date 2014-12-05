## @file station_message.py
#  @brief JSON messages for the station.
#  @author Loic Dalloz.
#  @version 1.0
#
import json
import base64
import cv2
import numpy

## JSON management for the station communication
#  Fields:
#    - self.my_message_type    => message type for custom messages to send
#    - self.my_message_content => message content for next custom message to send
#
class messages_sol:
    ## Constructor.
    #  @param sel The object pointer.
    def __init__(self):
        self.my_message_type = ""
        self.my_message_content = ""

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
        message_string = "{\"robot\":{\"X\":%s,\"Y\":%s,\"R\":%s,\"batteries\":[{\"batterie\":1, \"value\":%s},{\"batterie\":2, \"value\":%s}]},\"scanner\":{\"angle\":%s,\"image\":%s},\"message\": {\"type\":\"%s\",\"content\":\"%s\"}}" % (str(position[0]), str(position[1]), str(robot_angle), str(batteries_state[0]), str(batteries_state[1]), str(scanner_angle), str(image), str(self.my_message_type), str(self.my_message_content))
        self.my_message_content = ""
        self.my_message_type = ""
        return json.dumps(message_string)

    ## Add a custom message to the next message creation.
    #  @param self The object pointer
    #  @param message_type The custom message type in a string format.
    #  @param message_content The custom message content.
    def add_custom_message(self, message_type, message_content):
        self.my_message_type = message_type
        self.my_message_content = message_content

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
    #  @return (X, Y, T, message_type, message_content).
    def decode_message(self, message):
        decoded_message = json.loads(message)
        robot_x = decoded_message['robot']['X']
        robot_y = decoded_message['robot']['Y']
        robot_t = decoded_message['robot']['T']

        message_type = decoded_message['message']['type']
        message_content = decoded_message['message']['content']

        return (robot_x, robot_y, robot_t, message_type, message_content)