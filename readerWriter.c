#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <fcntl.h>
sem_t mutex;
sem_t db;

int read_count = 0;
int data = 0;

void down(sem_t *s)
{
    sem_wait(s);
}

void up(sem_t *s)
{
    sem_post(s);
}

void *Reader(void *arg)
{
    int id = *(int *)arg;

    while (1)
    {
        down(&mutex);
        read_count++;

        if (read_count == 1)
            down(&db);

        up(&mutex);

        printf("Reader %d is reading data = %d\n", id, data);
        sleep(1);

        down(&mutex);
        read_count--;

        if (read_count == 0)
            up(&db);

        up(&mutex);

        sleep(1);
    }
    return NULL;
}

void *writer(void *arg)
{
    int id = *(int *)arg;

    while (1)
    {
        printf("Writer %d is preparing to write..\n", id);
        sleep(1);
        down(&db);
        data++;
        printf("Writer %d is writing data = %d\n", id, data);
        sleep(1);
        up(&db);
    }
    return NULL;
}

int main()
{
    pthread_t r1, r2, r3, w1;

    int id1 = 1, id2 = 2, id3 = 3, id4 = 1;

    sem_init(&mutex, 0, 1);
    sem_init(&db, 0, 1);

    pthread_create(&r1, NULL, Reader, &id1);
    pthread_create(&r2, NULL, Reader, &id2);
    pthread_create(&r3, NULL, Reader, &id3);
    pthread_create(&w1, NULL, writer, &id4);

    pthread_join(r1, NULL);
    pthread_join(r2, NULL);
    pthread_join(r3, NULL);
    pthread_join(w1, NULL);

    sem_destroy(&mutex);
    sem_destroy(&db);
}
