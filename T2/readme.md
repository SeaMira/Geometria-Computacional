# Tarea 1: Estructuras con Template en CPP

Esta es la implementación para la tarea 1 de Geometría Computacional: Punto, Vector y Poligono.

## Requisitos

Antes de comenzar, asegúrate de tener instalado CMake. Este proyecto ha sido testeado con CMake versión 3.x o superior.

## Compilación del Proyecto

Para compilar el proyecto, sigue estos pasos desde la raíz del directorio del proyecto (./T1):

### Configuración de CMake

Para preparar el entorno para la compilación, ejecuta:

```bash
cmake -S . -B ./build
```

### Compilación

Para compilar el proyecto, utiliza el siguiente comando:

```bash
cmake --build ./build -j 10
```

El flag -j 10 permite la compilación paralela usando 10 hilos, lo que puede acelerar el proceso de compilación en máquinas multicore.

### Ejecución de Tests

Una vez compilado el proyecto, puedes ejecutar los tests con `ctest` utilizando:

```bash
ctest --test-dir ./build --output-on-failure
```

Este comando ejecuta todos los tests definidos y muestra la salida solo cuando un test falla, facilitando la depuración.

### Ejecución del Main

El CMake de este proyecto está configurado para generar un ejecutable de nombre `structures.exe` en la carpeta `build` generada en los pasos anteriores. Para ejecutar este archivo se debe ejecutar el siguiente comando:

```bash
./build/structures.exe <mode> <size> <range_radius> <pcnt>
```

Donde "mode" varia entre 1 y 2.

- 1 ejecuta el programa para generar una nube de "size" puntos y encontrar la cerradura convexa con el algoritmo de Gift Wrapping y el algoritmo Incremental, donde los puntos tienen componentes aleatorias en un rango de (-"range_radius", "range_radius").

- 2 ejecuta el programa para generar una nube de "size" puntos y encontrar la cerradura convexa con el algoritmo de Gift Wrapping y el algoritmo Incremental, donde un porcentaje="pcnt" de los puntos están en una circunferencia de radio "range_radius", y el resto de los "size"(1-"pcnt") puntos tienen sus componentes aleatorias en un rango de (-("range_radius"*cos(pi/4)-0.1), "range_radius"*cos(pi/4)-0.1).

Todos los parámetros son obligatorios para que se ejecute el programa.

### Testing para el proyecto

El verdadero uso del ejecutable se hace en la carpeta experiment, con python. El jupyter adjunto (exp.ipynb) contiene todos los pasos de ejecución utilizados.

## Autor

Sebastian Andrés Mira Pacheco
