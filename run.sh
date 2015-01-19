#!/bin/bash

if [ "$1" = "computer" ]; then
	echo "*****************************"
	echo "**  Run onboard computer.  **"
	echo "*****************************"
	echo ""
	cd Sources/Onboard_computer
	python main.py
	cd ../..
	echo ""
	echo "***********************"
	echo "**  Computer ended.  **"
	echo "***********************"
	echo ""
elif [ "$1" = "calibrate" ]; then
	echo "************************"
	echo "**  Run calibration.  **"
	echo "************************"
	echo ""
	cd Sources/Onboard_computer/Scanner_laser
	python calibration.py
	cp scan_config.txt ../
	cd ../../..
	echo ""
	echo "**************************"
	echo "**  Calibration ended.  **"
	echo "**************************"
	echo ""
elif [ "$1" = "ui" ]; then
	echo "**************************"
	echo "**  Run simplified UI.  **"
	echo "**************************"
	echo ""
	cd Sources/Ground_station
	python simplified_UI.py $2
	cd ../..
	echo ""
	echo "****************************"
	echo "**  Simplified UI ended.  **"
	echo "****************************"
	echo ""
else
	echo "Bad usage !"
	echo "Valid parameters :"
	echo "computer"
	echo "calibrate"
	echo "ui [address]"
fi
