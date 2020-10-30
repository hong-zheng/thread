#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <sys/ipc.h>
#include <string.h>
union semun{
        int val ;
} ;
int semid ;
void P(int no){
        struct sembuf sb ;
        sb.sem_num = no ;
        sb.sem_op  = -1 ;
        sb.sem_flg = 0 ; 
        semop(semid,&sb,1) ;
}
void V(int no){
        struct sembuf sb ;
        sb.sem_num = no ;
        sb.sem_op  = 1 ; 
        sb.sem_flg = 0 ; 
        semop(semid,&sb,1) ;
}
void get2fork(int no){
//      printf("%d get left  fork\n",no) ;

//      printf("%d get right fork\n",no) ;
        int left = no % 5 ;
        int right = (no+1) % 5 ;
        P(left) ;
        P(right) ;
}
void get2fork1(int no){
//      printf("%d get left  fork\n",no) ;
//      printf("%d get right fork\n",no) ;
        int left = no % 5 ;
        int right = (no+1) % 5 ;
        // 左右筷子同时都取,或者同时不拿
        struct sembuf sb[2] ;
        sb[0].sem_num = left ;
        sb[0].sem_op = -1 ;
        sb[0].sem_flg = 0 ;
        sb[1].sem_num = right ;
        sb[1].sem_op = -1 ;
        sb[1].sem_flg = 0 ;
        semop(semid,sb,2) ;
}
void put2fork(int no){
//      printf("%d get left  fork\n",no) ;
//      printf("%d get right fork\n",no) ;

    int left = no % 5 ;
        int right = (no+1) % 5 ;
        V(left) ;
        V(right) ;
}
void put2fork1(int no){
//      printf("%d get left  fork\n",no) ;
//      printf("%d get right fork\n",no) ;
        int left = no % 5 ;
        int right = (no+1) % 5 ;

        struct sembuf sb[2] ;
        sb[0].sem_num = left ;
        sb[0].sem_op  = 1 ;
        sb[0].sem_flg = 0 ;
        sb[1].sem_num = right ;
        sb[1].sem_op  = 1 ;
        sb[1].sem_flg = 0 ;
        semop(semid,sb,2) ;
}
void zhexuejia(int no){
        while(1){
        printf("%d , thinking...\n",no) ;
        sleep(rand()%3) ;

 printf("%d , hungry...\n",no) ;
        get2fork1(no) ;
        printf("%d , start eat...\n",no) ;
        sleep(rand()%2) ;
        printf("%d , end eat...\n",no) ;
        put2fork(no) ;
        }
}
void main(){
        srand(getpid()) ;
        // 创建5个信号量在信号量集 semid 中
        semid = semget(1234,5,IPC_CREAT|0644) ;

        int i ;
        // 设置信号量初值为1
        union semun su ;
        su.val = 1 ;
        for(i=0;i<5;++i){
                semctl(semid,i,SETVAL,su) ;
        }

        int no = 0 ;
        for(i=1;i<5;++i){
                pid_t pid = fork() ;
                if(pid==0){
                        no = i ;
                        break ;
                }
        }

        zhexuejia(no) ;
}
         