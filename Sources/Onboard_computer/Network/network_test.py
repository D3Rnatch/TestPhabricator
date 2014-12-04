from network import *

test = network(5000)

test.start()

while 1:
    data = test.read()
    if data != "":
        if ord(data[0]) != 13:
            print 'Data received : ', data
        test.send_to_base("ok !")