reset
real_b=-abs(log(sqrt(1-W)))

output_log = sprintf ( "%s/Log_%s.log", dir, filename)

set fit logfile output_log errorvariables
a=-0.02
b=real_b
c=0.2
P(x) = a * exp( b * x ) + c
fit P(x) dir."/".filename.".dat" using 1:2 via a,b,c

command_echo = sprintf ("echo %f \t %f \t %f \t %f \t %f \t %f \t %f >> %s/%s.dat", W, a, b, c, a_err, b_err, c_err, dir, output_file)

system (command_echo)
unset fit
