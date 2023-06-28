#!/bin/bash

gcc -Wall -Wshadow -Winline -o MetropolisMCMCMethod.exe MetropolisMCMCMethod_main.c -lm

D1="_N_mcmc"
D2="_N_int"
D3="_eps"

#valori costanti utilizzati
N_mcmc=500000
N_int=5000
L=1.00
eps=0.4

#IF the first COMMAND LINE ARGUMENT IS <r(eset)>, then remove the directorys
if [[ $1 = "r" ]]
then
    echo "Forced removal of ${D1}, ${D2}, ${D3}, *.exe, *.png, *~"
    rm -rf $D1 , $D2 , $D3, *.exe, *.png, *~
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
if [[ ( -d $D1 ) && ( ! ( -f "${D1}/Final_MetropolisMCMCMethod.dat" ) ) ]]
then
    rm -f ${D1}/*
    echo "Directory ${D1}/: Creating '${D1}/MetropolisMCMCMethod.dat'"
    for n_mcmc in 100 250 500 750 1000 1250 1500 2000 3000 5000 7500 10000 50000 75000 100000 500000 750000 1000000
    do
	echo -e "\t ./MetropolisMCMCMethod.exe ${n_mcmc} ${N_int} ${L} ${eps} | tail -n 1 >> ${D1}/MetropolisMCMCMethod.dat"
        ./MetropolisMCMCMethod.exe $n_mcmc $N_int $L $eps | tail -n 1 >> "${D1}/MetropolisMCMCMethod.dat"
    done
    echo "Directory ${D1}/: Creating '${D1}/Final_MetropolisMCMCMethod.dat'"
    cat "${D1}/MetropolisMCMCMethod.dat" | awk '{print $1, $4 }' > "${D1}/Final_MetropolisMCMCMethod.dat"
fi


if [ ! -d $D2 ]
then
    echo "Creating DIRECTORY '${D2}/'"
    mkdir $D2
fi

if [[ ( -d $D2 ) && ( ! ( -f "${D2}/Final_MetropolisMCMCMethod.dat" ) ) ]]
then
    rm -f ${D2}/*
    echo "Directory ${D2}/: Creating '${D2}/MetropolisMCMCMethod.dat'"
    for n_int in 10 25 50 75 100 250 500 750 1000 2500 5000 7500 10000
    do
	echo -e "\t ./MetropolisMCMCMethod.exe ${N_mcmc} ${n_int} ${L} ${eps} | tail -n 1 >> ${D2}/MetropolisMCMCMethod.dat"
        ./MetropolisMCMCMethod.exe $N_mcmc $n_int $L $eps | tail -n 1 >> "${D2}/MetropolisMCMCMethod.dat"
    done
    echo "Directory ${D2}/: Creating '${D2}/Final_MetropolisMCMCMethod.dat'"
    cat "${D2}/MetropolisMCMCMethod.dat" | awk '{print $2, $5}' > "${D2}/Final_MetropolisMCMCMethod.dat"
fi

if [ ! -d $D3 ]
then
    echo "Creating DIRECTORY '${D3}/'"
    mkdir $D3
fi

if [[ ( -d $D3 ) && ( ! ( -f "${D3}/Final_MetropolisMCMCMethod.dat" ) ) ]]
then
    rm -f ${D3}/*
    echo "Directory ${D3}/: Creating '${D3}/MetropolisMCMCMethod.dat'"
    for e in 0.01 0.05 0.1 0.15 0.20 0.25 0.30 0.35 0.40 0.45 0.50 0.55 0.60 0.65 0.70 0.75 0.80 0.85 0.90 0.95 1.0 1.05 1.10 1.15 1.20 1.25 1.30 1.35 1.40 1.45 1.50 1.55 1.60 1.65 1.70 1.75 1.80 1.85 1.90 1.95 2.00
    do
	echo -e "\t ./MetropolisMCMCMethod.exe 500000 10 ${L} ${e} | tail -n 1 >> ${D3}/MetropolisMCMCMethod.dat"
        ./MetropolisMCMCMethod.exe $N_mcmc 10 $L $e | tail -n 1 >> "${D3}/MetropolisMCMCMethod.dat"
    done
    echo "Directory ${D3}/: Creating '${D3}/Final_MetropolisMCMCMethod.dat'"
    cat "${D3}/MetropolisMCMCMethod.dat" | awk '{print $3, $4, $6}' > "${D3}/Final_MetropolisMCMCMethod.dat"
fi

gnuplot -e "dir1='${D1}'" -e "dir2='${D2}'" -e "dir3='${D3}'" -e "filename='Final_MetropolisMCMCMethod'" -e "N_mcmc2=${N_mcmc}" "GP_multiplot_MetropolisMCMCMethod.gp"

xdg-open "Graph_Final_MetropolisMCMCMethod4.png"
