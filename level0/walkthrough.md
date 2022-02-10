# ssh level0@localhost -p 4242 
# pass: level0

====
Уязвимость: разобрать дизасм код, понять, что нужно подать
====

**Поведение: сега**

Смотрим, что нам дано:
# ls -l /home/user/level0/
*-rwsr-x---+ 1 level1 users 747441 Mar  6  2016 level0*

Как мы видим на месте, где обычно установлен классический бит x (на исполнение), у нас выставлен специальный бит s.  Это позволяет обычному пользователю системы выполнять команды с повышенными привилегиями без необходимости входа в систему как этот пользователь.

Пробуем запустить: 
# /home/user/level0/level0
*Segmentation fault (core dumped)*

# ltrace not working
Статически собрано.

Запуск отладчика:
# gdb ./level0

# disas main

Расшифровка, построчный пример:

Dump of assembler code for function main:
   0x08048ec0 <+0>:     push   %ebp							Сохранить указатель фрейма стека от вызывающей программы в стеке.
   0x08048ec1 <+1>:     mov    %esp,%ebp					Создать новый указатель фрейма стека для вызываемой программы.
   0x08048ec3 <+3>:     and    $0xfffffff0,%esp			Выровнять стек по адресам, которые можно разделить на 16, установив младшие 4 бита в 0.
   0x08048ec6 <+6>:     sub    $0x20,%esp					Создать 32-байтовое пространство в стеке, например, для локальных переменных и прочего.
   0x08048ec9 <+9>:     mov    0xc(%ebp),%eax				Кладем в регистр значение argv[0]
   0x08048ecc <+12>:    add    $0x4,%eax					   Смещение 4
   0x08048ecf <+15>:    mov    (%eax),%eax					Кладем в регистр значение argv[1]
   0x08048ed1 <+17>:    mov    %eax,(%esp)					Передаем в указатель стека регистр
   0x08048ed4 <+20>:    call   0x8049710 <atoi>				Вызов atoi
   **0x08048ed9 <+25>:    cmp    $0x1a7,%eax					Сравнение результата с 0x1a7 (423)**
   0x08048ede <+30>:    jne    0x8048f58 <main+152>		Условный переход, если не равно - то на +152
   0x08048ee0 <+32>:    movl   $0x80c5348,(%esp)			"/bin/sh" в указатель стека
   0x08048ee7 <+39>:    call   0x8050bf0 <strdup>			strdup с аргументом
   0x08048eec <+44>:    mov    %eax,0x10(%esp)				сохранить в address of the thing that lies 16 bytes into the stack
   0x08048ef0 <+48>:    movl   $0x0,0x14(%esp)				null in address of the thing that lies 20 bytes into the stack
   0x08048ef8 <+56>:    call   0x8054680 <getegid>			get group process identification
   0x08048efd <+61>:    mov    %eax,0x1c(%esp)				сохранить в address of the thing that lies 28 bytes into the stack
   0x08048f01 <+65>:    call   0x8054670 <geteuid>			get user identification
   0x08048f06 <+70>:    mov    %eax,0x18(%esp)				сохранить в address of the thing that lies 24 bytes into the stack
   0x08048f0a <+74>:    mov    0x1c(%esp),%eax				получение в address of the thing that lies 28 bytes into the stack
   0x08048f0e <+78>:    mov    %eax,0x8(%esp)				сохранить в address of the thing that lies 8 bytes into the stack
   0x08048f12 <+82>:    mov    0x1c(%esp),%eax				получение в address of the thing that lies 28 bytes into the stack
   0x08048f16 <+86>:    mov    %eax,0x4(%esp)				сохранить в address of the thing that lies 4 bytes into the stack
   0x08048f1a <+90>:    mov    0x1c(%esp),%eax				address of the thing that lies 28 bytes into the stack
   0x08048f1e <+94>:    mov    %eax,(%esp)					Передаем в указатель стека регистр
   0x08048f21 <+97>:    call   0x8054700 <setresgid>		устанавливает идентификаторы групп реальных, эффективных и сохраненных пользователей процесса.
   0x08048f26 <+102>:   mov    0x18(%esp),%eax				address of the thing that lies 24 bytes into the stack
   0x08048f2a <+106>:   mov    %eax,0x8(%esp)				address of the thing that lies 8 bytes into the stack
   0x08048f2e <+110>:   mov    0x18(%esp),%eax				address of the thing that lies 24 bytes into the stack
   0x08048f32 <+114>:   mov    %eax,0x4(%esp)				address of the thing that lies 4 bytes into the stack
   0x08048f36 <+118>:   mov    0x18(%esp),%eax				address of the thing that lies 24 bytes into the stack
   0x08048f3a <+122>:   mov    %eax,(%esp)					Передаем в указатель стека регистр
   0x08048f3d <+125>:   call   0x8054690 <setresuid>		устанавливает идентификатор реального пользователя, идентификатор эффективного пользователя и идентификатор сохраненного пользователя текущего процесса
   0x08048f42 <+130>:   lea    0x10(%esp),%eax				указатель address of the thing that lies 16 bytes into the stack
   0x08048f46 <+134>:   mov    %eax,0x4(%esp)				address of the thing that lies 4 bytes into the stack
   0x08048f4a <+138>:   movl   $0x80c5348,(%esp)			положить "bin/sh"
   0x08048f51 <+145>:   call   0x8054640 <execv>			вызов execv с bin/sh
   0x08048f56 <+150>:   jmp    0x8048f80 <main+192>			переход на +192
   0x08048f58 <+152>:   mov    0x80ee170,%eax				stderr
   0x08048f5d <+157>:   mov    %eax,%edx					устанавливает eax в edx
   0x08048f5f <+159>:   mov    $0x80c5350,%eax				"No !\n"
   0x08048f64 <+164>:   mov    %edx,0xc(%esp)				address of the thing that lies 12 bytes into the stack
   0x08048f68 <+168>:   movl   $0x5,0x8(%esp)				address of the thing that lies 8 bytes into the stack
   0x08048f70 <+176>:   movl   $0x1,0x4(%esp)				address of the thing that lies 4 bytes into the stack
   0x08048f78 <+184>:   mov    %eax,(%esp)					Передаем в указатель стека регистр
   0x08048f7b <+187>:   call   0x804a230 <fwrite>			вызов fwrite
   0x08048f80 <+192>:   mov    $0x0,%eax
   0x08048f85 <+197>:   leave  
   0x08048f86 <+198>:   ret    
End of assembler dump.

Если перевести в примерный код, увидим, что для прохождения уровня нужно ввести число 423 (на него проверка) и тогда получим доступ к шеллу.

# ./level0 423
# whoami

# cat /home/user/level1/.pass
1fe8a524fa4bec01ca4ea2a869af2a02260d4a7d5fe7e7c24d8617e6dca12d3a

# exit

# su level1
