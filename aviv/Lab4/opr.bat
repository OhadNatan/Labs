tcc -I..\..\.. -c -DTURBOC XFORK.C
tlib  xinu.lib -+ XFORK.obj

tcc -I..\..\.. -c -DTURBOC XFORKSF.C
tlib  xinu.lib -+ XFORKSF.obj

tcc -I..\..\.. lab4.c xinu.lib
