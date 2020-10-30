#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
pthread_mutex_t mutex ;
int g_ticket = 100 ;
void *run(void* arg){

        char* thread = (char*)arg ;
        while(1){
                pthread_mutex_lock(&mutex) ;
                if( g_ticket > 0){
                        printf("%s %d\n",thread,g_ticket) ;
                        g_ticket-- ;
                        pthread_mutex_unlock(&mutex) ;
                } else {
                        pthread_mutex_unlock(&mutex) ;
                        break ;
                }
                usleep(100) ;
        }
}
void main(){
        pthread_t t1 , t2 , t3 , t4 ;
        pthread_mutex_init(&mutex,NULL) ;
        pthread_create(&t1,NULL,run,"thread1") ;
        pthread_create(&t2,NULL,run,"thread2") ;
        pthread_create(&t3,NULL,run,"thread3") ;
        pthread_create(&t4,NULL,run,"thread4") ;
        pthread_join(t1,NULL) ;
        pthread_join(t2,NULL) ;
        pthread_join(t3,NULL) ;
        pthread_join(t4,NULL) ;
        pthread_mutex_destroy(&mutex) ;
}



/*
// 手动实现 卖票，不用互斥量
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/sem.h>
#include <pthread.h>
int g_ticket = 100 ;
int semid  ;
int sem_create(key_t key){
        return semget(key,1,IPC_CREAT|0644) ;
}
union semun{
        int val ;
} ;
void sem_setval(int semid,int val){
        union semun su ;
        su.val = val ;
        semctl(semid,0,SETVAL,su) ;    
}
void P(int semid){
        struct sembuf sb ;    
        sb.sem_num = 0 ; 
        sb.sem_op  = -1 ;
        sb.sem_flg = 0 ; 
        semop(semid,&sb,1) ;
}
void V(int semid){
        struct sembuf sb ;

        sb.sem_num = 0 ;
        sb.sem_op  = 1 ;
        sb.sem_flg = 0 ;
        semop(semid,&sb,1) ;
}
void* run(void* arg){
        char* thread = (char*)arg ;

        while(1){
                P(semid) ;
                if(g_ticket>0){
                        printf("%s %d\n",thread,g_ticket--) ;
                }else{
                        V(semid) ;
                        break ;
                }
                V(semid) ;
                usleep(100) ;
        }
}
void main(){
        semid = sem_create(1234) ;
        sem_setval(semid,1) ;
        pthread_t t1 , t2 , t3 , t4 ;
       pthread_create(&t1,NULL,run,"thread1") ;
        pthread_create(&t2,NULL,run,"thread2") ;
        pthread_create(&t3,NULL,run,"thread3") ;
        pthread_create(&t4,NULL,run,"thread4") ;
        pthread_join(t1,NULL) ;
        pthread_join(t2,NULL) ;
        pthread_join(t3,NULL) ;
        pthread_join(t4,NULL) ;
}


*/