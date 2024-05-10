# include <stdio.h>
# include <pthread.h>

void *mensaje() {
    printf("==Hola Mundo==\n");
}

int main(int argc, char const *argv[])
{
    pthread_t thread1, thread2;
    //mensaje();
    pthread_create(&thread1, NULL, mensaje, NULL);
    pthread_create(&thread2, NULL, mensaje, NULL);
    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);
    return 0;
}

