#!/bin/bash
ifconfig eth0 | grep "inet addr"
cd Sources/Ground_station/Onboard_computer
python main.py
