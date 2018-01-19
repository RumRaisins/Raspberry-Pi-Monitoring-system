#!/bin/bash

source ../PiHealth.conf

NowTime=`date +"%Y-%m-%d--%H:%M:%S"`
Processes=(`ps --sort=-%cpu -eo user,pid,%cpu,%mem,command | awk '{printf("%s %d %.2f %.2f %s\n",$1,$2,$3,$4,$5)}'`)
index=${#Processes[*]}
for(( i = 7 ; i < index ; i = i + 5 ));do
    if [[ `echo ${Processes[i]} '<' 0.5 | bc -l` -eq 1 && `echo ${Processes[i + 1]} '<' 0.5 | bc -l` -eq 1 ]]; then
        echo  "$NowTime ${Processes[i-2]} ${Processes[i-1]} ${Processes[i]}% ${Processes[i+1]}%"
    elif [[ `echo ${Processes[i]} '<' 0.3 | bc -l` -eq 1 && `echo ${Processes[i + 1]} '<' 0.3 | bc -l` -eq 1 ]]; then
        break
    fi
done
