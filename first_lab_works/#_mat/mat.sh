#!/bin/bash


#Recursive
d()
{
	s=$1
	shift
	c=("$@")
	deter=0
	var=1
	if [ $s -eq 1 ]
		then
		det=${c[0]}
		echo $det
		return
	
	elif [ $s -eq 2 ]
		then 
		det=`expr ${c[0]} \* ${c[3]} - ${c[2]} \* ${c[1]} `
		echo $det
		return
	else
		p=` expr $s \* $s `
		for ((i=0; i< $s; ++i));
		do
		k=0
		for ((j=$s; j < $p ; ++j));
			do
			if [ ` expr $j % $s ` -ne $i  ]
				then
				b[$k]=${c[$j]}
				k=`expr $k + 1 `
			fi
	    	done
		d=`expr $s - 1 `
		de=` d $d "${b[@]}"  `
		deter=`expr $de \* ${c[$i]} \* $var  + $deter ` 
		var=`expr $var \* \-1`	
	done
	fi
echo $deter
}

echo " Enter the matrix size "
read n

#Read
for ((i = 0; i < $n; i++)); do
    for ((j = 0; j < $n; j++)); do
	echo "enter [$i,$j] element"
        read a[$((i * n + j))]
    done
done

## Output
echo "mat"

for ((i = 0; i < $n; i++)); do
    for ((j = 0; j < $n; j++)); do
        printf " %2d" ${a[$((i * n + j))]}
    done
    echo ""
done

echo " The Determinant is: "
d "$n" "${a[@]}" 





