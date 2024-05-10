#define TAMAÑO_MAX_VECTOR 1000 // Tamaño máximo del vector
#define NUM_HILOS_MAX 10 // Número máximo de hilos

void parametros(int argc, char *argv[]);
int inicializar_vectores();
int productoEscalar(int R2);
void *dot_product(void *threadarg);
void *dot_product2(void *threadarg);
void guardar_resultado();
void mostrar_pcterror(int contador, int R2);
int archivo();