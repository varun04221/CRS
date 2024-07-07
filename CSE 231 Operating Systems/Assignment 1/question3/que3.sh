#! /bin/bash

answer=""

while IFS=" " read -r ele1 ele2 ele3
do
 if [ $ele3 == "xor" ]
 then
    ele4=$((ele1^ele2))
 elif [ $ele3 == "product" ]
 then 
    ele4=$((ele1*ele2))
 else
    if [ $ele1 -ge $ele2 ]
    then
        ele4=$ele1
    else
        ele4=$ele2
    fi
 fi
 answer+="Result of $ele3 $ele1 , $ele2 : $ele4 \n"
done < $1

found=0
for ele in $(ls)
do
 if [ -d $ele ] && [ $ele == "Result" ]
 then
    found=1
 fi
done

if [ $found -eq 0 ]
then
    mkdir "Result"
fi

cd "Result"

echo -e $answer >output.txt