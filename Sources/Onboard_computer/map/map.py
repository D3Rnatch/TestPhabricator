## @file map.py
#  @brief Map manager module.
#  @author Loic Dalloz
#  @version 1.0
#
from numpy import *

## Map management for the robot and its environment.
#  Fields :
#       - self.map      => The map array.
#
class map:
    ## Constructor.
    #  @param self The object pointer.
    #  @param dimensions Dimension of th map. A tuple with width and length of the map.
    def __init__(self, dimensions):
        self.map = zeros(dimensions, dtype=int)

    ## Signal a hit in an area.
    #  @param self The object pointer.
    #  @param hit_area The hit area (a tuple).
    def signal_a_hit(self, hit_area):
        self.map.itemset(hit_area, 10)

    ## Get an area informations.
    #  @param self The object pointer.
    #  @param area The area (a tuple).
    #  @return Area infos.
    def get_area_infos(self, area):
        return self.map.item(area)

    ## Save the map to a file.
    #  @param self The object pointer.
    #  @param file_name The file name to save in.
    def save(self, file_name):
        save_file = open(file_name, "w")
        rows, cols = self.map.shape
        for i in xrange(0, rows):
            for j in xrange(0, cols):
                if self.map.item(i, j) >= 10:
                    save_file.write("x")
                else:
                    save_file.write(" ")

            save_file.write("\n")

        save_file.write("EOF\n")

    ## Destroy averything.
    #  @param self The object pointer.
    def destroy(self):
        pass