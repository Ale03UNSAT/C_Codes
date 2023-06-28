# Il programma grafica in un riuadro i file di dati..
# Necessita dei parametri:
# - «dir», ossia il percorso del file da graficare relativo alla cartella dove è presente "general_integratore.c"
# - «filename», ovvero il nome del file di tipo DAT da graficare senza l'estenzione ".dat"
# - «col1» e «col2», il numero delle colonne da graficare
# - «title_sK» la stringa che corrisponderà al titolo del file di output

reset
real_b=-abs(log(sqrt(1-W)))

output_log = sprintf ( "%s/Log_%s.log", dir, filename)

set fit logfile output_log errorvariables
a=-0.02
b=real_b
c=0.2
P(x) = a * exp( b * x ) + c
fit P(x) dir."/".filename.".dat" using 1:2 via a,b,c

command_echo = sprintf ("echo %f \t %f \t %f \t %f \t %f \t %f \t %f >> %s", W, a, b, c, a_err, b_err, c_err, dir, output_log)

system (command_echo)
unset fit
