# Il programma grafica in un riuadro i file di dati..
# Necessita dei parametri:
# - «dir», ossia il percorso del file da graficare relativo alla cartella dove è presente "general_integratore.c"
# - «filename», ovvero il nome del file di tipo DAT da graficare senza l'estenzione ".dat"
# - «col1» e «col2», il numero delle colonne da graficare
# - «title_sK» la stringa che corrisponderà al titolo del file di output

reset
output_log = sprintf ( "%s/Log_%s.log", dir, filename)

set fit logfile output_log errorvariables
A=0.4
B=0.02
C=0.006
m(x) = A*exp( -B * x )+C
fit [10:][] m(x) dir."/".filename.".dat" using 1:3 via A,B,C

D=0.7
E=0.02
F=0.01
e(x) = -D*exp(-E * x )+F
fit [10:][] e(x) dir."/".filename.".dat" using 1:2 via D,E,F
unset fit

set term qt
command_echo_em = sprintf ("echo %f \t %f \t %f \t %f \t %f >> %s/%s.dat", Temp, B, B_err, E, E_err, dir, output_file)
system (command_echo_em)
