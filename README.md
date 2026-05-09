# Producto Escalar con Hilos en C

Este proyecto se desarrolló como parte de la asignatura de **Ingeniería de Computadores 2**. Consiste en la implementación de un programa en C que calcula el producto escalar de dos vectores concurrentemente utilizando la biblioteca `pthreads`.

El objetivo principal es demostrar la creación y gestión de hilos, el uso de secciones críticas, la sincronización entre procesos y la manipulación de archivos en sistemas POSIX.

## ✨ Características Principales

* **Cálculo Concurrente**: División equilibrada de la carga de trabajo entre múltiples hilos para calcular el producto escalar.
* **Gestión de Memoria Dinámica**: Asignación dinámica de memoria (usando `malloc` y `free`) para los vectores, adaptándose al tamaño especificado por el usuario.
* **Sincronización Avanzada**:
    * Uso de cerrojos o *mutex* (`pthread_mutex_t`) para proteger secciones críticas (como la suma en la variable global del resultado).
    * Uso de variables de condición (`pthread_cond_t`) para permitir la ejecución estrictamente ordenada de los hilos (por turnos) si el usuario lo requiere.
* **Manipulación de Archivos**: 
    * Escritura de los resultados en un archivo `resultado.txt`.
    * Lectura de archivos y uso de la estructura `stat` para extraer los metadatos (fecha de creación) de `resultado.txt` y volcarlos en `resultado_copia.txt`.
* **Interfaz de Línea de Comandos**: Configuración flexible mediante la librería `getopt` para ajustar parámetros como el tamaño del vector y la cantidad de hilos.

## 🗂️ Estructura del Proyecto

El proyecto está modularizado para facilitar su comprensión y escalabilidad:

* `main.c`: Archivo principal que orquesta la ejecución y llama a las distintas funciones.
* `funciones.c` / `funciones.h`: Contienen la implementación de la lógica concurrente (`dot_product` y `dot_product2`), inicialización, validación y gestión de archivos.
* `Makefile`: Script de automatización para compilar y limpiar el proyecto de manera rápida.
* `helloWorld.c` y `productoEscalar.c`: Versiones y pruebas incrementales previas documentadas en la memoria.
* `Trabajo_Hilos.pdf` / `Trabajo_Hilos.Rmd`: Memoria técnica detallada del paso a paso del desarrollo del proyecto.

## 🛠️ Compilación y Ejecución

### Prerrequisitos
Es necesario disponer de un compilador de C (como `gcc`), la utilidad `make` y estar en un entorno que soporte `pthreads` (como Linux o macOS).

### Compilación
Para compilar el proyecto final, sitúate en la raíz del repositorio y ejecuta:

```bash
make
