#!/bin/bash

deb=1000
inc=1000
fin=61000

echo "" > plot.txt

i=$deb
while (( i <\ fin))
do
	echo "10 " $i " 3" | ./gen > test.txt
	
	./optim < test.txt
	opt=`cat out.txt`
	./bicriteria < test.txt
	bic=`cat out.txt`
	
	dif=`echo "$opt $bic" | ./add`
	echo $dif > f.txt
	sed -i 's/\./,/g' f.txt
	difv=`cat f.txt`
	echo $i,\"$difv\" >> plot.txt
	
	i=`expr $i + $inc`
done;
