#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <unistd.h>

#define CNT 5

sem_t forks[CNT];
pthread_mutex_t wait_Everyone;

void* eat_or_think(void* arg) {
    int id = *(int*)arg;
    int left_fork = id;
    int right_fork = (id + 1) % CNT;
    while(1) {
        pthread_mutex_lock(&wait_Everyone);
        // взятие правой и левой вилки должно произойти одновременно
        sem_wait(&forks[left_fork]);
        sem_wait(&forks[right_fork]);

        pthread_mutex_unlock(&wait_Everyone);
        printf("Философ%d ест\n", id + 1);
        sleep(1); // едим

        sem_post(&forks[left_fork]);
        sem_post(&forks[right_fork]);

        printf("Философ%d размышляет\n", id + 1);
        sleep(1);
    }
    return NULL;
}

void* grab_and_eat(void* arg) {
    int id = *(int*)arg;
    int left_fork = id;
    int right_fork = (id + 1) % CNT;
    while(1) {
        //pthread_mutex_lock(&waitEveryone);
        sem_wait(&forks[left_fork]);
        sem_wait(&forks[right_fork]);

        //pthread_mutex_unlock(&waitEveryone);
        printf("Философ%d ест\n", id + 1);
        //sleep(1); // едим

        sem_post(&forks[left_fork]);
        sem_post(&forks[right_fork]);

        printf("Философ%d размышляет\n", id + 1);
        sleep(1); // думаем
    }

    return NULL;
}

int main() {
    for (int i = 0; i < CNT; ++i) {
        if(sem_init(&forks[i], 0, 1) != 0) {
            return 1;
        }
    }
    if(pthread_mutex_init(&wait_Everyone, NULL) != 0) {
        return 1;
    }

    pthread_t philosophers[CNT];
    int id[CNT];
    for (int i = 0; i < CNT; ++i) {
        id[i] = i;
    }

    for (int i = 0; i < CNT; ++i){
        // grab_and_eat - проблема
        // eat_or_think - решение, разница в блокировке остальных философов
        pthread_create(&philosophers[i], NULL, eat_or_think, &id[i]);
    }
    for (int i = 0; i < CNT; ++i){
        pthread_join(philosophers[i], NULL);
    }

    for (int i = 0; i < CNT; ++i) {
        if(sem_destroy(&forks[i])) {
            return 1;
        }
    }
    pthread_mutex_destroy(&wait_Everyone);
    return 0;
}
