/*
0* 1 1 1 1 1 1 1 0 0 0
1* 9 3 0 1 3 5 0 1 3 6
2* 0 1 0 0 0 0 0 0 0 0
3* 1 a b c ! 2 o r ! 0
4* 0 0 0 0 0 0 0 0 0 0
5* a b c 0 0 0 0 0 0 0
6* d e f 0 0 0 0 0 0 0
7* 0 0 0 0 0 0 0 0 0 0
8* 0 0 0 0 0 0 0 0 0 0
9* 0 0 0 0 0 0 0 0 0 0
*/

#ifndef FILESYSTEM_H
#define FILESYSTEM_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

// L磁盘的存储块总数
#define L 10
// B表示每个存储块的长度
#define B 10
// K为系统保留盘数大小
#define K 3
//文件描述符数量
#define fdcpsize ((K-1)*B/4)
// 模拟磁盘
char ldisk[L][B];

char tmp[10][20];

//位图，0代表空，1代表被占用
int bmp[L];

//文件描述符
struct Fdcp
{
	int len;//文件长度
	int fpos[3];//磁盘块号数组，0位为块号，1位为偏移，2位为状态，0为空闲1为占用
}fdcp[fdcpsize];

//目录
struct Index {
	int fdcpnum;//描述符序号
	char* fname;//文件名
	char end = '!';//每个目录尾
};
struct FIndex
{
	int size = 0;//目录总长
	int num = 0;//目录总数
	Index* index;//目录项
}findex;

//打开文件表
struct OFT {
	char* buff; //读写缓冲区
	int rwpos = 0; //读写指针
	int fpos = 0; //文件描述符号
}oft[5];

//I/O系统

// 该函数把逻辑块i的内容读入到指针p指向的内存位置，拷贝的字符个数为存储块的长度B
void read_block(int i, char* p);

// 该函数把指针p指向的内容写入逻辑块i，拷贝的字符个数为存储块的长度B
void write_block(int i, char* p);

// 存储到文件
void write_to_file();

/*把文件内容恢复到数组
 *若文件为空，则返回-1
 *若文件大小不对，返回-2
 *否则返回0
 */
int read_from_file();

// 文件系统

/*根据指定的文件名创建新文件
 *成功，返回0
 *无空闲描述符，返回-1
 *目录分配不成功，返回-2
 *没有空盘符，返回-3
 */
int create(char* filename);

/*删除指定文件
 *删除成功，返回0
 *没有该文件，返回-1
 */
int destroy(char* filename);

/*打开文件。该函数返回的索引号可用于后续的read,write,lseek,或close操作。
 *打开成功，返回0
 *没有该文件，返回-1
 *打开文件已满，返回-2
 */
int open(char* filename);

/*关闭指定文件
 *成功关闭，返回0
 *文件未打开，返回-1
 */
int close(int index);

/*从指定文件顺序读入count个字节mem area指定的内存位置。读操作从文件的读写指针指示的位置开始
 *成功读取，返回0
 *文件未打开，返回-1
 *到达文件尾，返回-2
 */
int read(int index, char* mem_area, int count);

/*把mem_area指定的内存位置开始的count个字节顺序写入指定文件。写操作从文件的读写指针指示的位置开始
 *写入成功，返回0
 *文件未打开，返回-1
 *count过大导致覆盖后续文件，返回-2
 */
int write(int index, char* mem_area, int count);

/*把文件的读写指针移动到pos指定的位置。pos是一个整数，表示从文件开始位置的偏移量。
 *文件打开时，读写指针自动设置为0。每次读写操作之后，它指向最后被访问的字节的下一个位置。
 *lseek能够在不进行读写操作的情况下改变读写指针能位置。
 *成功移动，返回0
 *文件未打开，返回-1
 *移动超过文件尾，返回-2
 */
int lseek(int index, int pos);

//列表显示所有文件及其长度
int directory();

//shell

//对内存、目录、描述符初始化
void init();

/*执行指令
 *正常执行，返回0
 *退出，返回-1
 *错误指令，返回-2
 */
int exec(char str[]);

//菜单
void menu();

#endif // !FILESYSTEM_H