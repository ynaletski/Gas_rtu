SET PATH=C:\TC\BIN;C:\TC\INCLUDE;
tcc -ml -M -N -Ic:\tc\include -Lc:\tc\lib smp.c 7188xal.lib x607.lib
del *.obj