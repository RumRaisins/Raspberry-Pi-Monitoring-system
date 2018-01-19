#!/bin/bash


source ../PiHealth.conf
if [[ ! -f "$CpuLog" ]];then
    echo "Log file does't exist."
    touch $CpuLog
fi
NowTime=`date +"%Y-%m-%d--%H:%M:%S"`
#CpuUse=`top | head -n 3 | tail -n 1 | awk '{printf("%.1f" , $4)}'`
CpuInfo0=(`head -n 1  /proc/stat | awk -v sum=0 '{for(i=2;i<10;i++){sum+=$i}} END {printf("%d %d", sum,$5)}'`)
sleep 0.5
CpuInfo1=(`head -n 1  /proc/stat | awk -v sum=0 '{for(i=2;i<10;i++){sum+=$i}} END {printf("%d %d",sum, $5)}'`)
CpuUseSub=`echo "${CpuInfo1[0]}-${CpuInfo0[0]}"|bc`
IdleSub=`echo "${CpuInfo1[1]}-${CpuInfo0[1]}"|bc`
CpuUse=`echo "scale=3;(1-($IdleSub/$CpuUseSub))*100"|bc`
CpuTemp=`cat /sys/class/thermal/thermal_zone0/temp`
CpuTemp=`echo "scale=3;${CpuTemp}/1000" | bc`
CpuNotice="Normal"
if [[ `echo $CpuTemp '>' 50 | bc -l` -eq 1  &&  `echo $CpuTemp '<=' 70 | bc -l` ]]; then
    CpuNotice="Note"
elif [[ `echo $CpuTemp '>' 70 | bc -l` -eq 1 ]]; then
    CpuNotice="Warning!"
fi
CPUPro=`cat /proc/loadavg | awk '{printf("%s %s %s", $1 , $2 , $3)}'`
echo "$NowTime $CPUPro ${CpuUse}% ${CpuTemp}℃ $CpuNotice"  > $CpuLog
