#!/bin/bash
# program to take in password and check its strength
#Inform the user on conditions
echo "Minimum length of 8 characters"
echo "Must contain at least one numeric character"
echo "Must contain at least one of the following non-alphabetic characters: @, #, $, %, &, *, +, -,="
echo "Dont use general words found in the dictionary"
#read input
echo "enter the password"
read PASSWORD
echo $PASSWORD
SOURCE="/usr/share/dict/"
ATLEAST4="h.txt"  #created by running following command " sed -r '/.{4,}/!d' /usr/share/dict/cracklib-small >> h.txt "
CRACKLIB="cracklib-small"
WORDS="words"
#check input
echo ${#PASSWORD}
if [[ ${#PASSWORD} -ge 8 && "$PASSWORD" == *[A-Z]* && "$PASSWORD" == *[a-z]* && "$PASSWORD" == *[0-9]* && "${PASSWORD//[^@#$%&*+=-]/}" ]]; then #Verify the char combo
#if [[ `[ -f $SOURCE$CRACKLIB ] && echo "1" || echo "0"` ]]; then #Verify if cracklib exist
#    for((i=4;i<=${#PASSWORD};i++))
#    do
#        for((j=0;j<=${#PASSWORD}-$i;j++))
#        do
#            if [ `grep -c -E -i "^${PASSWORD:$j:$i}$" $SOURCE$CRACKLIB` -gt 0 ] #verify with dictionary ( cracklib)
#            then
#                echo "$PASSWORD: Weak PASSWORD"
#                exit
#            fi
#        done
#    done
#else
#if [[ `[ -f $SOURCE$WORDS ] && echo "1" || echo "0"` ]]; then #Verify if cracklib exist
#    for((i=4;i<=${#PASSWORD};i++));
#    do
#        for((j=0;j<=${#PASSWORD}-$i;j++))
#        do
#            if [ `grep -c -E -i "^${PASSWORD:$j:$i}$" $SOURCE$WORDS` -gt 0 ] #verify with dictionary (words)
#            then
#                echo "$PASSWORD: Weak PASSWORD"
#                exit
#            fi
#        done
#    done
#else
if [[ `[ -f $ATLEAST4 ] && echo "1" || echo "0"` ]]; then #Verify if cracklib exist
    for((i=4;i<=${#PASSWORD};i++))
    do
        for((j=0;j<=${#PASSWORD}-$i;j++))
        do
            if [ `grep -c -E -i "^${PASSWORD:$j:$i}$" $ATLEAST4` -gt 0 ] #verify with dictionary (our dictionary)
            then
                echo "$PASSWORD: Weak PASSWORD"
                exit
            fi
        done
    done
    echo "$PASSWORD: Strong PASSWORD"
	else	
		echo "DICTIONARY NOT FOUND"
	fi	
else echo "WEAK PASSWORD"
fi	
