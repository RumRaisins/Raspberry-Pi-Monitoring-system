#!/bin/bash


NowTime=`date +"%Y-%m-%d__%H%:%M%S"`

eval $(awk -F: -v sum=0 '{if($3 >= 1000 && $3 != 65534){sum+=1;printf("User["sum"]=%s",$1)}}\
	  END {printf("UserNum=%d",sum)}' /etc/passwd)

MostActiveUser=`last | cut -d " " -f 1 | grep -v root | grep -v wtmp | grep -v reboot|\
				grep "[a-zA-Z]"| sort | uniq -c | sort -k 1 -n -r | awk -v num=3\
				'{if(num>0){printf(",%s",$2);num--}}'| cut -c 2-`

eval $(awk -F : '{if($3==1000)printf("UserWithRoot=%s",$1)}' /etc/passwd)


if [[ -r /etc/sudoers ]]; then
	for (( i = 0; i < ; i++ )); do
		grep -q "${User[$i]}" /etc/sudoers
		if [[ $? -eq 0 ]]; then
			UserWithRoot="$UserWithRoot,${User[$i]}"
		fi
	done
	else 
	UserWithRoot="${UserWithRoot},Failed_with_"
fi

UserLogedIn=`w -h | awk '{printf(",%s_%s_%s",$1,$3,$2)}'|cut -c 2-`

echo "$NowTime $UserNum [${MostActiveUser}] [${UserWithRoot}] [${UserLogedIn}]"











#UserInfo=(`cat /etc/passwd | grep -v root | awk -F ":" '{if($3 > 999 &&\
#    $3<65534) printf("%s ",$1) }'`)
#for(( i = 0 ; i < ${#UserInfo[*]} ; i++ )); do
#    echo ${UserInfo[i]}
#done


#User3=(`last | cut -d " " -f 1 | grep -v root | sort | uniq -c | sort -k 1 -n -r| head -n 3|awk '{printf("%s " , $2)}'`)
#echo ${User3[0]}
#echo ${User3[1]}
#echo ${User3[2]}

#UserInfo=(`cat /etc/passwd | grep -v root | awk -F ":" '{if($3 > 999 &&\
#    $3<65534) printf("%s ",$1) }'`)
#for(( i = 0 ; i < ${#UserInfo[*]} ; i++ )); do
#    echo ${UserInfo[i]}
#done


#ExistUser=` w -h | awk '{printf(",%s_%s_%s" , $1, $2 ,$3)}'|cut -c 2-`
#echo $ExistUser
#eval $(awk -F : '{if($3==1000) printf("UserWithRoos=%s",$1)}' /etc/passwd)
