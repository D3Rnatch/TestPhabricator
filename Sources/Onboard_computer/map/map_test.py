from map import map

print "start...\n"

mp = map((20, 20), 10)

mp.update_map((10, 10, 0), (0, 50))
mp.save("test_map_0.txt",(10, 10))
mp.update_map((10, 10, 0), (90, 50))
mp.save("test_map_90.txt",(10, 10))
mp.update_map((10, 10, 0), (180, 50))
mp.save("test_map_180.txt",(10, 10))
mp.update_map((10, 10, 0), (45, 50))
mp.save("test_map_45.txt",(10, 10))
mp.update_map((10, 10, 0), (135, 50))

mp.save("test_map.txt",(10, 10))

print "stop.\n"

