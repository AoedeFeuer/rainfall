# su bonus3
# pass: 71d449df0f960b36e0055eb58c14d0f5d0ddc0b35328d657f91cf0df15910587

====
Уязвимость: атой проверка
====

**Поведение: При запуске получаем либо пустую строку (если с 1 аргументом), либо ничего - в иных случаях.**

# ltrace ./bonus3 
__libc_start_main(0x80484f4, 1, 0xbffff804, 0x8048620, 0x8048690 <unfinished ...>
fopen("/home/user/end/.pass", "r")                                             = 0
+++ exited (status 255) +++

Проверяем, какие есть функции на этом уровне:

# i func
<>
0x080484f4  main
<>

Собственно говоря, смотрим main (других нет)

# disas main
Dump of assembler code for function main:
   0x080484f4 <+0>:     push   %ebp
   0x080484f5 <+1>:     mov    %esp,%ebp
   0x080484f7 <+3>:     push   %edi
   0x080484f8 <+4>:     push   %ebx
   0x080484f9 <+5>:     and    $0xfffffff0,%esp
   0x080484fc <+8>:     sub    $0xa0,%esp                   160
   0x08048502 <+14>:    mov    $0x80486f0,%edx              "r"
   0x08048507 <+19>:    mov    $0x80486f2,%eax              "/home/user/end/.pass"
   0x0804850c <+24>:    mov    %edx,0x4(%esp)
   0x08048510 <+28>:    mov    %eax,(%esp)
   0x08048513 <+31>:    call   0x8048410 <fopen@plt>
   0x08048518 <+36>:    mov    %eax,0x9c(%esp)              156 (esp + 0x9c сюда сохраняем значение fopen)
   0x0804851f <+43>:    lea    0x18(%esp),%ebx              24
   0x08048523 <+47>:    mov    $0x0,%eax                    0
   0x08048528 <+52>:    mov    $0x21,%edx                   33
   0x0804852d <+57>:    mov    %ebx,%edi
   0x0804852f <+59>:    mov    %edx,%ecx
   0x08048531 <+61>:    rep stos %eax,%es:(%edi)            memset
   0x08048533 <+63>:    cmpl   $0x0,0x9c(%esp)              0
   0x0804853b <+71>:    je     0x8048543 <main+79>          =
   0x0804853d <+73>:    cmpl   $0x2,0x8(%ebp)               2
   0x08048541 <+77>:    je     0x804854d <main+89>          !=
   0x08048543 <+79>:    mov    $0xffffffff,%eax
   0x08048548 <+84>:    jmp    0x8048615 <main+289>
   0x0804854d <+89>:    lea    0x18(%esp),%eax              24
   0x08048551 <+93>:    mov    0x9c(%esp),%edx
   0x08048558 <+100>:   mov    %edx,0xc(%esp)
   0x0804855c <+104>:   movl   $0x42,0x8(%esp)              66
   0x08048564 <+112>:   movl   $0x1,0x4(%esp)               1
   0x0804856c <+120>:   mov    %eax,(%esp)
   0x0804856f <+123>:   call   0x80483d0 <fread@plt>
   0x08048574 <+128>:   movb   $0x0,0x59(%esp)              0 в esp+89
   0x08048579 <+133>:   mov    0xc(%ebp),%eax               12
   0x0804857c <+136>:   add    $0x4,%eax
   0x0804857f <+139>:   mov    (%eax),%eax
   0x08048581 <+141>:   mov    %eax,(%esp)
   0x08048584 <+144>:   call   0x8048430 <atoi@plt>
   0x08048589 <+149>:   movb   $0x0,0x18(%esp,%eax,1)       0 в (esp + eax * 1 + 24)
   0x0804858e <+154>:   lea    0x18(%esp),%eax
   0x08048592 <+158>:   lea    0x42(%eax),%edx              66
   0x08048595 <+161>:   mov    0x9c(%esp),%eax              f
   0x0804859c <+168>:   mov    %eax,0xc(%esp)
   0x080485a0 <+172>:   movl   $0x41,0x8(%esp)              65
   0x080485a8 <+180>:   movl   $0x1,0x4(%esp)               1
   0x080485b0 <+188>:   mov    %edx,(%esp)
   0x080485b3 <+191>:   call   0x80483d0 <fread@plt>
   0x080485b8 <+196>:   mov    0x9c(%esp),%eax              f
   0x080485bf <+203>:   mov    %eax,(%esp)
   0x080485c2 <+206>:   call   0x80483c0 <fclose@plt>
   0x080485c7 <+211>:   mov    0xc(%ebp),%eax
   0x080485ca <+214>:   add    $0x4,%eax                    4
   0x080485cd <+217>:   mov    (%eax),%eax
   0x080485cf <+219>:   mov    %eax,0x4(%esp)
   0x080485d3 <+223>:   lea    0x18(%esp),%eax              24
   0x080485d7 <+227>:   mov    %eax,(%esp)
   0x080485da <+230>:   call   0x80483b0 <strcmp@plt>
   0x080485df <+235>:   test   %eax,%eax
   0x080485e1 <+237>:   jne    0x8048601 <main+269>         !=
   0x080485e3 <+239>:   movl   $0x0,0x8(%esp)               0
   0x080485eb <+247>:   movl   $0x8048707,0x4(%esp)         "sh"
   0x080485f3 <+255>:   movl   $0x804870a,(%esp)            "/bin/sh"
   0x080485fa <+262>:   call   0x8048420 <execl@plt>
   0x080485ff <+267>:   jmp    0x8048610 <main+284>
   0x08048601 <+269>:   lea    0x18(%esp),%eax
   0x08048605 <+273>:   add    $0x42,%eax                   66
   0x08048608 <+276>:   mov    %eax,(%esp)
   0x0804860b <+279>:   call   0x80483e0 <puts@plt>
   0x08048610 <+284>:   mov    $0x0,%eax
   0x08048615 <+289>:   lea    -0x8(%ebp),%esp
   0x08048618 <+292>:   pop    %ebx
   0x08048619 <+293>:   pop    %edi
   0x0804861a <+294>:   pop    %ebp
   0x0804861b <+295>:   ret    
End of assembler dump.

Через fread программа читает .pass файл и сохраняет в буфер. 
После чего программа переводит аргумент через атой и берет число за индекс строки (буфер), куда пишет 0.
И после чего происходит сравнение буфера и строки аргумента. Они должны быть равны.

Смысл в том, что если передать в атой пустую строку,то в нулевой индекс идет null, после чего сравнивается null с null и получаем доступ к шеллу

# ./bonus3 ""

# cat /home/user/end/.pass
3321b6f81659f9a71c76616f606e4b50189cecfea611393d5d649f75e157353c

# su end
3321b6f81659f9a71c76616f606e4b50189cecfea611393d5d649f75e157353c

# cat end
Congratulations graduate!