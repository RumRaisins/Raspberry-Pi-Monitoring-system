#!/bin/bash

source ./PiHealth.conf

#if [[ ! -f "$UserLog" ]]; then
#    echo "UserLog file doesn't exist."
#    touch $UserLog
#fi

# UserInfo=(`cat /etc/passwd | grep -v nologin | grep -v halt | grep -v shutdown | awk -F ":" '{printf("%s %d %d ",$1 ,$3 ,$4) }'`)
UserInfo=(`cat /etc/passwd | grep -v root | awk -F ":" '{if($3 > 999 &&\
    $3<65534) printf("%s ",$1) }'`)
for(( i = 0 ; i < ${#UserInfo[*]} ; i++ )); do
    echo ${UserInfo[i]}
done

