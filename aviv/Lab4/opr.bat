tcc -I..\..\.. -c -DTURBOC INITIALI.C
tlib  xinu.lib -+ INITIALI.obj

tcc -I..\..\.. -c -DTURBOC XFORK.C
tlib  xinu.lib -+ XFORK.obj


tcc -I..\..\.. -c -DTURBOC XFORKSF.C
tlib  xinu.lib -+ XFORKSF.obj

tcc -I..\..\.. -c -DTURBOC KILL.C
tlib  xinu.lib -+ KILL.obj

@REM tcc -I..\..\.. -c -DTURBOC XWAIT.C
@REM tlib  xinu.lib -+ XWAIT.obj


@REM tcc -I..\..\.. -c -DTURBOC GETMEM.C
@REM tlib  xinu.lib -+ GETMEM.obj

@REM tcc -I..\..\.. -c -DTURBOC funcQ.C
@REM tlib  xinu.lib -+ funcQ.obj



@REM tcc -I..\..\.. -c -DTURBOC clkint.C
@REM tlib  xinu.lib -+ clkint.obj



tcc -I..\..\.. lab4.c xinu.lib 

