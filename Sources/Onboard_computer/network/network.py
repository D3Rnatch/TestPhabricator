## @file network.py
#  @brief Network manager module.
#  @author Loic Dalloz
#  @version 1.0
#
import socket
import select
import sys
import signal

## Network manager module.
#  Fields :
#     - self.base_connection => base connection used to communicate with the base.
#     - self.listenner       => socket used to listent to new connections.
#     - self.my_port            => port to be used for the communication.
#     - self.clients         => connected clients list.
#
class network:
    ## Class constructor.
    #  @param self The object pointer.
    #  @param port The port to listen.
    def __init__(self, port):
        self.my_port = port
        self.clients = []

    ## Create the connection listenner (start everything).
    #  @param self The object pointer.
    def start(self):
        try:
            self.listenner = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
            self.listenner.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
            self.listenner.bind(('', int(self.my_port)))
            self.listenner.listen(1)
            self.clients.append(self.listenner)
        except Exception, e:
            print 'Fatal error while trying to listen :\n\t', e, '\n\nExiting.\n'
            exit()

    ## Send something to the station.
    #  @param self The object pointer.
    #  @param message The message to send.
    def send_to_base(self, message):
        for s in self.clients:
            try:
                if s != self.listenner:
                    s.send(message)
            except Exception, e:
                print 'Error while sending to client :\n\t', e, '\nReseting the connection...\n'
                self.clients.remove(s)

    ## Check for some data.
    #  @param self The object pointer.
    #  @return The data received. In string format.
    def read(self):

        return_value = ""
        try:
            inputready, outputready, exceptready = select.select(self.clients, [], [], 0)
        except select.error, e:
            print 'Fatal error with select :\n\t', e, '\n\nExiting.\n'
            exit()
        except socket.error, e:
            print 'Fatal error with socket while selecting inputs :\n\t', e, '\n\nExiting\n'
            exit()
        except Exception, e:
            print 'Fatal unknown error while selecting inputs :\n\t', e, '\n\nExiting\n'
            exit()
        
        for s in inputready:
            if s == self.listenner:
                client, addr = self.listenner.accept()
                self.clients.append(client)
		print "Connection de : " + str(addr)
            else:
                try:
                    data = s.recv(1024)
                    if data:
			return_value = data
                    else:
                        self.clients.remove(s)
			print "Deconnection d'un client."
                        s.close()
                except Exception, e:
                    print 'Error while trying to read data from available socket :\n\t', e, '\n\n'

        return return_value

    ## Close the connection.
    #  @param self The object pointer.
    def close(self):
        for s in self.clients:
            s.close()

    ## Check if the station is connected.
    #  @param self The object pointer.
    #  @return True if connected else False.
    def is_connected(self):
        for s in self.clients:
            if s == self.listenner:
                return true
        return false

