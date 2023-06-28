#!/bin/bash

gcc -Wall -Wshadow -Winline -o FiniteMarkovChain.exe FiniteMarkovChain_main.c -lm

D1="_P"
D2="_W63"

#valori costanti utilizzati
N_STEPS=1000000
W=0.0049

#IF the first COMMAND LINE ARGUMENT IS <r(eset)>, then remove the directorys
if [[ $1 = "r" ]]
then
    echo "Forced removal of ${D1}, ${D2}, *.exe, *.png, *~"
    rm -rf $D1 , $D2, *.exe, *.png, *~
else
    echo "You can use <r> COMMAND LINE ARG to reset the DIRECTORYS";
    rm -rf *.png, *~
fi

# IF not exist the '_X/' directory, then make it
if [[ ! -d $D1 ]]
then
    echo "Creating DIRECTORY '${D1}/'"
    mkdir $D1
fi
 
#if the '_X/' directory exist but there's no FileName.dat, then remove all files in the directory
if [[ ( -d $D1 ) && ( ! ( -f "${D1}/FiniteMarkovChain_RAW.dat" ) ) ]]
then
    rm -rf ${D1}/*
    n=1000000
    echo "Directory ${D1}/: Creating '${D1}/FiniteMarkovChain_RAW. dat'"
    echo -e "\t ./FiniteMarkovChain.exe ${n} ${W} >> ${D1}/FiniteMarkovChain_RAW.dat"
    ./FiniteMarkovChain.exe $n $W > "${D1}/FiniteMarkovChain_RAW.dat"
    
    echo "Directory ${D1}/: Creating '${D1}/FiniteMarkovChain_RAW. dat'"
    echo -e "\t cat ${D1}/FiniteMarkovChain_RAW.dat | grep '#P#' | sed 's/#P#//g' > ${D1}/FiniteMarkovChain_P.dat"
    cat "${D1}/FiniteMarkovChain_RAW.dat" | grep '#P#' | sed 's/#P#//g' > "${D1}/FiniteMarkovChain_P.dat"

    echo "Directory ${D1}/: Creating '${D1}/FiniteMarkovChain_RAW. dat'"
    echo -e "\t cat ${D1}/FiniteMarkovChain_RAW.dat | grep '#PI#' | sed 's/#PI#//g' > ${D1}/FiniteMarkovChain_PI.dat"
    cat "${D1}/FiniteMarkovChain_RAW.dat" | grep '#PI#' | sed 's/#PI#//g' > "${D1}/FiniteMarkovChain_PI.dat"
fi

if [ ! -d $D2 ]
then
    echo "Creating DIRECTORY '${D2}/'"
    mkdir $D2
fi

if [[ ( -d $D2 ) && ( ! ( -f "${D2}/Final_FiniteMarkovChain_W.dat" ) ) ]]
then
    rm -f ${D2}/*
    echo "Directory ${D2}/: Creating '${D2}/Final FiniteMarkovChain_W.dat'"
    echo "# w63 (a,b=TAU,c) (a_err, b_err, c_err) " > "${D2}/Final_FiniteMarkovChain_W.dat"
    i=0
    for w in 0.0005 0.0010 0.0015 0.0020 0.0025 0.0030 0.0035 0.0040 0.0045 0.0050 0.0100 0.0150 0.0200 0.0250 0.03000 0.0350 0.0400 0.0450 0.0500 0.0600 0.0650 0.0700 0.0750 0.0800 0.0850 0.0900 0.0950 0.1000 0.2000 0.3000 0.4000 0.5000 0.6000 0.7000 0.8000 0.9000 0.9900 0.9990 0.9999
    do
	i=$((i+1))
        echo -e "\t ./FiniteMarkovChain.exe ${N_STEPS} ${w} | grep '#W63#' | sed 's/#W63#//g'  >> ${D2}/FiniteMarkovChain_W_${i}.dat"
        ./FiniteMarkovChain.exe $N_STEPS $w | grep '#W63#' | sed 's/#W63#//g' > "${D2}/FiniteMarkovChain_W_${i}.dat"
	gnuplot -e "dir='${D2}'" -e "filename='FiniteMarkovChain_W_${i}'" -e "W=$w" -e "output_file='Final_FiniteMarkovChain_W'" 'GP_fit_FiniteMarkovChain.gp' 
    done
fi

#t0="{/:Bold Finite Markov Chain: Functional relation between P(t) and t; {/Symbol t} and w_{63}} (Fixed values: T_{MAX}=${N_STEPS}, W_{63}=${W})"

gnuplot -e "dir1='${D1}'" -e "dir2='${D2}'" -e "filename1='FiniteMarkovChain_P'" -e "filename2='FiniteMarkovChain_PI'" -e "filename3='Final_FiniteMarkovChain_W'" -e "W=${W}" 'GP_multiplot_FiniteMarkovChain.gp'

xdg-open 'Graph_FiniteMarkovChain4.png'
