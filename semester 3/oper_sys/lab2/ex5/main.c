#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

pthread_mutex_t mutex_room = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t woman_can_enter = PTHREAD_COND_INITIALIZER;
pthread_cond_t man_can_enter = PTHREAD_COND_INITIALIZER;

int woman_room_count = 0;
int man_room_count = 0;

void woman_wants_to_enter(int room_size) {
    pthread_mutex_lock(&mutex_room);

    while (woman_room_count >= room_size || man_room_count > 0) {
        pthread_cond_wait(&woman_can_enter, &mutex_room);
    }
    ++woman_room_count;

    pthread_mutex_unlock(&mutex_room);
}

void man_wants_to_enter(int room_size) {
    pthread_mutex_lock(&mutex_room);
    while (man_room_count >= room_size || woman_room_count > 0) {
        pthread_cond_wait(&man_can_enter, &mutex_room);
    }
    ++man_room_count;

    pthread_mutex_unlock(&mutex_room);
}

void woman_leaves() {
    pthread_mutex_lock(&mutex_room);
    --woman_room_count;
    if (woman_room_count > 0) {
        printf("+ ");
        pthread_cond_broadcast(&woman_can_enter);
    } else {
        printf("\nRoom: ");
        pthread_cond_broadcast(&man_can_enter);
        pthread_cond_broadcast(&woman_can_enter);
    }

    pthread_mutex_unlock(&mutex_room);
}

void man_leaves() {
    pthread_mutex_lock(&mutex_room);
    --man_room_count;
    if (man_room_count > 0) {
        printf("+ ");
        pthread_cond_broadcast(&man_can_enter);
    } else {
        printf("\nRoom: ");
        pthread_cond_broadcast(&woman_can_enter);
        pthread_cond_broadcast(&man_can_enter);
    }

    pthread_mutex_unlock(&mutex_room);
}

void* womanShower(void* arg) {
    int room_size = *(int *) arg;
    woman_wants_to_enter(room_size);
    printf("W ");

    sleep(1);
    woman_leaves();
    return NULL;
}

void* manShower(void* arg) {
    int room_size = *(int *) arg;
    man_wants_to_enter(room_size);
    printf("M ");

    sleep(1);
    man_leaves();
    return NULL;
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Request must include: %s <N>\n", argv[0]);
        return 1;
	}
    char* endptr;
    const int N = strtol(argv[1], &endptr, 10);
    if (N <= 0 || (*endptr != '\0' && *endptr != '\n')) {
        printf("Invalid input\n");
        return 0;
    }
    int max_in = N;
    const int men_count = 4 * N - 1;
    const int women_count = 4 * N - 2;
    pthread_t men[men_count];
    pthread_t women[women_count];
    printf("Room: ");

    for (int i = 0; i < men_count; ++i) {
        pthread_create(&men[i], NULL, manShower, &max_in);
    }
    for (int i = 0; i < women_count; ++i) {
        pthread_create(&women[i], NULL, womanShower, &max_in);
    }
    for (int i = 0; i < men_count; ++i) {
        pthread_join(men[i], NULL);
    }
    for (int i = 0; i < women_count; ++i) {
        pthread_join(women[i], NULL);
    }
    printf("\n");
    return 0;
}
