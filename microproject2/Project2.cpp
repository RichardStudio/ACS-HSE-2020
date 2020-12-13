#include <iostream>
#include <pthread.h>
#include <semaphore.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

using namespace std;

// ����������� ������.
int m;
// ���-�� �������.
int n;
// �����(������).
int buf;
//�������, ������������ ���������  ����� ����.
sem_t empty;
//�������, ������������ ��������� ����� �����.
sem_t full;
//�������.
pthread_mutex_t mutexF;
pthread_mutex_t mutexS;
// ������� ��� ����� �������. ����� ���������� ����� 10. 
int numOfCooking = 0;
// ����� ������������� 3 ��������� ����� �������.
bool isCookTired = false;

// C�������� ������� ������� � ��������������. ����� � ��� ������ ����.
void* Producer(void* param) {
    while (numOfCooking < 3) {
        pthread_mutex_lock(&mutexF);
        // ��������� �������� �������� � ���� ���� ��� ������.
        sem_wait(&empty);
        printf("The cook prepared the food. Now there are %d servings.\n", m);
        buf = m;  // ��������� ������(put item)
        numOfCooking++;
        for (int i = 0; i < m; i++) {
            // ����������� �������� �������� � ��������� ������� ����.
            sem_post(&full);
        }
        if (numOfCooking >= 3) {
            isCookTired = true;
        }
        pthread_mutex_unlock(&mutexF);
    }
    return nullptr;
}
// ����������� - ������.
void* Consumer(void* param) {
    int cNum = *((int*)param); // ����� ������
    while (!isCookTired) {
        sleep(10);
        pthread_mutex_lock(&mutexS);
        // ��������� �������� ��������. ���� �������� ���� - ���� ���� ����� ����������. 
        sem_wait(&full);
        printf("Barbarian number %d ate. %d servings left.\n", cNum + 1, buf - 1);
        // ��������� ����� ������ � ������.
        buf--;
        // ���� ��� �����������
        if (buf == 0) {
            // ����������� �������� �������� � ������� ������ � ���, ��� ���� ��������.
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
    // ������� ������.
    pthread_t threadP;
    pthread_create(&threadP, nullptr, Producer, (void*) nullptr);
    // ������� ������������ (�������)
    pthread_t threadC[n - 1];
    int consumers[n - 1];
    for (int i = 0; i < n - 1; i++) {
        consumers[i] = i + 1;
        pthread_create(&threadC[i], nullptr, Consumer, (void*)(consumers + i));
    }
    //����� ������� ����� ���� ����� ������������
    int mNum = 0;
    Consumer((void*)&mNum);
    return 0;
}
