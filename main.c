#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <getopt.h> // Para el análisis de opciones de línea de comandos
#include "funciones.h"

int main(int argc, char *argv[]) {
    int fd_origen, fd_destino;
    int contador;
    int R2;
    // Parseo de argumentos de línea de comandos
    parametros(argc, argv);
    // Incializa el vector y obtiene el resultado esperado
    R2 = inicializar_vectores();
    // Realiza el producto escalar por hilos y obtiene el contador de errores
    contador = productoEscalar(R2);
    // Guarda el resultado el resultado.txt
    guardar_resultado();
    // Muestra mensaje de error 
    mostrar_pcterror(contador, R2);
    // Muestra el n de 0 en resultado.txt y guarda la fecha en resultado_copia.txt
    archivo();
    return 0;
}
