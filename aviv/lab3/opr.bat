

tcc -I..\..\.. -c -DTURBOC func.C
tlib  xinu.lib -+ func.obj

tcc -I..\..\.. -c -DTURBOC initiali.C
tlib xinu.lib -+ initiali.obj

tcc -I..\..\.. -c -DTURBOC create.C
tlib xinu.lib -+ create.obj

tcc -I..\..\.. -c -DTURBOC CLKINT.C
tlib  xinu.lib -+ CLKINT.OBJ

tcc -I..\..\.. -c -DTURBOC resched.C
tlib  xinu.lib -+ RESCHED.obj

tcc -I..\..\.. -c -DTURBOC getitem.C
tlib xinu.lib -+ getitem.obj

tcc -I..\..\.. lab3.c xinu.lib
