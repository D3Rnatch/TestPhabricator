## @file mapping.py
#  @brief Map manager module.
#  @author Loic Dalloz
#  @version 1.0
#
from numpy import *
import math

## Map management for the robot and its environment.
#  Fields :
#       - self.saved_map	=> The map array.
#       - self.granularity	=> Map granularity (in cm)
#
class mapping:
    ## Constructor.
    #  @param self The object pointer.
    #  @param dimensions Dimension of th map. A tuple with width and length of the map.
    #  @param new_granularity Set the map granularity (in cm).
    def __init__(self, dimensions, new_granularity):
        self.saved_map = zeros(dimensions, dtype=int)
        self.granulatity = new_granularity

    ## Signal a hit in an area.
    #  @param self The object pointer.
    #  @param hit_area The hit area (a tuple).
    def signal_a_hit(self, hit_area):
        self.saved_map.itemset(hit_area, 10)

    ## Get an area informations.
    #  @param self The object pointer.
    #  @param area The area (a tuple).
    #  @return Area infos.
    def get_area_infos(self, area):
        return self.saved_map.item(area)

    ## Save the map to a file.
    #  @param self The object pointer.
    #  @param file_name The file name to save in.
    def save(self, file_name, robot_position):
        save_file = open(file_name, "w")
        rows, cols = self.saved_map.shape

        for j in xrange(0, cols+2):
            save_file.write("-")

        save_file.write("\n")

        for i in xrange(0, rows):
            save_file.write("|")
            for j in xrange(0, cols):
                if self.saved_map.item(i, j) >= 10:
                    save_file.write("x")
                elif i == robot_position[0] and j == robot_position[1]:
                    save_file.write("R")
                else:
                    save_file.write(" ")
                    
            save_file.write("|\n")
                    
        for j in xrange(0, cols+2):
            save_file.write("-")
    
        save_file.write("\n")
        save_file.write("EOF\n")

    ## Update map from scanner infos.
    #  @param self The object pointer.
    #  @param robot_position The robot position (x, y, teta).
    #  @param scanner_infos The scanner infos (scanner_rotation, scanner distance).
    def update_map(self, robot_position, scanner_infos):
        angle = robot_position[2] + scanner_infos[0]
        rad = math.pi * angle / 180
        x_position = cos(rad)*scanner_infos[1]
        y_position = sin(rad)*scanner_infos[1]
        rounded_x = x_position/self.granulatity
        rounded_y = y_position/self.granulatity
        x_final = rounded_x + robot_position[1]
        y_final = rounded_y + robot_position[0]
        self.signal_a_hit((x_final, y_final))
        #print "Infos angle calc :"
        #print "\tAngle : " + str(angle) + "deg\t" + str(rad) + "rad"
        #print "\tX = " + str(x_final) + "\tY = " + str(y_final)

    ## Destroy averything.
    #  @param self The object pointer.
    def destroy(self):
        pass
