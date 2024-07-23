#! /bin/sh

NAME=camera
NAME_DEBUG=camera_debug

#取得camera进程ID
PID=`ps -x | grep $NAME | grep -v $0 | grep -v grep | awk '{print $1}'`

if [ -z $PID ]; then
echo "camera is not running."
exit 1
fi

echo "camera pid is" $PID "try to restart its..."
kill -1 $PID
sleep 1
echo "camera is restarted sucessful."
