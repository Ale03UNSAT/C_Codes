reset
getValue(row,col,filename) = system('awk ''{if (NR == '.row.') print $'.col.'}'' '.filename.'')
real_b=-abs(log(sqrt(1-W)))
real_c=getValue(3,4,dir1."/".filename1.".dat")

set fit logfile dir1."/Log_FiniteMarkovChain.log" errorvariables
a=-0.02
b=real_b
c=0.2
P(x) = a * exp( b * x ) + c
fit [0.0:][0.0:1] P(x) dir1.'/'.filename1.'.dat' using 1:4 via a,b,c
title_fit = sprintf ("a=%.2f;b=%.2e;c=%.2f;", a, b, c)
unset fit

#####################
# I Grafico: P vs N # 
#####################
set term pngcairo size 1024,768
set output 'Graph_FiniteMarkovChain1.png'

set ylabel "P_{K}(t)"
set xlabel "t"
set xrange [0:10000]
set xtics 2000
#set key at 8000,0.23
set key bottom
plot dir1."/".filename1.".dat" u 1 : 2 w l lw 1.5 lc rgb "#aea04b" title "P_{1}", \
     dir1."/".filename1.".dat" u 1 : 3 w l lw 1.5 lc rgb "#ff8000" title "P_{2}", \
     dir1."/".filename1.".dat" u 1 : 4 w l lw 1.5 lc rgb "#ff0000" title "P_{3}", \
     dir1."/".filename1.".dat" u 1 : 5 w l lw 1.5 lc rgb "#5e2020" title "P_{4}", \
     dir1."/".filename2.".dat" u 1 : 2 w l lt 3 dt 2 lw 2.5 lc rgb "#aea04b" title "{/Symbol p}_{1}", \
     dir1."/".filename2.".dat" u 1 : 3 w l lt 3 dt 2 lw 2.5 lc rgb "#ff8000" title "{/Symbol p}_{2}", \
     dir1."/".filename2.".dat" u 1 : 4 w l lt 3 dt 2 lw 2.5 lc rgb "#ff0000" title "{/Symbol p}_{3}", \
     dir1."/".filename2.".dat" u 1 : 5 w l lt 3 dt 2 lw 2.5 lc rgb "#5e2020" title "{/Symbol p}_{4}"


######################
# II Grafico: P vs t # 
######################s
set term pngcairo size 1024,768
set output 'Graph_FiniteMarkovChain2.png'

set ylabel "P_{K}(t)"
set xrange [0:10000]
set xlabel "t"
plot dir1."/".filename1.".dat" u 1 : 6 w l lw 1.5 lc rgb "#30d5c8" title "P_{5}", \
     dir1."/".filename1.".dat" u 1 : 7 w l lw 1.5 lc rgb "#066eff" title  "P_{6}", \
     dir1."/".filename2.".dat" u 1 : 6 w l lt 3 dt 2 lw 2.5 lc rgb "#30d5c8" title "{/Symbol p}_{5}", \
     dir1."/".filename2.".dat" u 1 : 7 w l lt 3 dt 2 lw 2.5 lc rgb "#066eff" title "{/Symbol p}_{6}"

########################
# III Grafico: P3 vs t # 
########################
set term pngcairo size 1024,768
set output 'Graph_FiniteMarkovChain3.png'

set ylabel "P_{5}"
set xlabel "t"
#set title "y_{FIT} = ae^{-bx}+c, ".title_fit     
plot dir1."/".filename1.".dat" u 1 : 4 w l lw 1.5 lc rgb "#4682B4" title "DATA", \
     P(x) w l lt 3 dt 2 lw 1.5 lc rgb "#000000" title "y_{FIT} = ae^{-bx}+c, ".title_fit

set key default
########################
# IV Grafico: N vs w63 # 
########################
set term pngcairo size 1024,768
set output 'Graph_FiniteMarkovChain4.png'

set ylabel "{/Symbol t}"
set xlabel "W_{63}"
set logscale y
set xrange [0.:1.]
set xtics 0,0.1
#set title "{/Symbol t} = -1/b = 1/ln(sqrt(1-x))"
plot dir2."/".filename3.".dat" u 1 : (-1.0/$3) w l lw 1.5 lc rgb "#4682B4" title "{/Symbol t}", \
     1.0/abs(log(sqrt(1.0-x))) w l lt 3 dt 3 lw 1.5 lc rgb "#000000" title "f(x) = 1/ln(sqrt(1-x))"