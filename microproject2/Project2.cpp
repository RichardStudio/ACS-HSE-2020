#include <iostream>
#include <pthread.h>
#include <semaphore.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

using namespace std;

// Вместимость горшка.
int m;
// Кол-во дикарей.
int n;
// Буфер(горшок).
int buf;
//семафор, отображающий насколько  буфер пуст.
sem_t empty;
//семафор, отображающий насколько полон буфер.
sem_t full;
//мутексы.
pthread_mutex_t mutexF;
pthread_mutex_t mutexS;
// Сколько раз повар готовил. Пусть максимумом будет 10. 
int numOfCooking = 0;
// После приготовления 3 горшочков повар устанет.
bool isCookTired = false;

// Cтартовая функция потоков – производителей. Повар у нас только один.
void* Producer(void* param) {
    while (numOfCooking < 3) {
        pthread_mutex_lock(&mutexF);
        // Уменьшаем значение семафора и ждем пока все съедят.
        sem_wait(&empty);
        printf("The cook prepared the food. Now there are %d servings.\n", m);
        buf = m;  // Заполняем горшок(put item)
        numOfCooking++;
        for (int i = 0; i < m; i++) {
            // Увеличиваем значение семафора и разрешаем дикарям есть.
            sem_post(&full);
        }
        if (numOfCooking >= 3) {
            isCookTired = true;
        }
        pthread_mutex_unlock(&mutexF);
    }
    return nullptr;
}
// Потребители - дикари.
void* Consumer(void* param) {
    int cNum = *((int*)param); // номер дикаря
    while (!isCookTired) {
        sleep(10);
        pthread_mutex_lock(&mutexS);
        // Уменьшаем значение семафора. Если горшочек пуст - ждум пока повар приготовит. 
        sem_wait(&full);
        printf("Barbarian number %d ate. %d servings left.\n", cNum + 1, buf - 1);
        // Уменьшаем число порций в горшке.
        buf--;
        // Если еда закончилась
        if (buf == 0) {
            // Увеличивает значение семафора и говорит повору о том, что пора работать.
            sem_post(&empty);
        }
        pthread_mutex_unlock(&mutexS);
    }
    return nullptr;
}

int main()
{
    cout << "enter the capacity of the pot\n";
    cin >> m;
    cout << "enter number of barbarians\n";
    cin >> n;
    pthread_mutex_init(&mutexF, nullptr);
    pthread_mutex_init(&mutexS, nullptr);
    sem_init(&empty, 0, 1);
    sem_init(&full, 0, 0);
    // Создаем повара.
    pthread_t threadP;
    pthread_create(&threadP, nullptr, Producer, (void*) nullptr);
    // Создаем потребителей (дикарей)
    pthread_t threadC[n - 1];
    int consumers[n - 1];
    for (int i = 0; i < n - 1; i++) {
        consumers[i] = i + 1;
        pthread_create(&threadC[i], nullptr, Consumer, (void*)(consumers + i));
    }
    //пусть главный поток тоже будет потребителем
    int mNum = 0;
    Consumer((void*)&mNum);
    return 0;
}
