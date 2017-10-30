#!/bin/bash
CHILD_PROCESS_LIMIT=5
REFRESH_TIME=5
TIME_INDEX=`date +%s`
while true
do	for pid in `ps -ef | awk '{print $2}'`;
	do
	#echo $pid
	#handle_process_with_many_children $pid
        PARENT_ID=$pid
        CHILDREN_COUNT=0
        #echo $PARENT_ID
        #Counting the number of child process
        CHILDREN_COUNT=`pgrep -c -P ${PARENT_ID}`
        #echo $CHILDREN_COUNT
        if ((CHILDREN_COUNT > CHILD_PROCESS_LIMIT)); then #Send mail if child greater than five
        #for testing
        #echo "Parent Detail" 
        #echo "`ps --no-headers --pid $PARENT_ID -o pid,user,command`"
        #echo "Children Details"
        #echo "`ps --no-headers --ppid $PARENT_ID -o pid,user,cmd`"
        #echo ""
        if expr $((`date +%s`-TIME_INDEX > 600)) ; then #600 sec = 10 minute
        #for logging
        echo `date` >> ./process_monitor.log
        echo "Parent Detail" >> ./process_monitor.log 
        echo "`ps --no-headers --pid $PARENT_ID -o pid,user,command`" >> ./process_monitor.log 
        echo "Children Details" >> ./process_monitor.log 
        echo "`ps --no-headers --ppid $PARENT_ID -o pid,user,cmd`" >> ./process_monitor.log 
        echo "" >> ./process_monitor.log
        #mail_process_info $PARENT_ID
	TIME_INDEX=`date +%s`
        fi
     	#for mailing
        MAIL_CONTENT="Parent Detail\n`ps --no-headers --pid $PARENT_ID -o pid,user,command`\nChildren Details\n`ps --no-headers --ppid $PARENT_ID -o pid,user,fname`\n"
	echo -e $MAIL_CONTENT | mail "root@example.org" -s "Process with more than 5 child" & # mail
        echo "mailing..." #notify
        fi
	done
	sleep $REFRESH_TIME
done
