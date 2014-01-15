#!/bin/bash
DEVICE_SERIAL_NUM=0149C7A517015010
PROJECT_PATH=$PWD
DEVICE=phablet@192.168.1.107
DEVICE_PORT=22

#mkdir ${PROJECT_PATH}/build
/usr/share/qtcreator/ubuntu/scripts/qtc_device_buildpackage "$DEVICE_SERIAL_NUM" "$PROJECT_PATH" "$DEVICE" $DEVICE_PORT
