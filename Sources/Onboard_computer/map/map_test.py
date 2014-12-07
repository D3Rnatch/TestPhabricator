from map import map

print "start...\n"

mp = map((20, 20))

mp.signal_a_hit((1,1))
mp.signal_a_hit((1,2))
mp.signal_a_hit((1,3))
mp.signal_a_hit((1,4))
mp.signal_a_hit((1,5))
mp.signal_a_hit((1,6))
mp.signal_a_hit((1,7))
mp.signal_a_hit((10,15))
mp.signal_a_hit((11,16))
mp.signal_a_hit((11,17))
mp.signal_a_hit((19,19))
mp.signal_a_hit((19,18))
mp.signal_a_hit((19,17))

mp.save("test_map.txt")

print "stop.\n"

