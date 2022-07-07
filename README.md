# Tarea 3

## Enunciado

Se solicita crear un programa en lenguaje C/C++ que implemente una simulación del funcionamiento de las cajas registradoras de un supermercado. Para ello se pide utilizar bibliotecas de hebras de control pthread.h, semáforos semaphore.h y de manera opcional la de manejo de pantalla ncurses.h.

Requisitos:

  - Generar el(los) productor(es) (cliente(s)).
  - Generar el(los) consumidor(es) (caja registradora(s)).
  - Generar un número randomico entre dos números enteros.

Se debe desarrollar usando una metodología de programación modular permitiendo el llamado de funciones.

El programa debe ser desarrollado para que se ejecute sobre una plataforma Linux utilizando
el compilador gcc o g++.

## Funcionamiento en Linux

Para el funcionamiento en Linux, debemos seguir los siguientes pasos

1. Clonar el repositorio en alguna carpeta de Linux usando la siguiente línea de comando:

```git clone https://github.com/cmonttf/Tarea3_SO.git```  

2. El siguiente paso es acceder a la carpeta clonada con ```cd```:

```cd Tarea_SO```

3. Luego debemos compilar el archivo .c de la siguiente forma:

```gcc -o Tarea3 Tarea3.c -lpthread```

4. Y por último ejecutamos el archivo ```Tarea3```

```/.Tarea3```
