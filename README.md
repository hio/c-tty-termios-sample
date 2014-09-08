c-tty-termios-sample
====================

This program requires terminal (tty).

Sample
------

    $ make
    cc -Wall -pedantic-errors   -c -o prog.o prog.c
    cc   prog.o   -o prog
    $ ./prog
    fd = 0
    ttyname = /dev/ttyp0
    (waiting for input)


Characters you typed are shown as character codes like these:

"hello" and enter:
`[68][65][6c][6c][6f][0a]`

"こんにちわ" (hello in Japanese) and enter:
`[e3][81][93][e3][82][93][e3][81][ab][e3][81][a1][e3][82][8f][0a]`
