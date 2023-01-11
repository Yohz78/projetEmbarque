#include <pthread.h>
#include <stdio.h>
#include <unistd.h>

void *print_message(void *ptr) {
    while (1) {
        char *message;
        message = (char *) ptr;
        printf("%s\n", message);
        sleep(1);
    }
}

int main() {
    pthread_t thread1;
    const char *message1 = "Thread 1";
    int  iret1;

    iret1 = pthread_create(&thread1, NULL, print_message, (void*) message1);
    pthread_join( thread1, NULL);
    return 0;
}