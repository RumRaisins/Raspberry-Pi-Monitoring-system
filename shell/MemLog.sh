#!/bin/bash

source ../PiHealth.conf
DyAver=$1
if [[ x"$DyAver" = x ]]; then
	exit
fi
if [[ ! -f "$MemLog" ]]; then
	echo "Log file doesn't exist."
	touch $MemLog
fi
MemValue=(`free -m | head -n 2 | tail -n 1 |awk '{printf("%d %f %d\n" , $2 , $3 , $7)}'`)
MemAverPrec=`echo "scale=1;${MemValue[1]}*100/${MemValue[0]}" | bc`
NowAver=`echo "scale=1;0.2*${MemAverPrec}+0.8*${DyAver}" | bc`
NowTime=`date +"%Y-%m-%d--%H:%M:%S"`
echo "$NowTime ${MemValue[0]}M ${MemValue[2]}M ${MemAverPrec}% ${NowAver}%" >> ${MemLog}
