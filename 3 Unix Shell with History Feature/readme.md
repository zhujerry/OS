# UNIX Shell with History Feature

1	Summarize the process control methods provided by Linux(or Windows, if applied), and describe how to use which.

1.1	read()函数

函数定义：ssize_t read(int fd, void * buf, size_t count);


函数说明：read()会把参数fd所指的文件传送count 个字节到buf 指针所指的内存中。read 内部命令被用来从标准输入读取单行数据。这个命令可以用来读取键盘输入，当使用重定向的时候，可以读取文件中的一行数据。

返回值：返回值为实际读取到的字节数, 如果返回0, 表示已到达文件尾或是无可读取的数据。若参数count 为0, 则read()不会有作用并返回0。

1.2	fork()函数

fork用于进程创建，它能从已经存在的进程（父进程）中创建一个新的进程（子进程）。

在父进程中，fork返回新创建子进程的进程ID；在子进程中，fork返回0；如果出现错误，fork返回一个负值，通过fork的返回值，决定执行父进程还是子进程的代码。

父子进程共用一份代码，但是拥有两份代码段、数据段相同的数据。调用fork函数之后，父子进程各自拥有一个独立的执行流，父子进程均会从fork之后的指令开始执行。内核会分配新的内存块和内存数据结构给子进程，并将父进程部分数据结构内容拷贝给子进程。

fork之后，父进程和子进程两个执行流分别执行，谁先谁后不确定，由调度器决定。

1.3	execvp()函数

exec系统调用会从当前进程中把当前程序的机器指令清除，然后在空的进程中载入调用时指定的程序代码，最后运行这个新的程序。这样的定义就意味着，所有execvp()后面的代码都将不被执行，相当于在主函数里“重写”了一遍传入execvp函数中的程序，又在紧接着在后面加了句exit(1)；这样往往带来不便，但根据定义，我们可以将fork和execvp结合，从而保护父进程。

1.4	wait()函数

进程一旦调用了wait，就立即阻塞自己，由wait自动分析是否当前进程的某个子进程已经退出，如果让它找到了这样一个已经变成僵尸的子进程，wait就会收集这个子进程的信息，并把它彻底销毁后返回；如果没有找到这样一个子进程，wait就会一直阻塞在这里，直到有一个出现为止。


2	Your design of the program


2.1	概述

本程序分为如下几个函数以实现功能：

int execset(char *args[], char str[])实现对于args的初始化，以方便进行后续操作

void exe()用于执行指令，由于指令分为普通指令和历史记录中的指令，故分开两个函数来实现

void history(char str[])用于创建历史记录

本程序的全局变量有如下几个：

#define MAX_LINE 80                     /* 指令最大长度 */

char buf[MAX_LINE / 2 + 1][MAX_LINE];   /* 当前指令的缓冲区 */

char hty[10][MAX_LINE];                 /* 历史记录数组 */

int count = 0;                          /* 历史记录数量 */

int flag = 0;                           /* 指令特征 */

int num = 0;                            /* 调用执行历史记录函数的次数 */

2.3	函数功能

2.3.1	int execset(char *args[], char str[])

本函数实现对于args的设置。str[]为用户输入的指令内容，通过主函数来传入。根据指令内容，返回不同数值，其中普通指令返回0，存在&返回1，存在！返回2，指令是history返回3。

初始化buf和args后，按字符读取str内容，添加到buf当中，遇空格或结尾则结束，并把当前buf的值赋给args，随后指针加一。最后根据args[0]的特征返回对应的值。由此实现了对于指令的分词和分析功能。

2.3.2	void exe(char *args[])

根据flag的数值，即execset函数返回值，来决定如何运行当前指令。如果是普通指令，或存在&的指令，则fork子进程，调用execvp()函数来执行指令；如果是关于历史记录的指令，则调用exe(char *args[], char ch[])函数，进一步处理指令。

2.3.3	void exe(char *args[], char ch[])

根据count（历史记录数量）和flag来调用历史记录，找到相应记录后重新调用上述exe()函数来执行。根据ch的特征，分为直接显示历史记录、执行第几个历史记录、执行上一个历史记录、没有此条记录四中情况。

2.3.4	void history(char ch[])

创建、写入历史记录。将当前的指令，添加到全局变量hty[10][]数组中。如果数组已满，则前面指令依次往前挪一个单位，然后将指令插入到hty[9]。由于历史记录仅需要存储10个，故采用了字符数组这一简单的数据结构，如果存储数量较大，则应当采用栈的形式来储存。


3	Snapshots of experimental results(statistics) with analysis

略

4	Problems encountered and your solution

4.1	使用cin读入以空格作为结束

读入指令时使用cin，以空格作为结束。尝试过直接用循环读入到args当中，但cin无法直接将字符赋值给字符指针数组，使用缓冲区一个个字符读入，完成后再赋值给args，但简单的“args[i] = buf”后，随着后续的读入刷新buf值，前面args[i]的内容也会改变，导致args中永远存最后一个空格后的部分指令内容（如ls -l &，则args中均为&）。
改用read()函数读入指令，其返回值为读入的长度，方便截取和判断。

4.2	字符缓冲区被后续内容刷新

原先使用一位数组char buf[MAX_LINE]作为字符缓冲区，赋值给args[]，但发现只能存储到指令中最后一部分的内容，其它内容被冲掉。
改用二位数组char buf[MAX_LINE/2+1][MAX_LINE]来作为缓冲区，指令每个空格单独拥有一个缓冲区，再给args[]进行赋值。

4.3	与历史记录相关的指令和直接执行指令难以区分

两种指令难以直接区分，同时也难以归为一类进行运行。
单独设置准备函数（execset()）处理读入内容，设立flag标志位来表示指令特征。普通指令0，存在&1，存在！2，指令是history 3。分立两种执行函数exe(char *args[])和exe(char *args[], char ch[])，处理普通指令和历史记录。前者判断当前指令的类型，如果是普通指令，则直接运行，否则交由后者进一步处理，直到当前指令是普通指令为止。

4.4	历史记录中的!问题

当历史记录中存在!%d和!!时，即使交友后者exe继续处理，也无法变成普通指令。例如输入指令!!，而上一个指令恰好为!3或者!!时，当前指令的前一个指令并不能直接运行，需要进一步处理。又如输入指令!4，而4号指令为!2时，也需要进一步处理。
在exe(char *args[], char ch[])当中详细区分，设置num作为参考数。当指令!!，前一个指令仍为!开头，重新调用args设置函数execset(args, hty[last-num++])，仍然调用exe(args)，由于num数值的改变，待执行的指令会刷新，从!!变为!%d。循环执行，直到指令变为普通指令。第二种情况解决办法类似。


5	Reference materials

5.1	linux中fork()函数详解 - 学习记录园 - 博客园

https://www.cnblogs.com/dongguolei/p/8086346.html

5.2	execvp使用

https://blog.csdn.net/u011857683/article/details/81160059

5.3	基于fork(),execvp()和wait()实现类linux下的bash——mybash

https://www.cnblogs.com/elevator/p/7698864.html 

6	Your suggestions and comments

提供一些参考文档，关键函数使用实例。网上的使用教程很杂乱，重复率很高，难以找到合适的函数定义、使用的文章。
