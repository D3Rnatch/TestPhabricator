# Import all we need !
import argparse
import socket
import pygame
import os
import time
import sys 

# Parse arguments
parser = argparse.ArgumentParser(
    description = 'This program intend to control the hovercraft for the PFE !\nIt connect to the robot from the port 5000 by default.',
    epilog = 'Exampels comming soon.')
parser.add_argument('-a', '--address', help='Ip adress of the robot to connect to. Default = localhost', default='localhost')
parser.add_argument('-p', '--port', default=5000, type=int, help='Give the port to connect to (5000 by default).')
parser.add_argument('--version', action='version', version='%(prog)s 0.2')
args = parser.parse_args()

# Initiate Pygame
pygame.init()
pygame.joystick.init()
if pygame.joystick.get_init() == False:
    print 'No no ! No joystick !'
    exit()

# Run the input listener
running = True
connected = False
compteur = 0
while running:
    # Read the joystick values
    # ############################################################
    # Init the joystick
    pygame.joystick.Joystick(0).init()
    # Just check if it's okay
    if pygame.joystick.Joystick(0).get_init() == False:
        print 'No no ! No joystick !'
        exit()

    # Flush the input
    pygame.event.pump()

    # Read the axis values
    x = pygame.joystick.Joystick(0).get_axis(0)
    y = pygame.joystick.Joystick(0).get_axis(1)
    v = pygame.joystick.Joystick(0).get_axis(2)

    # If we need to close
    if pygame.joystick.Joystick(0).get_button(0) == True :
        print('Bye bye')
        if connected == True:
            s.close()
            connected = False
        exit()

    # If we need to connect / disconnect
    if (compteur < 10):     # Just a little timer between two pressed buttons
        compteur = compteur + 1
    if ((pygame.joystick.Joystick(0).get_button(8) == True) & (compteur > 5)) :
        compteur = 0
        if connected == False:  # If not already connected, try to connect
            try:
                s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
                s.connect((args.address, args.port)) 
            except:
                print 'Error while connecting ! '
            connected = True
        else:   # If already connected, disconnect !
            s.close()
            connected = False
    # End listen joystick input
    # ###########################################################

    # Send data if connected
    if connected == True:
        s.send(str(int(x*46 + 79)) + ':' + str(int(y*46 + 79)) + ':' + str(int(v*46 + 79)))

    # Print all those data !
    # ###########################################################
    # determine real x and y in the referential
    for i in range(0, 20):
        if ((x*10 <= i - 10) & (x*10 > i -11)):
            jj = i

    for j in range(0, 20):
        if ((y*10 <= j - 10) & (y*10 > j - 11)):
            ii = j

    for vv in range(0, 20):
        if ((v*10 <= vv - 10) & (v*10 > vv - 11)):
            vvv = vv

    # Print datas !
    os.system('cls')
    print '--------------------------------------------------------------   ---'
    for i in range(0, 20):
        sys.stdout.write('-')
        for j in range(0, 20):
            if ((j == jj) & (i == ii)):
                sys.stdout.write(' x ')
            else:
                sys.stdout.write('   ')

        sys.stdout.write('-   -')
        if (vvv == i):
            sys.stdout.write('x')
        else:
            sys.stdout.write(' ')
        sys.stdout.write('-\n')
    print '--------------------------------------------------------------   ---'
    if connected == False :
        print 'Not connected'
    else:
        print 'Connected'
    # End printed datas ! #####################################################
    
    # Wait a little time
    time.sleep(0.1)	