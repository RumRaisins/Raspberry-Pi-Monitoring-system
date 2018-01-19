#!/bin/bash


Systeminfo=(`uname -a `)
for (( i = 0 ; i < ${#Systeminfo[*]} ; ++i )); do
    echo ${Systeminfo[i]}
done

MemLeavelist=(`free -m | head -n 2 |tail -n 1| awk '{printf("%d %d",$2,$3)}'`)
echo ${MemLeavelist[0]}
echo ${MemLeavelist[1]}
MemLeave= awk '{printf("%.2f", ${MemLeavelist[1]}/${MemLeavelist[0])}' 
echo ${MemLeave}




