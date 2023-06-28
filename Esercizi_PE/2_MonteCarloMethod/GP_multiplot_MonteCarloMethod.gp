# Il programma grafica in un riuadro i file di dati..
# Necessita dei parametri:
# - «dir», ossia il percorso del file da graficare relativo alla cartella dove è presente "general_integratore.c"
# - «filename», ovvero il nome del file di tipo DAT da graficare senza l'estenzione ".dat"
# - «col1» e «col2», il numero delle colonne da graficare
# - «title_sK» la stringa che corrisponderà al titolo del file di output

#######################
# I Grafico: I, N_MCS #
#######################
reset
set term qt
set term pngcairo size 1024,768
set output 'Graph_'.filename.'1.png'

set ylabel "{E[I]}"
set xlabel "N_{MCS}"
set logscale x 10
#set title "{/:Bold ".title1."}"
plot dir1."/".filename.".dat" u 1 : 2 w l lw 2.5 lc rgb "#4682B4" title "data I(N_{MCS})", \
     pi lt 3 dt 2 lw 1.5 lc rgb "#000000" title "y = {/Symbol p}"

##########################
# II Grafico: S_I, N_MCS #
##########################
reset
set term qt
set term pngcairo size 1024,768
set output 'Graph_'.filename.'2.png'

set ylabel "{/Symbol s}_{E[I]}"
set xlabel "N_{MCS}"
set logscale x 10
#set title "{/:Bold ".title2."}"
plot dir1."/".filename.".dat" u 1 : 3 w l lw 2.5 lc rgb "#4682B4" title "error I(N_{MCS})"

unset logscale x
###########################
# III Grafico: S_I, N_INT #
###########################
reset
set term qt
set term pngcairo size 1024,768
set output 'Graph_'.filename.'3.png'

set ylabel "{/Symbol s}_{E[I]}"
set xlabel "N_{INT}"
set xtics 200
#set title "{/:Bold ".title3."}"
plot dir2."/".filename.".dat" u 1 : 2 w l lw 2.5 lc rgb "#4682B4" title "error I(N_{INT})", \
     1/sqrt(N_mcs*x-1) lt 3 dt 2 lw 1.5 lc rgb "#000000" title "y=1/sqrt(N_{MCS}*N_{INT}-1)"
 
######################
# IV Grafico: S_I, L #
######################
reset
set term qt
set term pngcairo size 1024,768
set output 'Graph_'.filename.'4.png'

set ylabel "{/Symbol s}_{E[I]}"
set xlabel "L"
set xtics 5
#set title "{/:Bold ".title4."}"
plot dir3."/".filename.".dat" u 1 : 2 w l lw 2.5 lc rgb "#4682B4" title "error I(L)", \
     sqrt(4*pi*x*x-pi*pi)/sqrt(N_int*N_mcs-1) lt 3 dt 2 lw 1.5 lc rgb "#000000" title "y=sqrt(4{/Symbol p}L^2-{/Symbol p}^2)/sqrt(N-1)"


