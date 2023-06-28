#!/bin/bash

gcc -Wall -Wshadow -Winline -o RectangularMethod.exe RectangularMethod_main.c

D1="_N"
D2="_L"

L=1.0
N=10000
#IF the first COMMAND LINE ARGUMENT IS <r(eset)>, then remove the directories
if [[ $1 = "r" ]] 
then
   echo "Forced removal of ${D1}, ${D2}, *.exe, *.png, *~"
    rm -rf $D1 , $D2, *.exe, *.png, *~
else
    echo "You can use <r> COMMAND LINE ARG to reset the DIRECTORYS";
    rm -rf *.png, *~
fi


# IF not exist the '_X/' directory, then make it
if [ ! -d $D1 ]
then
    echo "Creating DIRECTORY '${D1}/'"
    mkdir $D1
fi


#if the '_X/' directory exist but there's no Final_Document.dat, then remove all files in the directory
if [[ ( -d $D1) && ( ! ( -f "${D1}/Final_RectangularMethod.dat" )) ]]
then
    rm -f ${D1}/*
    echo "Directory ${D1}/: Creating '${D1}/RectangularMethod.dat'"
    for n in 100 250 500 750 1000 1250 1500 2000 3000 5000 7500 10000 50000 100000
    do
	echo -e "\t ./RectangularMethod.exe ${n} ${L} >> ${D1}/RectangularMethod.dat"
	./RectangularMethod.exe $n $L >> "${D1}/RectangularMethod.dat"
    done

    echo "Directory ${D1}/: Creating '${D1}/Final_RectangularMethod.dat'"
    cat "${D1}/RectangularMethod.dat" | grep "#ANS#" | sed 's/#ANS#//g' > "${D1}/Final_RectangularMethod.dat"
fi



if [ ! -d $D2 ]
then
    echo "Creating DIRECTORY '${D2}/'"
    mkdir $D2
fi

N=10000

if [[ ( -d $D2) && ( ! ( -f "${D2}/Final_RectangularMethod.dat" )) ]]
then
    rm -f ${D2}/*
    echo "Directory ${D2}/: Creating '${D2}/RectangularMethod.dat'"
    for l in 1.0 2.0 4.0 8.0 16.0 32.0
    do
	echo -e "\t ./RectangularMethod.exe ${N} ${l} >> ${D2}/RectangularMethod.dat"
	./RectangularMethod.exe $N $l >> "${D2}/RectangularMethod.dat"
    done

    echo "Directory ${D2}/: Creating '${D2}/Final_RectangularMethod.dat'"
    cat "${D2}/RectangularMethod.dat" | grep "#ANS#" | sed 's/#ANS#//g' > "${D2}/Final_RectangularMethod.dat"
fi

gnuplot -e "dir1='${D1}'" -e "dir2='${D2}'" -e "filename='Final_RectangularMethod'" "GP_multiplot_RectangularMethod.gp"

xdg-open "Graph_Final_RectangularMethod1.png"
