# Il programma grafica in un riuadro i file di dati..
# Necessita dei parametri:
# - «dir», ossia il percorso del file da graficare relativo alla cartella dove è presente "general_integratore.c"
# - «filename», ovvero il nome del file di tipo DAT da graficare senza l'estenzione ".dat"
# - «col1» e «col2», il numero delle colonne da graficare
# - «title_sK» la stringa che corrisponderà al titolo del file di output

###################
# I Grafico: I, N #
###################
reset
set term qt
set term pngcairo size 1024,768
set output "Graph_".filename."1.png"

set ylabel "I"
set xlabel "N"
set xtics 20000
plot dir1."/".filename.".dat" u 2 : 3 w l lw 2.5 lc rgb "#4682B4" title "data I(N)", \
     pi lt 3 dt 2 lw 1.5 lc rgb "#000000" title "y = {/Symbol p}"

#####################
# II Grafico: I, L #
#####################
reset
set term qt
set term pngcairo size 1024,768
set output "Graph_".filename."2.png"

set ylabel "{/Symbol s}_I"
set xlabel "L^{4}/N"
plot dir2."/".filename.".dat" u ($1*$1*$1*$1/$2) : (sqrt(abs($3*$3-pi*pi))/sqrt(10000-1)) w l lw 1.5 lc rgb "#4682B4" title "error I(L)"

unset multiplot 

