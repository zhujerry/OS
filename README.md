# OS
BJTU CIT OS class

实验二 线程同步

1.1 实验简介

本实验讨论临界区问题及其解决方案。实验首先创建两个共享数据资源的并发线程。在没有同步控制机制的情况下，
我们将看到某些异常现象。针对观察到的现象，本实验采用两套解决方案：

利用Windows的mutex机制

采用软件方案

然后比较这两种方案的性能优劣。


实验三 UNIX Shell with History Feature

Goal

The goal of this project is (1) to obtain a good understanding of operating system shell
and system calls, (2) to practice creating process and coordinate the running of parent
and child processes.

实验三 页面替换算法

3.1 实验简介

本实验要求实现多种页面替换算法，然后利用随机产生的引用串测试其性能。


实验四 文件系统

4.1实验简介

本实验要求在假设的I/O系统之上开发一个简单的文件系统，这样做既能让实验者对文件系统有整体了解，又避免了涉及过多细节。
用户通过create、open、read等命令与文件系统交互。文件系统把磁盘视为顺序编号的逻辑块序列，逻辑块的编号为0至L-1。
I/O系统利用内存中的数组模拟磁盘。
