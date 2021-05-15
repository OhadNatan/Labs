@REM tcc -I..\..\.. -c -DTURBOC INITIALI.C
@REM tlib  xinu.lib -+ INITIALI.obj

@REM tcc -I..\..\.. -c -DTURBOC XFORK.C
@REM tlib  xinu.lib -+ XFORK.obj

@REM @REM tcc -I..\..\.. -c -DTURBOC XFORKSF.C
@REM @REM tlib  xinu.lib -+ XFORKSF.obj

@REM tcc -I..\..\.. -c -DTURBOC KILL.C
@REM tlib  xinu.lib -+ KILL.obj

@REM tcc -I..\..\.. -c -DTURBOC XWAIT.C
@REM tlib  xinu.lib -+ XWAIT.obj


tcc -I..\..\.. -c -DTURBOC GETMEM.C
tlib  xinu.lib -+ GETMEM.obj

tcc -I..\..\.. -c -DTURBOC funcQ.C
tlib  xinu.lib -+ funcQ.obj



@REM tcc -I..\..\.. -c -DTURBOC clkint.C
@REM tlib  xinu.lib -+ clkint.obj



tcc -I..\..\.. lab4.c xinu.lib
