#include <pthread.h>  
#include <stdio.h>  
#include <stdlib.h>  
#include <unistd.h>  
  
void *thread_function2(void *arg) {  
    int method = *(int *) arg;  
  
    if (method == 0) {  
        printf("Нить %lu: Выполнение вложенной функции потока с возвратом через return.\n", pthread_self());  
        sleep(1);  
        return NULL;  
    }  
    else{  
        printf("Нить %lu: Выполнение вложенной функции потока с возвратом через pthread_exit.\n", pthread_self());  
        sleep(1);  
        pthread_exit(NULL);  // Демонстрация того, что такой вызов завершит всю нить целиком
    }  
}  
  
  
void *thread_function1(void *arg) {  
    int method = *(int *) arg;  
  
    if (method == 0) {  
        printf("Нить %lu: Выполнение главной функции потока с возвратом через return.\n", pthread_self());   
        thread_function2(arg);  
        printf("Нить %lu: Завершена возвратом из главной функции потока через return.\n", pthread_self());  
        return NULL; // Завершение нити через возврат
    } else if (method == 1) {  
        printf("Нить %lu: Выполнение главной функции потока с возвратом через pthread_exit.\n", pthread_self());    
        thread_function2(arg);  
        printf("Нить %lu: Главная функция потока завершена вызовом pthread_exit.\n", pthread_self());  
        pthread_exit(NULL); // Завершение нити через pthread_exit  
    } else {  
        printf("Нить %lu: Неизвестный метод завершения.\n", pthread_self());  
        int error = 1;  
        pthread_exit((void *) &error);  
    }  
}  
  
int main(void) {  
    pthread_t tid[2];  
    int methods[2] = {0, 1};  
    int result;  
  
    // Создание нитей и запись их идентификаторов в массив  
    for (int i = 0; i < 2; i++) {  
        result = pthread_create(&tid[i], NULL, thread_function1, &methods[i]);  
        if (result != 0) {  
            fprintf(stderr, "Ошибка создания первой нити\n");  
            exit(EXIT_FAILURE);  
        }  
    }  
  
    // Ожидание завершения нитей  
    for (int i = 0; i < 2; i++) {  
        pthread_join(tid[i], NULL);  
        printf("Главный поток: нить номер %d завершена.\n", i);  
    }  
  
    printf("Главный поток: Завершение программы.\n");  
    return 0;  
}