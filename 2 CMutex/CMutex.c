#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

int accnt1 = 0, accnt2 = 0, counter = 0;
double time = 0.0, tbegin = 0.0, tend = 0.0;
HANDLE hMutex;

int c1 = 0, c2 = 0, will_wait;

////混乱
//DWORD WINAPI run(LPVOID p)
//{
//    int tmp1, tmp2, r;
//    do{
//        tmp1 = accnt1;
//        tmp2 = accnt2;
//        r = rand();
//        accnt1 = tmp1 + r;
//        accnt2 = tmp2 - r;
//        counter ++;
//    }while( accnt1 + accnt2 == 0 );
//    printf("%d\n%d\n%d\n", counter,accnt1,accnt2);
//}

////mutex方案
//DWORD WINAPI run(LPVOID p)
//{
//    int tmp1, tmp2, r;
//    while(1)
//    {
//        WaitForSingleObject( hMutex, INFINITE );
//        tmp1 = accnt1;
//        tmp2 = accnt2;
//        r = rand();
//        accnt1 = tmp1 + r;
//        accnt2 = tmp2 - r;
//        counter ++;
//        if(counter % 50000 == 0)
//            printf("%d ",counter);
//        if(accnt1 + accnt2)
//        {
//            printf("%d\n%d\n%d\n", counter, accnt1, accnt2);
//            ReleaseMutex( hMutex );
//            break;
//        }
//        if(counter == 500000)
//        {
//            tend = GetTickCount();
//            time = tend - tbegin;
//            printf("\n计数：%d\n用时：%lf\n",counter, time);
//        }
//        ReleaseMutex( hMutex );
//    }
//    printf("%d\n%d\n%d\n", counter,accnt1,accnt2);
//}

//软件方案
DWORD WINAPI run_1(LPVOID p)
{
    int tmp1, tmp2 , r;
    while(1)
    {
        c1 = 1;
        will_wait = 1;
        while( c2 && (will_wait==1) )Sleep(0);
        tmp1 = accnt1;
        tmp2 = accnt2;
        r = rand();
        accnt1 = tmp1 + r;
        accnt2 = tmp2 - r;
        counter ++;
        if(counter % 50000 == 0)
            printf("1-%d ",counter);
        if(accnt1 + accnt2)
        {
            printf("1-%d\n%d\n%d\n", counter,accnt1,accnt2);
            c1 = 0;
            break;
        }
        if(counter == 500000)
        {
            tend = GetTickCount();
            time = tend - tbegin;
            printf("\n线程1\n计数：%d\n用时：%lf\n",counter, time);
        }
        c1 = 0;
    }
}
DWORD WINAPI run_2(LPVOID p)
{
    int tmp1, tmp2, r;
    while(1)
    {
        c2 = 1;
        will_wait = 2;
        while( c1 && (will_wait==2) )Sleep(0);
        tmp1 = accnt1;
        tmp2 = accnt2;
        r = rand();
        accnt1 = tmp1 + r;
        accnt2 = tmp2 - r;
        counter ++;
        if(counter % 50000 == 0)
            printf("2-%d ",counter);
        if(accnt1 + accnt2)
        {
            printf("2-%d\n%d\n%d\n", counter,accnt1,accnt2);
            c2 = 0;
            break;
        }
        if(counter == 500000)
        {
            tend = GetTickCount();
            time = tend - tbegin;
            printf("\n线程2\n计数：%d\n用时：%lf\n",counter, time);
        }
        c2 = 0;
    }
}

int main(int argc, char *argv[])
{
    tbegin = GetTickCount();

//    hMutex = CreateMutex(NULL,FALSE,NULL);
//    CreateThread(NULL, 0, run, NULL, 0, NULL);
//    CreateThread(NULL, 0, run, NULL, 0, NULL);

    CreateThread(NULL, 0, run_1, NULL, 0, NULL);
    CreateThread(NULL, 0, run_2, NULL, 0, NULL);

    system("pause");
    return 0;
}
