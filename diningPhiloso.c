#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <semaphore.h>
#include <unistd.h>

#define N 5

#define LEFT(i) (i + 1) % N
#define RIGHT(i) i

#define THINKING 0
#define HUNGARY 1
#define EATING 2

int state[N];

sem_t mutex;
sem_t s[N];

void down(sem_t *s)
{
    sem_wait(s);
}

void up(sem_t *s)
{
    sem_post(s);
}

void test(int i)
{
    if (state[i] == HUNGARY && state[LEFT(i)] != EATING && state[RIGHT(i)] != EATING)
    {
        state[i] = EATING;

        printf("Philosopher %d is eating\n", i);
        up(&s[i]);
    }
}

void take_forks(int i)
{
    down(&mutex);

    state[i] = HUNGARY;

    printf("Philosopher %d is hungary\n", i);

    test(i);

    up(&mutex);
    down(&s[i]);
}

void put_forks(int i)
{
    down(&mutex);

    state[i] = THINKING;

    printf("Philosopher %d is finished eating\n", i);

    test(LEFT(i));
    test(RIGHT(i));

    up(&mutex);
}

void *philosopher(void *arg)
{
    int i = *(int *)arg;

    while (1)
    {
        printf("Philosopher %d is thinking..\n", i);
        sleep(1);

        take_forks(i);

        sleep(2);

        put_forks(i);
    }
    return NULL;
}

int main()
{
    pthread_t philosophers[N];
    int id[N];

    sem_init(&mutex, 0, 1);

    for (int i = 0; i < N; i++)
    {
        state[i] = THINKING;
        sem_init(&s[i], 0, 0);
    }

    for (int i = 0; i < N; i++)
    {
        id[i] = i;
        pthread_create(&philosophers[i], NULL, philosopher, &id[i]);
    }

    for (int i = 0; i < N; i++)
    {
        pthread_join(philosophers[i], NULL);
    }
    return 0;
}
