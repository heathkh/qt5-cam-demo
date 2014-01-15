#!/bin/bash
DEVICE_SERIAL_NUM=0149C7A517015010
FOLDERNAME=/qt5-cam-demo
DEVICE=phablet@192.168.1.107
DEVICE_PORT=22

/usr/share/qtcreator/ubuntu/scripts/qtc_device_buildpackage "$DEVICE_SERIAL_NUM" "$FOLDERNAME" "$DEVICE" $DEVICE_PORT
