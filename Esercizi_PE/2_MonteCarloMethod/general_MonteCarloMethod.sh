#!/bin/bash

gcc -Wall -Wshadow -Winline -o MonteCarloMethod.exe MonteCarloMethod_main.c -lm

D1="_N_mcs";
D2="_N_int";
D3="_L";

N_mcs=100000
N_int=5000
L=1.00
#IF the first COMMAND LINE ARGUMENT IS <r(eset)>, then remove the directorys
if [[ $1 = "r" ]]
then
    echo "Forced removal of ${D1}, ${D2}, ${D3}, *.exe, *.png, *~"
    rm -rf $D1 , $D2 , $D3, *.exe, *.png, , *~
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
if [[ ( -d $D1 ) && ( ! ( -f "${D1}/Final_MonteCarloMethod.dat" ) ) ]]
then
    rm -f ${D1}/*
    echo "Directory ${D1}/: Creating '${D1}/MonteCarloMethod.dat'"
    echo "# N_mcs - N_int - L - I - s_I" > "${D1}/MonteCarloMethod.dat"
    for n_mcs in 100 250 500 750 1000 1250 1500 2000 3000 5000 7500 10000 50000 100000
    do
	echo -e "\t ./MonteCarloMethod.exe ${n_mcs} ${N_int} ${L} >> ${D1}/MonteCarloMethod.dat"
        ./MonteCarloMethod.exe $n_mcs $N_int $L >> "${D1}/MonteCarloMethod.dat"
    done
    echo "Directory ${D1}/: Creating '${D1}/Final_MonteCarloMethod.dat'"
    cat "${D1}/MonteCarloMethod.dat" | awk '{if (NR >= 2) print $1, $4, $5 }' > "${D1}/Final_MonteCarloMethod.dat"
fi


if [ ! -d $D2 ]
then
    echo "Creating DIRECTORY '${D2}/'"
    mkdir $D2
fi

if [[ ( -d $D2 ) && ( ! ( -f "${D2}/Final_MonteCarloMethod.dat" ) ) ]]
then
    rm -f ${D2}/*
    echo "Directory ${D2}/: Creating '${D2}/MonteCarloMethod.dat'"
    echo "# N_mcs - N_int - L - I - s_I" > "${D2}/MonteCarloMethod.dat" 
    for n_int in 5 10 15 20 25 50 75 100 150 200 250 500 750 1000
    do
	echo -e "\t ./MonteCarloMethod.exe ${N_mcs} ${n_int} ${L} >> ${D2}/MonteCarloMethod.dat"
        ./MonteCarloMethod.exe $N_mcs $n_int $L >> "${D2}/MonteCarloMethod.dat"
    done
    echo "Directory ${D2}/: Creating '${D2}/Final_MonteCarloMethod.dat'"
    cat "${D2}/MonteCarloMethod.dat" | awk '{if (NR >= 2) print $2, $5 }' > "${D2}/Final_MonteCarloMethod.dat"
fi


if [ ! -d $D3 ]
then
    echo "Creating DIRECTORY '${D3}/'"
    mkdir $D3
fi

if [[ ( -d $D3 ) && ( ! ( -f "${D3}/Final_MonteCarloMethod.dat" ) ) ]]
then
    rm -f ${D3}/*
    echo "Directory ${D3}/: Creating '${D3}/MonteCarloMethod.dat'"
    echo "# N_mcs - N_int - L - I - s_I" > "${D3}/MonteCarloMethod.dat" 
    for l in 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 20 25 30 35 40 45 50 
    do
	echo -e "\t ./MonteCarloMethod.exe ${N_mcs} ${N_int} ${l} >> ${D3}/MonteCarloMethod.dat"
        ./MonteCarloMethod.exe $N_mcs $N_int $l >> "${D3}/MonteCarloMethod.dat"
    done
    echo "Directory ${D3}/: Creating '${D3}/Final_MonteCarloMethod.dat'"
    cat "${D3}/MonteCarloMethod.dat" | awk '{if (NR >= 2) print $3, $5 }' > "${D3}/Final_MonteCarloMethod.dat"
fi  

#t0="{/:Bold MonteCarlo Integral: Theta(1-x^2-y^2) in [-L,L]^2} (Fixed values: L=1.0, N_{MCS}=100k, N_{INT}=50k, N := N_{MCS}*N_{INT})"
#t1="E[I] in funzione di N_{MCS}, con L=1.0, N_{INT}=50k"
#t2="Errore su E[I] in funzione di N_{MCS}, con L=1.0, N_{INT}=50k"
#t3="Errore su E[I] in funzione di N_{INT}, con L=1.0, N_{MCS}=100k"
#t4="Errore su E[I] in funzione di L, con N_{MCS}=100k, N_{INT}=50k e N := N_{MCS}*N_{INT}"
#gnuplot -e "dir1='${D1}'" -e "dir2='${D2}'" -e "dir3='${D3}'" -e "filename='Final_MonteCarloMethod'" -e "title1='${t1}'" -e "title2='${t2}'" -e "title3='${t3}'" -e "title4='${t4}'" -e "N_mcs=${N_mcs}" -e "N_int=${N_int}" "GP_multiplot_MonteCarloMethod.gp"

gnuplot -e "dir1='${D1}'" -e "dir2='${D2}'" -e "dir3='${D3}'" -e "filename='Final_MonteCarloMethod'" -e "N_mcs=${N_mcs}" -e "N_int=${N_int}" "GP_multiplot_MonteCarloMethod.gp"

xdg-open "Graph_Final_MonteCarloMethod4.png"
