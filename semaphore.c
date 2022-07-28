
/* Programa de ejemplo de semaphore. */
/* para compilar usar: gcc -o tarea3_ejemplo tarea3_ejemplo.c -lpthread */

#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


typedef struct {
  int capacidad_almacen;                   //tam corera      random
  float* almacen;                   
  int rondas;                             //cada ronda es un cliente de la caja  random
  int demora_min_productor;               //cliente  
  int demora_max_productor;                  
  int demora_min_consumidor;              //cajero    
  int demora_max_consumidor;
  sem_t puede_producir;
  sem_t puede_consumir;
} datos_compartidos_t;



void* produce(void* data); //cliente
void* consume(void* data); //cajero
int random_entre(int min, int max);    //genera un random entre max y min 

int main(int argc, char* argv[]) {    
  int error;
  struct timespec tiempo_ini;
  struct timespec tiempo_fin;
  pthread_t productor, consumidor;
  datos_compartidos_t datos_compartidos;

  srandom(time(NULL));

  if (argc == 5) {    //nombre de programa mas 4 variables   (parametros de entrada por consola)
    //datos_compartidos.capacidad_almacen=atoi(argv[1]);
    datos_compartidos.capacidad_almacen=random_entre(1, 10);
   // datos_compartidos.rondas=atoi(argv[2]);
    datos_compartidos.rondas=random_entre(1, 10);
    datos_compartidos.demora_min_productor=atoi(argv[1]);
    datos_compartidos.demora_max_productor=atoi(argv[2]);
    datos_compartidos.demora_min_consumidor=atoi(argv[3]);
    datos_compartidos.demora_max_consumidor=atoi(argv[4]);
  } else {
    printf("Usar: %s  demora_min_productor demora_max_productor demora_min_consumidor demora_max_consumidor\n", argv[0]);
    exit(EXIT_FAILURE);
  }

  datos_compartidos.almacen = (float*) calloc(datos_compartidos.capacidad_almacen, sizeof(float)); //se crea la correa
  sem_init(&datos_compartidos.puede_producir, 0, datos_compartidos.capacidad_almacen);          //si inicializa los semaforos
  sem_init(&datos_compartidos.puede_consumir, 0, 0);      //cero es que solo afecta al proceso


  clock_gettime(CLOCK_MONOTONIC, &tiempo_ini); //capturar el tiempo de proceso 

  error = pthread_create(&productor, NULL, produce, &datos_compartidos);
  if (error == 0) {
    error = pthread_create(&consumidor, NULL, consume, &datos_compartidos);
    if (error != 0) {
      fprintf(stderr, "error: no puede crear consumidor\n");
      error = 1;
    }
  } else {
    fprintf(stderr, "error: no puede crear productor\n");
    error = 1;
  }
  if (error == 0) {
    pthread_join(productor, NULL);
    pthread_join(consumidor, NULL);
  }

  clock_gettime(CLOCK_MONOTONIC, &tiempo_fin);     //capturar el tiempo de proceso 

  float periodo = (tiempo_fin.tv_sec - tiempo_ini.tv_sec) + 
          (tiempo_fin.tv_nsec - tiempo_ini.tv_nsec) * 1e-9;
  printf("Tiempo de ejecución: %.9lfs\n", periodo);  //para saber el tiempo de ejecucion 

  sem_destroy(&datos_compartidos.puede_consumir);
  sem_destroy(&datos_compartidos.puede_producir);
  free(datos_compartidos.almacen);

  return EXIT_SUCCESS;
}

void* produce(void* data) {
  datos_compartidos_t* datos_compartidos = (datos_compartidos_t*)data;
  int contador = 0; //identificar los productos
  for (int ronda = 0; ronda < datos_compartidos->rondas; ++ronda) { //cuantas veces va a producir algo clientes
    printf("INICIO RONDA P: %i\n", ronda);
    for (int indice = 0; indice < datos_compartidos->capacidad_almacen; ++indice) { //
      sem_wait(&datos_compartidos->puede_producir); //si tiene un valor mayor a cero -1 hace
      usleep(1000 * random_entre(datos_compartidos->demora_min_productor, datos_compartidos->demora_max_productor));//esperar un valor entre los tiempos de produccion
      datos_compartidos->almacen[indice] = ++contador; 
      printf("Indice almacen %i se produce %lg\n", indice, datos_compartidos->almacen[indice]);
      sem_post(&datos_compartidos->puede_consumir);//aumenta para poder consumir
    }
  }
  return NULL;
}

void* consume(void* data) {
  datos_compartidos_t* datos_compartidos = (datos_compartidos_t*)data;
  for (int ronda = 0; ronda < datos_compartidos->rondas; ++ronda) { //clientes cantidad
    printf("\t\tINICIO RONDA C: %i\n", ronda);
    for (int indice = 0; indice < datos_compartidos->capacidad_almacen; ++indice) { //
      sem_wait(&datos_compartidos->puede_consumir); //pregunta si hay algo en la correa
      float value = datos_compartidos->almacen[indice];//9*+de*-9sxdc
      usleep(1500 * random_entre(datos_compartidos->demora_min_consumidor
        , datos_compartidos->demora_max_consumidor));
      printf("\t\tIndice almacen %i se consume %lg\n", indice, value);
      sem_post(&datos_compartidos->puede_producir);
    }
  }
  return NULL;
}

int random_entre(int min, int max) {
  int aux=0;
  if (max > min)
    aux=random() % (max - min);
  return min + aux;
}