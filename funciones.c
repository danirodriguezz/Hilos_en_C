#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <getopt.h> // Para el análisis de opciones de línea de comandos
#include "funciones.h"


// Variables globales
int R;
int turno = 0;
pthread_mutex_t cerrojo;
pthread_cond_t turno_cond = PTHREAD_COND_INITIALIZER;
int tamaño_vector = TAMAÑO_MAX_VECTOR;
int num_hilos = NUM_HILOS_MAX;
int usar_turno = 0;
char cadena_decimal[100];

// Estructura para los argumentos del hilo
struct thread_data {
    int thread_id;
    int *vector1;
    int *vector2;
    int size;
    int num_hilos;
};

// Función que ejecutará cada hilo con orden
void *dot_product(void *threadarg) {
    struct thread_data *my_data;
    my_data = (struct thread_data *) threadarg;
    int start = (my_data->thread_id * my_data->size) / my_data->num_hilos;
    int end = (my_data->thread_id + 1) * my_data->size / my_data->num_hilos;
    // Declaramos la zona crítica
    pthread_mutex_lock(&cerrojo);
    while (my_data->thread_id != turno)
    {   
        // Si no es el turno del hilo le declaramos que espere
        pthread_cond_wait(&turno_cond, &cerrojo);
    }
    // Si es el turno realizamos el cálculo
    for (int i = start; i < end; i++) {
        R += (int) (my_data->vector1[i] * my_data->vector2[i]);
    }
    turno = (turno + 1) % my_data->num_hilos;  // Cambiar al siguiente turno
    // Anunciamos a los hilos que hay un cambio de hilos
    pthread_cond_broadcast(&turno_cond);
    // Desbloqueamos la zona crítica para los cerrojos
    pthread_mutex_unlock(&cerrojo);
    // Terminamos de usar el hilo
    pthread_exit(NULL);
}

// Función que ejecutará cada hilo sin orden
void *dot_product2(void *threadarg) {
    struct thread_data *my_data;
    my_data = (struct thread_data *) threadarg;
    int start = (my_data->thread_id * my_data->size) / my_data->num_hilos;
    int end = (my_data->thread_id + 1) * my_data->size / my_data->num_hilos;
    // Realizamos el cálculo
    for (int i = start; i < end; i++) {
        // Declaramos la zona crítica
        pthread_mutex_lock(&cerrojo);
        R += (int) (my_data->vector1[i] * my_data->vector2[i]);
        // Terminamos la zona crítica
        pthread_mutex_unlock(&cerrojo);
    }
    pthread_exit(NULL);
}

void parametros(int argc, char *argv[]) {
    int opt;
    while ((opt = getopt(argc, argv, "n:h:t")) != -1) {
        switch (opt) {
            case 'n':
                tamaño_vector = atoi(optarg);
                break;
            case 'h':
                num_hilos = atoi(optarg);
                break;
            case 't':
                usar_turno = 1;
                break;
            default:
                fprintf(stderr, "Uso: %s -n <tamaño_vector> -h <num_hilos>\n", argv[0]);
                exit(EXIT_FAILURE);
        }
    }
    // Verificación de los valores ingresados
    if (tamaño_vector <= 0 || tamaño_vector > TAMAÑO_MAX_VECTOR) {
        fprintf(stderr, "Los valores de tamaño de vector deben estar entre 1 y %d\n", 
        TAMAÑO_MAX_VECTOR);
        exit(EXIT_FAILURE);
    } else if (num_hilos <= 0 || num_hilos > NUM_HILOS_MAX)
    {
        fprintf(stderr, "Los valores de los hilos deben ir entre 1 y %d\n", 
        NUM_HILOS_MAX);
        exit(EXIT_FAILURE);
    }
}

int inicializar_vectores() {
    int vector1[tamaño_vector];
    int vector2[tamaño_vector];
    // Inicialización de vectores
    for (int i = 0; i < tamaño_vector; i++) {
        vector1[i] = i;
        vector2[i] = i * 2;
    }
    // Calculamos el resultado esperado del producto escalar
    int R2 = 0;
    for (int i = 0; i < tamaño_vector; i++) {
        R2 += (int)(vector1[i] * vector2[i]);
    }
    return R2;
}

int productoEscalar(int R2) {
    struct thread_data td[num_hilos];
    pthread_t threads[num_hilos];
    int contador = 0;
    // Realizamos 10000 iteraciones para el producto escalar
    for (int i = 0; i < 10000; i++)
    {
        R = 0;
        // Creación de los hilos
        for (int j = 0; j < num_hilos; j++) {
            td[j].thread_id = j;
            // Declarando los vectores con memoria dinámica
            td[j].vector1 = (int *) malloc(tamaño_vector * sizeof(int));
            td[j].vector2 = (int *) malloc(tamaño_vector * sizeof(int));
            td[j].size = tamaño_vector;
            td[j].num_hilos = num_hilos;
            // Inicialización de vectores
            for (int i = 0; i < tamaño_vector; i++) {
                td[j].vector1[i] = i;
                td[j].vector2[i] = i * 2;
            }
            if (usar_turno == 1)
            {
                pthread_create(&threads[j], NULL, dot_product, (void *)&td[j]);
            } else {
                pthread_create(&threads[j], NULL, dot_product2, (void *)&td[j]);
            }
            
        }

        // Espera a que todos los hilos terminen
        for (int i = 0; i < num_hilos; i++) {
            pthread_join(threads[i], NULL);
        }
        // Liberando la memoria de los hilos
        for (int i = 0; i < num_hilos; i++) {
            free(td[i].vector1);
            free(td[i].vector2);
        }

        if (R != R2) {
            printf("El resultado por hilos fue %d\n", R);
            printf("Se esperaba que diera %d\n", R2);
            contador++;
        }
    }
    return contador;
}

void guardar_resultado() {
    // Convertir el resultado a string
    int chars_escribir = snprintf(cadena_decimal, sizeof(cadena_decimal), "%d", R);
    if (chars_escribir < 0) {
        perror("Error al convertir el resultado a cadena");
        exit(EXIT_FAILURE);
    }

    // Guardar el resultado en un fichero txt
    int h;
    h = open("resultado.txt", O_RDWR | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
    if (h == -1) {
        perror("Error al abrir el archivo");
        exit(EXIT_FAILURE);
    }
    if (write(h, cadena_decimal, chars_escribir) == -1) {
        perror("Error al escribir en el archivo");
        exit(EXIT_FAILURE);
    }
    close(h);
}

void mostrar_pcterror(int contador, int R2) {
    // Mostrando el porcentaje de error
    double porcentaje_error = (double)contador / 10;
    printf("El resultado del producto escalar es %d\n", R);
    printf("Se esperaba que el producto escalar diese %d\n", R2);
    printf("El porcentaje de error es del %.2f%% \n", porcentaje_error);
}

int archivo() {
    int contador_ceros = 0;
    int fd_origen, fd_destino;
    char *leido;
    int chars_escribir = snprintf(cadena_decimal, sizeof(cadena_decimal), "%d", R);
    // Abrir el archivo de origen para lectura
    fd_origen = open("resultado.txt", O_RDONLY);
    if (fd_origen == -1) {
        perror("Error al abrir el archivo de origen");
        return 1;
    }
    leido = (char *) malloc((chars_escribir + 1) * sizeof(char));
    read(fd_origen, leido, chars_escribir);
    leido[chars_escribir] = '\0';
    printf("He leido del archivo : %s\n", leido);
    // Contamos el número de 0 en el archivo
    for (int i = 0; i < sizeof(leido) + 1; i++) {
        if(leido[i] == '0') {
            contador_ceros++;
        }
    }
    printf("En resultado.txt hay %d ceros\n", contador_ceros);
    // Leer la fecha de creación del archivo de origen
    struct stat atributos;
    if (fstat(fd_origen, &atributos) == -1) {
        perror("Error al obtener los atributos del archivo");
        return 1;
    }
    time_t tiempo_creacion = atributos.st_ctime;
    struct tm *tiempo_info = localtime(&tiempo_creacion);

    // Abrir el archivo de destino para escritura
    fd_destino = open("resultado_copia.txt", O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
    if (fd_destino == -1) {
        perror("Error al abrir el archivo de destino");
        return 1;
    }

    // Escribir la fecha de creación en el archivo de destino
    dprintf(fd_destino, "Fecha de creación del archivo \"%s\": %d-%02d-%02d %02d:%02d:%02d\n",
            "resultado.txt",
            tiempo_info->tm_year + 1900, tiempo_info->tm_mon + 1, tiempo_info->tm_mday,
            tiempo_info->tm_hour, tiempo_info->tm_min, tiempo_info->tm_sec);

    // Cerrar los archivos
    close(fd_origen);
    close(fd_destino);

    printf("Fecha de creación copiada correctamente en \"%s\".\n", "resultado_copia.txt");
}