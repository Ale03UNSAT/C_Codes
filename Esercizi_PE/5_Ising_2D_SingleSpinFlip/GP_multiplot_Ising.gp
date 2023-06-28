# Il programma grafica in un riuadro i file di dati..
# Necessita dei parametri:
# - «dir», ossia il percorso del file da graficare relativo alla cartella dove è presente "general_integratore.c"
# - «filename», ovvero il nome del file di tipo DAT da graficare senza l'estenzione ".dat"
# - «col1» e «col2», il numero delle colonne da graficare
# - «title_sK» la stringa che corrisponderà al titolo del file di output


###################
# I Grafico: A, t #
###################
reset
set term qt
set term pngcairo size 1024,768
set output 'Graph_'.filename.'1.png'

set ylabel "a"
set xlabel "t"
set logscale x
#set title "{/:Bold Evoluzione temporale delle osservabili intensive E e M (con  L=4, T=2.0<T_{C})}"
set key left
plot dir1."/".filename."".dir1.".dat" u 1 : 2 w l lw 2.5 lc rgb "#FF7514" title "e(t)", \
     dir1."/".filename."".dir1.".dat" u 1 : 3 w l lw 2.5 lc rgb "#0047AB" title "m(t)"

####################
# II Grafico: A, t #
####################
reset
set term qt
set term pngcairo size 1024,768
set output 'Graph_'.filename.'2.png'

set ylabel "a"
set xlabel "t"
set logscale x
#set title "{/:Bold Evoluzione temporale delle osservabili intensive E e M (con  L=15, T=2.0<T_{C})}"
set key left
plot dir1."/".filename."".dir1.".dat" u 1 : 4 w l lw 2.5 lc rgb "#FF7514" title "e(t)", \
     dir1."/".filename."".dir1.".dat" u 1 : 5 w l lw 2.5 lc rgb "#0047AB" title "m(t)"

#####################
# III Grafico: A, t #
#####################
reset
set term qt
set term pngcairo size 1024,768
set output 'Graph_'.filename.'3.png'

set ylabel "e"
set xlabel "t"
set logscale x
#set title "{/:Bold Evoluzione temporale dell'energia intensiva (con L=500, T>T_{C})"
set key top left
plot dir1."/".filename."".dir1.".dat" u 1 : 6  w l lw 2.5 lc rgb "#FFD700" title "T=T_{C}=2.268", \
     dir1."/".filename."".dir1.".dat" u 1 : 8  w l lw 2.5 lc rgb "#FFA500" title "T=2.5", \
     dir1."/".filename."".dir1.".dat" u 1 : 10 w l lw 2.5 lc rgb "#FF7154" title "T=5.0", \
     dir1."/".filename."".dir1.".dat" u 1 : 12 w l lw 2.5 lc rgb "#FF0000" title "T=10.0", \
     dir1."/".filename."".dir1.".dat" u 1 : 14 w l lw 2.5 lc rgb "#8B0000" title "T=20.0"	

####################
# IV Grafico: A, t #
####################
reset
set term qt
set term pngcairo size 1024,768
set output 'Graph_'.filename.'4.png'

set ylabel "m"
set xlabel "t"
set logscale x
#set title "{/:Bold Evoluzione temporale della magnetizazione intensiva (con L=500, T>T_{C})}"
set key bottom left
plot dir1."/".filename."".dir1.".dat" u 1 : 7  w l lw 2.5 lc rgb "#FFD700" title "T=T_{C}=2.268", \
     dir1."/".filename."".dir1.".dat" u 1 : 9  w l lw 2.5 lc rgb "#FFA500" title "T=2.5", \
     dir1."/".filename."".dir1.".dat" u 1 : 11 w l lw 2.5 lc rgb "#FF7514" title "T=5.0", \
     dir1."/".filename."".dir1.".dat" u 1 : 13 w l lw 2.5 lc rgb "#FF0000" title "T=10.0", \
     dir1."/".filename."".dir1.".dat" u 1 : 15 w l lw 2.5 lc rgb "#8B0000" title "T=20.0"	

unset logscale x
###################
# V Grafico: A, t #
###################
reset
set term qt
set term pngcairo size 1024,768
set output 'Graph_'.filename.'5.png'

set ylabel "{/Symbol t}_{TERM; e}"
set xlabel "T"
set logscale y
#set title "{/:Bold Tempo di termalizzazione dell'energia intensiva (E) in funzione della temperatura (L fissato)}"
set arrow 1 from 2./log(1+sqrt(2)),100 rto 0,1000000000 nohead dt 2 lw 1.5
show arrow 1
plot  dir2."/".filename."".dir2."_1.dat" u 1 : 2  w l lw 2.5 lc rgb "#FFD700" title "L=10", \
      dir2."/".filename."".dir2."_1.dat" u 1 : 4  w l lw 2.5 lc rgb "#FFA500" title "L=50", \
      dir2."/".filename."".dir2."_1.dat" u 1 : 6  w l lw 2.5 lc rgb "#FF7514" title "L=100", \
      dir2."/".filename."".dir2."_1.dat" u 1 : 8  w l lw 2.5 lc rgb "#FF0000" title "L=250", \
      dir2."/".filename."".dir2."_1.dat" u 1 : 10 w l lw 2.5 lc rgb "#8B0000" title "L=500", \
      dir2."/".filename."".dir2."_1.dat" u 1 : 12 w l lw 2.5 lc rgb "#000000" title "L=1000", \
      1/0 w l dt 2 lw 1.5 lc rgb "#000000" title "T=T_{C}"
unset arrow 1
####################
# VI Grafico: A, t #
####################

reset
set term qt
set term pngcairo size 1024,768
set output 'Graph_'.filename.'6.png'


set arrow 2 from 2./log(1+sqrt(2)),100 rto 0,10000000000 nohead dt 2 lw 1.5
show arrow 2
set ylabel "{/Symbol t}_{TERM; m}"
set xlabel "T"
set logscale y
#set title "{/:Bold Tempo di termalizzazione della magnetizzazione estensiva (M) in funzione della temperatura (L fissato)}"
plot  dir2."/".filename."".dir2."_1.dat" u 1 : 3  w l lw 2.5 lc rgb "#FFD700" title "L=5",  \
      dir2."/".filename."".dir2."_1.dat" u 1 : 5  w l lw 2.5 lc rgb "#FFA500" title "L=10", \
      dir2."/".filename."".dir2."_1.dat" u 1 : 7  w l lw 2.5 lc rgb "#FF7514" title "L=20", \
      dir2."/".filename."".dir2."_1.dat" u 1 : 9  w l lw 2.5 lc rgb "#FF0000" title "L=40", \
      dir2."/".filename."".dir2."_1.dat" u 1 : 11 w l lw 2.5 lc rgb "#8B0000"  title "L=60", \
      dir2."/".filename."".dir2."_1.dat" u 1 : 13 w l lw 2.5 lc rgb "#000000" title "L=80", \
      1/0 w l dt 2 lw 1.5 lc rgb "#000000" title "T=T_{C}"
unset logscale y
unset arrow 2
#####################
# VII Grafico: A, t #
#####################
reset
set term qt
set term pngcairo size 1024,768
set output 'Graph_'.filename.'7.png'

set ylabel "a"
set xlabel "t"
set logscale x
#set title "{/:Bold Evoluzione temporale dell'energia e della magetizzazione intensive da configurazioni iniziali diverse (con L=500, T=2.5>T_{C})"
set key top left
plot dir2."/".filename."".dir2."_2.dat" u 1 : 2 w l dt 2 lw 2.5 lc rgb "#FF8000" title "e^{(+)}", \
     dir2."/".filename."".dir2."_2.dat" u 1 : 4 w l dt 3 lw 2.5 lc rgb "#FF8000" title "e^{(-)}", \
     dir2."/".filename."".dir2."_2.dat" u 1 : 6 w l      lw 2.5 lc rgb "#FF8000" title "e^{(0)}", \
     dir2."/".filename."".dir2."_2.dat" u 1 : 3 w l dt 2 lw 2.5 lc rgb "#4682B4" title "m^{(+)}", \
     dir2."/".filename."".dir2."_2.dat" u 1 : 5 w l dt 3 lw 2.5 lc rgb "#4682B4" title "m^{(-)}", \
     dir2."/".filename."".dir2."_2.dat" u 1 : 7 w l      lw 2.5 lc rgb "#4682B4" title "m^{(0)}"

######################
# VIII Grafico: A, t #
######################
reset
set term qt
set term pngcairo size 1024,768
set output 'Graph_'.filename.'8.png'
set multiplot
#set title "{/:Bold Evoluzione temporale delle osservabili intensive mediate <m>_{t_{N-1};t_{N}} ed <e>_{t_{N-1};t_{N}} (con  L=50, T_{C}=2.269)}"

set logscale x
set xrange [1000:]

# Grafico:a <A>, T #
####################
set size 0.5, 0.5
set origin 0.0, 0.5
set ylabel "<m>"
set xlabel "t"
#set title "{/:Bold <m>_{t_{N-1};t_{N}}, T<T_{C} }"
plot dir3."/".filename."".dir3."_RAW0.dat"  u 1 : 3 : 5 w errorbars pt 7 lw 0.5 lc rgb "#00080" title "T=1.0", \
     dir3."/".filename."".dir3."_RAW2.dat" u 1 : 3 : 5 w errorbars pt 3 lw 0.5 lc rgb "#20B2AA" title "T=2.0"

# Grafico:b <A>, T #
####################
set size 0.5, 0.5
set origin 0.5, 0.5
set ylabel "<e>"
set xlabel "t"
plot dir3."/".filename."".dir3."_RAW0.dat"  u 1 : 2 : 4 w errorbars pt 7 lw 0.5 lc rgb "#000080" title "T=1.0", \
     dir3."/".filename."".dir3."_RAW2.dat" u 1 : 2 : 4 w errorbars pt 3 lw 0.5 lc rgb "#20B2AA" title "T=2.0"
     
# Grafico:c <A>, T #
####################
set size 0.5, 0.5
set origin 0.0, 0.0
set ylabel "<m>"
set xlabel "t"
plot dir3."/".filename."".dir3."_RAW19.dat" u 1 : 3 : 5 w errorbars pt 7 lw 0.5 lc rgb "#FF0000" title "T=3.0", \
     dir3."/".filename."".dir3."_RAW21.dat" u 1 : 3 : 5 w errorbars pt 3 lw 0.5 lc rgb "#8B0000" title "T=4.0"

# Grafico:d <A>, T #
####################
set size 0.5, 0.5
set origin 0.5, 0.0
set ylabel "<e>"
set xlabel "t"
#set title "{/:Bold <e>_{t-{N-1};t_{N}}, T>T_{C} }"
plot dir3."/".filename."".dir3."_RAW19.dat" u 1 : 2 : 4 w errorbars pt 7 lw 0.5 lc rgb "#FF0000" title "T=3.0", \
     dir3."/".filename."".dir3."_RAW21.dat" u 1 : 2 : 4 w errorbars pt 3 lw 0.5 lc rgb "#8B0000" title "T=4.0"

unset multiplot

######################
# IX Grafico: tau, T #
######################
reset
set term qt
set term pngcairo size 1024,768
set output 'Graph_'.filename.'9.png'

set arrow 3 from 2./log(1+sqrt(2)),0 rto 0.0,1.0 nohead dt 2 lw 1.5
show arrow 3
set ylabel "B( T | L)"
set xlabel "T"
set yrange [0:1]
#set title "{/:Bold Parametro di Binder in funzione della temperatura per L fissati}" 
plot dir4."/".filename."".dir4.".dat" u 1 : 2 w l lw 2.5 lc rgb "#FFD700" title "L = 10", \
     dir4."/".filename."".dir4.".dat" u 1 : 3 w l lw 2.5 lc rgb "#FFA500" title "L = 50", \
     dir4."/".filename."".dir4.".dat" u 1 : 4 w l lw 2.5 lc rgb "#FF7514" title "L = 100", \
     dir4."/".filename."".dir4.".dat" u 1 : 5 w l lw 2.5 lc rgb "#FF0000" title "L = 250", \
     dir4."/".filename."".dir4.".dat" u 1 : 6 w l lw 2.5 lc rgb "#8B0000" title "L = 500", \
     dir4."/".filename."".dir4.".dat" u 1 : 7 w l lw 2.5 lc rgb "#000000" title "L = 1000", \
     1/0 w l dt 2 lw 1.5 lc rgb "#000000" title "T=T_{C}"
unset arrow 3