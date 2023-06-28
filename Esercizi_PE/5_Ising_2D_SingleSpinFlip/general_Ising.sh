#!/bin/bash

#Partenza con spin 1
gcc -Wall -Wshadow -Winline -D __Int__ -D __PositiveStart__  -o Ising_Pos.exe Ising_main.c -lm
#Partenza con spin -1
gcc -Wall -Wshadow -Winline -D __Int__ -D __NegativeStart__ -o Ising_Neg.exe Ising_main.c -lm
#Partenza con spin casuali ( <m>=0 )
gcc -Wall -Wshadow -Winline -D __Int__ -D __RandomStart__   -o Ising_RandomStart.exe Ising_main.c -lm

#Studio transiente modalità tre storie
gcc -Wall -Wshadow -Winline -D __TransientStudy__                 -o Ising_T1.exe Ising_main.c -lm
gcc -Wall -Wshadow -Winline -D __TransientStudy__ -D __Print_em__ -o Ising_T2.exe Ising_main.c -lm 

gcc -Wall -Wshadow -Winline -D __Binder__ -o Ising_Binder.exe Ising_main.c -lm 
#gcc -Wall -Wshadow -Winline -D __Graphs17__ -D __TransientPhaseAndTimeCorrelation__ -o Ising_TC.exe Ising_main.c -lm 

#Se si vuoile essere sadici con il caso a temperature reali
#gcc -Wall -Wshadow -Winline -D __k_b__ -o KB_Ising.exe Ising_main.c -lm

D1="_Int"
D2="_TransientPhase"
D3="_Time_Block-Mean"
D4="_Binder"

filling_temperature_Ising ()
{
    #Creazione dell'array
    local -n array=$1
    i=$2
    #L'indice dei naturali va da $2 a $3
    while [[ $i -le $3 ]]
    do
	for j in 0 5
	do
	    array+=( "${i}.${j}" )
	    #Se i è uguale a 4 e j=0
	    if [[ ( $i -eq $4) && ( $j -eq 0 ) ]]
	    then
		for j in 1 2 3
		do
		    for k in 0 2 4 6 8
		    do
			array+=("${i}.${j}${k}")
		    done
		done 
	    fi
	done
	((i++))
    done
}


#Se l'argomento da linea di terminale è 'r' (reset) allora cancella tutte le cartelle 
if [[ $1 = "r" ]]
then
    #Se si esegue il reset, le cartelle D2 e D4 non saranno cancellate
    echo "Forced removal of ${D1}, ${D3}, *.exe, *.png, *~"
    rm -rf $D1, $D3, *.exe, *.png, *~
else
    echo "You can use <r> COMMAND LINE ARG to reset the DIRECTORYS";
    rm -rf *png, *~
fi


if [[ ! -d $D1 ]]
then
    echo "Creating DIRECTORY '${D1}/'"
    mkdir $D1
fi
 
#Se la sotto-cartella '_X/' esiste ma non c'è il file 'Filename.dat', allora rimuovi qualcosa nella directory e ricrealo   
if [[ ( -d $D1 ) && ( ! ( -f "${D1}/Final_Ising${D1}.dat" ) ) ]]
then
    if [[ ! ( -f "${D1}/Final_Ising${D1}.dat" ) ]]
    then
	rm -rf ${D1}/*
	T_MAX=1000000000
	N_REPS=10
	LEN=500
	T=2.0
	    
	declare -a paste_filelist1=()
	
	#(1)-(2).First Approach to the Model: Equilibrium position for E, M. Two fixed-length lattices and T=2.0<T_{C}
	declare -a len=( "4" "15" )
	for i in ${!len[@]}
	do
	    echo "Directory ${D1}/: Creating '${D1}/Ising${D1}_L$i.dat'"
	    echo -e "\t ./Ising_Pos.exe $T_MAX $N_REPS ${len[$i]} $T | grep '#N#' | sed 's/#N#//g' > ${D1}/Ising${D1}_L$i.dat"
	    ./Ising_Pos.exe $T_MAX $N_REPS ${len[$i]} $T | grep '#N#' | sed 's/#N#//g' > "${D1}/Ising${D1}_L$i.dat"
	    paste_filelist1+=("${D1}/Ising${D1}_L$i.dat")
	done
	
	#(3)-(4).Studying Length dependences of the Extensive Observables at T>=T_{C}, with fixed length L=500
	declare -a temperatures=( "2.268" "2.5" "5.0" "10.0" "20.0" )
	for i in ${!temperatures[@]} 
	do	
	    echo "Directory ${D1}/: Creating '${D1}/Ising${D1}_G$i.dat'"
	    echo -e "\t ./Ising_Pos.exe $T_MAX $N_REPS $LEN ${temperatures[$i]}  | grep '#N#' | sed 's/#N#//g' > ${D1}/Ising${D1}_G$i.dat"
	    ./Ising_Pos.exe $T_MAX $N_REPS $LEN ${temperatures[$i]} | grep '#N#' | sed 's/#N#//g' > "${D1}/Ising${D1}_G$i.dat"
	    paste_filelist1+=("${D1}/Ising${D1}_G$i.dat")
	done
	
	#pasting the files (1)--(4) files into the _Ext/Final_Ising_Ext.dat
	echo "paste ${paste_filelist1[@]} | awk '{print \$1, \$2, \$3, \$5, \$6, \$8, \$9, \$11, \$12, \$14, \$15, \$17, \$18, \$20, \$21}' > ${D1}/Final_Ising${D1}.dat"
	paste ${paste_filelist1[@]} | awk '{print $1, $2, $3, $5, $6, $8, $9, $11, $12, $14, $15, $17, $18, $20, $21}' > "${D1}/Final_Ising${D1}.dat"
    fi
fi

if [[ ! -d $D2 ]]
then
    echo "Creating DIRECTORY '${D2}/'"
    mkdir $D2
fi

if [[ ( -d $D2 ) && ( ( ! ( -f "${D2}/Final_Ising${D2}_1.dat" ) ) || ( ! ( -f "${D2}/Final_Ising${D2}_2.dat" ) ) ) ]]
then
    T_MAX=1000000000
    N_REPS=10

    #(6)-(5).Studying Transient Phases for different Lengths, varying Temperatures 
    if [[ ! ( -f "${D2}/Final_Ising${D2}_1.dat" ) ]]
    then	
	declare -a paste_filelist2=()
	declare -a temperatures=() 
	filling_temperature_Ising temperatures 1 6 2 0 6 #da 1.0 a 6.5 ogni 0.5 con passi da 0.2 da 2.10 a 2.38
	declare -a lengthsE=( 10 50 100 250 500 1000 )
	declare -a lengthsM=( 5 10 20 40 80 160 )
	
	for len in 0 1 2 3 4 5
	do
	    #SE non esistono i file con i transienti dell'energia intensiva
	    if [[ ! ( -f "${D2}/Ising${D2}_TPE$len.dat" ) ]]
	    then
		echo "Directory ${D2}/: Creating '${D2}/Ising${D2}_TPE$len.dat'"
		for temp in ${!temperatures[@]}
		do
		    echo -e "\t ./Ising_T1.exe $T_MAX $N_REPS ${lengthsE[$len]} ${temperatures[$temp]} | grep '#TPE#' | sed 's/#TPE#//g' >> ${D2}/Ising${D2}_TPE$len.dat"
		    ./Ising_T1.exe $T_MAX $N_REPS ${lengthsE[$len]} ${temperatures[$temp]} | grep '#TPE#' | sed 's/#TPE#//g'>> "${D2}/Ising${D2}_TPE$len.dat"
		done
	    fi
	    paste_filelist2+=( "${D2}/Ising${D2}_TPE$len.dat" )

	    #Se non esistono i file con i transienti della magnetizzazione intensiva
	    if [[ ! ( -f "${D2}/Ising${D2}_TPM$len.dat" ) ]]
	    then
		echo "Directory ${D2}/: Creating '${D2}/Ising${D2}_TPM$len.dat'"
		for temp in ${!temperatures[@]}
		do
       		    echo -e "\t ./Ising_T1.exe $T_MAX $N_REPS ${lengthsM[$len]} ${temperatures[$temp]} | grep '#TPM#' | sed 's/#TPM#//g' >> ${D2}/Ising${D2}_TPM$len.dat"
		    ./Ising_T1.exe $T_MAX $N_REPS ${lengthsM[$len]} ${temperatures[$temp]} | grep '#TPM#' | sed 's/#TPM#//g' >> "${D2}/Ising${D2}_TPM$len.dat"
		done
	    fi
	    paste_filelist2+=( "${D2}/Ising${D2}_TPM$len.dat" )
	done
	echo "paste ${paste_filelist2[@]} | awk '{print \$1, \$2, \$4, \$6, \$8, \$10, \$12, \$14, \$16, \$18, \$20, \$22, \$24}' > ${D2}/Final_Ising${D2}_1.dat"
	paste ${paste_filelist2[@]} | awk '{print $1, $2, $4, $6, $8, $10, $12, $14, $16, $18, $20, $22, $24}' > "${D2}/Final_Ising${D2}_1.dat"
    fi

    #(7).Studying the Transient Phases by searchig when three systems ( m(0)=1, m(0)=0 and m(0)=-1) reach the equilibrium state
    #T_MAX=10000000000
    LEN=500
    T=2.5
    N_REPS=10
    if [[ ! ( -f "${D2}/Final_Ising{D2}_2.dat" ) ]]
    then
        echo "Directory ${D2}/: Creating '${D2}/Final_Ising${D2}_2.dat'"
        echo -e "\t ./Ising_T2.exe $T_MAX $N_REPS $LEN $T  | grep '#N#' | sed 's/#N#//g' > ${D2}/Final_Ising${D2}_2.dat"
        ./Ising_T2.exe $T_MAX $N_REPS $LEN $T | grep '#N#' | sed 's/#N#//g' > "${D2}/Final_Ising${D2}_2.dat"
    fi
fi

if [[ ! -d $D3 ]]
then
    echo "Creating DIRECTORY '${D3}/'"
    mkdir $D3
fi
 
if [[ ( -d $D3 ) && ( ! ( -f "${D3}/Final_Ising${D3}_RAW22.dat" ) ) ]]
then
    rm -rf ${D3}/*
    T_MAX=100000000
    N_REPS=10
    LEN=500

    declare -a temperatures=()
    filling_temperature_Ising temperatures 1 4 2 0 6 #da 1.0 a 6.5 ogni 0.5 con passi da 0.2 da 2.10 a 2.38
   
    echo "Directory ${D3}/: Creating '${D3}/Final_Ising${D3}.dat'"
    for temp in ${!temperatures[@]}
    do
	echo -e "\t ./Ising_Pos.exe ${T_MAX} ${N_REPS} ${LEN} ${temperatures[$temp]} | grep '#E-M#' | sed 's/#E-M#//g' > ${D3}/Final_Ising${D3}_RAW${temp}.dat"
	./Ising_Pos.exe $T_MAX $N_REPS $LEN ${temperatures[$temp]} | grep '#E-M#' | sed 's/#E-M#//g' > "${D3}/Final_Ising${D3}_RAW${temp}.dat"
	#Si può attivare un fit
	#gnuplot -e "dir='${D3}'" -e "filename='Final_Ising${D3}_RAW${temp}'" -e "output_file='Final_Ising${D3}'" -e "Temp=${temperatures[$temp]}" 'GP_fit_Ising.gp' 
    done
fi

# IF not exist the '_X/' directory, then make it
if [[ ! -d $D4 ]]
then
    echo "Creating DIRECTORY '${D4}/'"
    mkdir $D4
fi
 
#if the '_X/' directory exist but there's no FileName.dat, then remove all files in the directory
if [[ ( -d $D4 ) && ( ! ( -f "${D4}/Final_Ising${D4}.dat" ) ) ]]
then
    rm -rf ${D4}/*
    T_MAX=1000000000
    N_REPS=10

    declare -a paste_filelist3=()
    declare -a lengths=( 5 10 50 100 500 )
    declare -a temperatures=()
    filling_temperature_Ising temperatures 1 4 2 0 6 #da 1.0 a 6.5 ogni 0.5 con passi da 0.2 da 2.10 a 2.38

    echo "Directory ${D4}/: Creating '${D4}/Final_Ising${D4}_${len}.dat'" 
    for len in ${!lengths[@]}
    do
	for temp in ${!temperatures[@]}
	do
	    echo -e "\t ./Ising_Binder.exe ${T_MAX} ${N_REPS} ${lengths[$len]} ${temperatures[$temp]} | grep '#B#' | sed 's/#B#//g' | awk '{ print \$1, \$2 }' >> ${D4}/Final_Ising${D4}_B${len}.dat"
	    ./Ising_Binder.exe $T_MAX $N_REPS ${lengths[$len]} ${temperatures[$temp]} | grep '#B#' | sed 's/#B#//g' | awk '{ print $1, $2}' >> "${D4}/Final_Ising${D4}_B${len}.dat" 
	done
	paste_filelist3+=("${D4}/Final_Ising${D4}_B${len}.dat")
    done
    echo "paste ${paste_filelist3[@]} | awk '{print \$1, \$2, \$4, \$6, \$8, \$10, \$12}' > ${D4}/Final_Ising${D4}.dat"
    paste ${paste_filelist3[@]} | awk '{print $1, $2, $4, $6, $8, $10, $12}' > "${D4}/Final_Ising${D4}.dat"
fi

gnuplot -e "dir1='${D1}'" -e "dir2='${D2}'" -e "dir3='${D3}'" -e "dir4='${D4}'" -e "filename='Final_Ising'" 'GP_multiplot_Ising.gp'

xdg-open "Graph_Final_Ising1.png"
