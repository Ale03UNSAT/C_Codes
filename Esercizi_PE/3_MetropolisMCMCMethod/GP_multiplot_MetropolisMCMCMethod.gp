reset
output_filelog = sprintf ( "%s/Log_%s.log", dir2, filename)
set fit logfile output_filelog errorvariables
a=0.001
f(x) = a/sqrt(x-1)
fit f(x) dir2."/".filename.".dat" using 1:2 via a
fit_label = sprintf ("y=%f/sqrt(N-1)", a)

command_echo = sprintf ("echo %f \t %f >> %s", a, a_err, output_filelog)
system (command_echo)
unset fit

########################
# I Grafico: I, N_MCMC #
########################
reset
set term qt
set term pngcairo size 1024,768
set output 'Graph_'.filename.'1.png'

set ylabel "{E[I]}"
set xlabel "N_{MCMC}"
set logscale x 10
plot dir1."/".filename.".dat" u 1 : 2 w l lw 1.5 lc rgb "#4682B4" title "data I(N_{MCMC})", \
     pi lt 3 dt 2 lw 1 lc rgb "#000000" title "y = {/Symbol p}"

##########################
# II Grafico: S_I, N_INT #
####à#####################
reset
set term qt
set term pngcairo size 1024,768
set output 'Graph_'.filename.'2.png'

set ylabel "{/Symbol s}_{E[I]}"
set xlabel "N_{INT}"
set logscale x 10
plot dir2."/".filename.".dat" u 1 : 2 w l lw 1.5 lc rgb "#4682B4" title "error I(N_{INT})", \
     f(x) lt 3 dt 2 lw 1 lc rgb "#000000" title fit_label

unset logscale

#######################
# III Grafico: S_I, L #
#######################
reset
set term qt
set term pngcairo size 1024,768
set output 'Graph_'.filename.'3.png'

set ylabel "P_{acc}"
set xlabel "{/Symbol e}"
set yrange [ 0.0:1.0 ]
set xtics 0.2
plot dir3."/".filename.".dat" u 1 : 3 w l lw 2 lc rgb "#4682B4" title "P_{ACC}({/Symbol e})", \
     0.5 lt 3 dt 2 lw 0.5 lc rgb "#000000"

#######################
# III Grafico: S_I, L #
#######################
reset
set term qt
set term pngcairo size 1024,768
set output 'Graph_'.filename.'4.png'

set arrow 1 from 1.8,0.000010 rto 0,1.00 nohead lw 0.5 dt 2 lc rgb "#000000"
set ylabel "abs({/Symbol p} - I)"
set xlabel "{/Symbol e}"
unset yrange
set logscale y
set xtics 0.2
set key left bottom
plot dir3."/".filename.".dat" u 1 : (abs(pi-$2)) w l lw 2 lc rgb "#4682B4" title "|{/Symbol p}-I({/Symbol e})|", \
     dir3."/".filename.".dat" u 1 : 3 w l lw 0.5 lc rgb "#000000" title "P_{ACC}({/Symbol e})", \
     0.5 dt 3 lw 2.5 lc rgb "#000000", \
     1/0 lw 0.5 dt 2 lc rgb "#000000" title "{/Symbol e}=1.8"

unset multiplot 

