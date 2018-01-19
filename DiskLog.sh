#!/bin/bash

function Usage(){
	echo "Don't need input"
}
if [[ $# -ne 0 ]]; then
#	Usage()
	exit 1
fi

source ./PiHealth.conf

if [[ $? -ne 0 ]]; then
	echo "DiskLog Source fail!"
	exit 1
fi

if [[ ! -f "$DiskLog" ]]; then
	echo "DiskLog file doesn't exist."
	touch $DiskLog
fi
NowTime=`date +"%Y-%m-%d__%H:%M:%S"`

eval $(df -T -m -x tmpfs -x devtempfs | tail -n +2|\
	awk '{printf("paramount["NR"]=%d;parleft["NR"]=%d;parname["NR"]=%s;\
	usedperc["NR"]=%s",$3,$5,$7,$6)}\
	END{printf("parnum=%d", NR)}')
echo ${parmount[1]}
for (( i = 1 ; i <= $panum; i++ )); do
	echo "$NowTime 1 ${parname[$i]} ${paramount[$i]} ${parleft[$i]}\
		${usedperc[$i]}">>$DiskLog
	DiskSum=$[ $DiskSum + ${paramount[$i]} ]
	LeftSum=$[ $LeftSum + ${parleft[$i]} ]
done
UserdPreSum=$[ (100-$LeftSum*100/DiskSum) ]
echo "$NowTime 0 disk #DiskSum $LeftSum ${UserdPreSum}%">>$DiskLog
