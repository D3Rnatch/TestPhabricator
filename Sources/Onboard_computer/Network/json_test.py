from station_message import *

test = messages_sol()

test.add_custom_message("test", "This is a test message")
mstring = test.build_message((10, 11), 30, (50, 60), 20, "test image string") 
print mstring, "\n"

x, y, t, m_t, m_c = test.decode_message('{"robot":{"X":10,"Y":10,"T":20},"message":{"type":"Test","content":"This is a test message"}}')

print "X, Y, T = ", x, ", ", y, ", ", t, "\n"
print "Message type = ", m_t, "\n"
print "Message content = ", m_c, "\n" 
