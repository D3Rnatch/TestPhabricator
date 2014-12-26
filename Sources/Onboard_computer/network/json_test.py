from station_message import *

test = messages_sol()

test.add_custom_message("test", "1 - This is a test message")
test.add_custom_message("test2", "2 - This is a second test message")
test.add_custom_message("system", "3 - This is a system message")
mstring = test.build_message((10, 11), 30, (50, 60), 0, "") 
print "Message 1 : \n", mstring, "\n"
mstring = test.build_message((10, 11), 30, (50, 60), 0, "") 
print "Message 2 : \n", mstring, "\n"
mstring = test.build_message((10, 11), 30, (50, 60), 0, "") 
print "Message 3 : \n", mstring, "\n"

x, y, t, m_t, m_c = test.decode_message('{"robot":{"X":10,"Y":10,"T":20},"message":{"type":"Test","content":"This is a test message"}}')

print "X, Y, T = ", x, ", ", y, ", ", t, "\n"
print "Message type = ", m_t, "\n"
print "Message content = ", m_c, "\n" 
