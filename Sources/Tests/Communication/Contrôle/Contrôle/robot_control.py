# Import some important lib
import socket
import serial
import argparse

# Parse arguments
parser = argparse.ArgumentParser(description='This is just the control program for the hovercraft !\nIt accept a connection on the port 5000 by default to get control stream from the sol station end send everything on the arduino.',
                                 epilog='Examples comming soon !')
parser.add_argument('-a', '--address', 
                    help='Give the adress of the robot on the network (localhost if it runs in local). Default = localhost',
                    default='localhost')
parser.add_argument('-p', 
                    '--port', 
                    type=int, 
                    help='Give the port to lisent for connection (5000 by default).', 
                    default=5000)
parser.add_argument('-s',
                    '--serial',
                    help='Give the name of the serial to use (default /dev/ttyACM0).',
                    default='/dev/ttyACM0')
parser.add_argument('-b',
                    '--bauds',
                    type=int,
                    help='Get bauds for communication with the arduino (default 9600).',
                    default=9600)
parser.add_argument('-d',
                    '--debug',
                    help='Debug mode.',
                    default=False,
                    action='store_true')
parser.add_argument('--version', 
                    action='version', 
                    version='%(prog)s 0.2')
args = parser.parse_args()

# Init socket
s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.bind((args.address, args.port))

# Init serial
ser = serial.Serial(args.serial, args.bauds)

# Waiting for 1 connection
s.listen(1)
while(1):
    (client, addr) = s.accept()
    run = True
    if args.debug:
        print 'Someone connected from ' + str(addr) + ':' + str(args.port) 

    # Run the app when someone is connected !
    while(run):
        # Get the data
        data = client.recv(1024)

        # If disconnected
        if not data:
            run = False
            client.close()
            if args.debug:
                print 'Disconnected !'
        else:
            # Process it
            datas = data.split(':')
            x = int(datas[0])
            y = int(datas[1])
            z = int(datas[2])

            # print those datas if debug
            if args.debug:
                print '\tx = ' + str(x) + '\n\ty = ' + str(y) + '\n\tz = ' + str(z) + '\n'

            # Format datas before send it to the Arduino
            x2 = chr(x)
            y2 = chr(y)
            z2 = chr(z)

            # Send this to the robot !
            if args.debug:
                print 'It gives : ' + x2 + y2 + z2 + '\n<-->\n\n'
            ser.write(x2 + y2 + z2 + '\n')

# If exit needed
exit()