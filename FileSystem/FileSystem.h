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

// L���̵Ĵ洢������
#define L 10
// B��ʾÿ���洢��ĳ���
#define B 10
// KΪϵͳ����������С
#define K 3
//�ļ�����������
#define fdcpsize ((K-1)*B/4)
// ģ�����
char ldisk[L][B];

char tmp[10][20];

//λͼ��0����գ�1����ռ��
int bmp[L];

//�ļ�������
struct Fdcp
{
	int len;//�ļ�����
	int fpos[3];//���̿�����飬0λΪ��ţ�1λΪƫ�ƣ�2λΪ״̬��0Ϊ����1Ϊռ��
}fdcp[fdcpsize];

//Ŀ¼
struct Index {
	int fdcpnum;//���������
	char* fname;//�ļ���
	char end = '!';//ÿ��Ŀ¼β
};
struct FIndex
{
	int size = 0;//Ŀ¼�ܳ�
	int num = 0;//Ŀ¼����
	Index* index;//Ŀ¼��
}findex;

//���ļ���
struct OFT {
	char* buff; //��д������
	int rwpos = 0; //��дָ��
	int fpos = 0; //�ļ���������
}oft[5];

//I/Oϵͳ

// �ú������߼���i�����ݶ��뵽ָ��pָ����ڴ�λ�ã��������ַ�����Ϊ�洢��ĳ���B
void read_block(int i, char* p);

// �ú�����ָ��pָ�������д���߼���i���������ַ�����Ϊ�洢��ĳ���B
void write_block(int i, char* p);

// �洢���ļ�
void write_to_file();

/*���ļ����ݻָ�������
 *���ļ�Ϊ�գ��򷵻�-1
 *���ļ���С���ԣ�����-2
 *���򷵻�0
 */
int read_from_file();

// �ļ�ϵͳ

/*����ָ�����ļ����������ļ�
 *�ɹ�������0
 *�޿���������������-1
 *Ŀ¼���䲻�ɹ�������-2
 *û�п��̷�������-3
 */
int create(char* filename);

/*ɾ��ָ���ļ�
 *ɾ���ɹ�������0
 *û�и��ļ�������-1
 */
int destroy(char* filename);

/*���ļ����ú������ص������ſ����ں�����read,write,lseek,��close������
 *�򿪳ɹ�������0
 *û�и��ļ�������-1
 *���ļ�����������-2
 */
int open(char* filename);

/*�ر�ָ���ļ�
 *�ɹ��رգ�����0
 *�ļ�δ�򿪣�����-1
 */
int close(int index);

/*��ָ���ļ�˳�����count���ֽ�mem areaָ�����ڴ�λ�á����������ļ��Ķ�дָ��ָʾ��λ�ÿ�ʼ
 *�ɹ���ȡ������0
 *�ļ�δ�򿪣�����-1
 *�����ļ�β������-2
 */
int read(int index, char* mem_area, int count);

/*��mem_areaָ�����ڴ�λ�ÿ�ʼ��count���ֽ�˳��д��ָ���ļ���д�������ļ��Ķ�дָ��ָʾ��λ�ÿ�ʼ
 *д��ɹ�������0
 *�ļ�δ�򿪣�����-1
 *count�����¸��Ǻ����ļ�������-2
 */
int write(int index, char* mem_area, int count);

/*���ļ��Ķ�дָ���ƶ���posָ����λ�á�pos��һ����������ʾ���ļ���ʼλ�õ�ƫ������
 *�ļ���ʱ����дָ���Զ�����Ϊ0��ÿ�ζ�д����֮����ָ����󱻷��ʵ��ֽڵ���һ��λ�á�
 *lseek�ܹ��ڲ����ж�д����������¸ı��дָ����λ�á�
 *�ɹ��ƶ�������0
 *�ļ�δ�򿪣�����-1
 *�ƶ������ļ�β������-2
 */
int lseek(int index, int pos);

//�б���ʾ�����ļ����䳤��
int directory();

//shell

//���ڴ桢Ŀ¼����������ʼ��
void init();

/*ִ��ָ��
 *����ִ�У�����0
 *�˳�������-1
 *����ָ�����-2
 */
int exec(char str[]);

//�˵�
void menu();

#endif // !FILESYSTEM_H