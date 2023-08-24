[![Review Assignment Due Date](https://classroom.github.com/assets/deadline-readme-button-24ddc0f5d75046c5622901739e7c5dd533143b0c8e959d652212380cedb1ea36.svg)](https://classroom.github.com/a/0GbZJ90g)
**Nombres de Integrantes:** [Completar aquí]

# Tarea 1 - Llamadas al sistema y planificación de procesos

Para comprender con mayor claridad cómo funciona un sistema operativo, en la primera parte de este curso trabajarás con un kernel real. El kernel es una adaptación del de Unix versión 6 (1975), llamada xv6, que puede ejecutar en
hardware x86 moderno, pues Unix versión 6 corría en un computador [PDP-11](https://en.wikipedia.org/wiki/PDP-11) en 1975. Dado que la gran parte de Unix fue escrita en el lenguaje de programación C, fue portado a lo largo de su historia a muchas arquitecturas, y ello es lo que permite que podamos trabajar con el en esta tarea.

La primera parte de esta tarea consiste en implementar una llamada al sistema bastante sencilla, lo cual te permitirá familiarizarte con programación y depuración de kernel ("_kernel hacking_"), y conocer la forma cómo son implementadas las llamadas y el comportamiento del sistema operativo para responder a ellas.

En la segunda parte de la tarea modificarás el planificador de procesos para incorporarle un algoritmo conocido como _lottery scheduling_ (planificación por lotería).

## Modalidad de trabajo

Esta tarea deberá ser resuelta en parejas. La totalidad del trabajo debe realizarse en el repositorio clonado después de aceptar la invitación en GitHub classroom. Para la entrega, se considerará la última revisión en el repositorio anterior a la fecha y hora de entrega. Para las partes 1 y 2 de la tarea las fechas de entrega son las siguientes:

* Primera parte (30% de la nota): 1 de septiembre, 23:59 hrs.
* Segunda parte (70% de la nota): 15 de septiembre, 23:59 hrs.

# Primera Parte: Llamada **getreadcount()**

La primera parte de esta tarea consiste en agregar una llamada de sistema al kernel de xv6, de nombre `getreadcount()`. Esta llamada retorna cuántas veces la llamada al sistema `read()` ha sido invocada por procesos de usuario desde el momento en que el kernel ha sido iniciado.

La nueva llamada al sistema debe tener la siguiente firma:

```c
int getreadcount(void)
```

El valor de retorno es el de un contador que debe implementarse internamente en el kernel, el cual debe ser incrementado cada vez que un proceso llama a
`read()`.

## Sincronización

Es necesario que el acceso al contador que actualiza la llamada `getreadcount()` sea sincronizado. Para esto, deberás crear y usar un spinlock en el kernel.  De lo contrario, las cuentas informadas por la llamada podrían ser inconsistentes.

Puedes ver el uso de un spinlock en `proc.c` (busca `ptable.lock`). La implementación de spinlocks la encuentras en `spinlock.{c,h}`. Para adquirir un spinlock usas la operación `acquire`, y para liberarlo `release`. Ambas operaciones requieren un puntero al spinlock.

## Consejos

Puedes revisar un playlist de YouTube con [vídeos](https://www.youtube.com/playlist?list=PL3yryPU8iwGO2IsoEa_F8_zIytuHIHV37) que explican todo lo necesario para comenzar comenzar tu _kernel hacking_ después de clonar el repositorio con  el código base de la tarea.

Una manera de comenzar a hackear un base de código relativamente grande como la de xv6 es buscar partes similares a lo que requieres hacer, y luego copiar y modificar éstas. Podrías buscar otra llamada al sistema, como `getpid()` u otra llamada sencilla. Luego, la copias en la forma que consideres útil, y la modificas para que se ajuste a los requisitos de la tarea.

La mayor parte del tiempo la dedicarás a entender el código. No es mucho el código que deberás implementar en la tarea.

Puedes usar el debugger gdb para trazar la ejecución del código del kernel. Para ello, recuerda compilar xv6 con el comando `make qemu-gdb`. Esto compilará y ejecutará xv6, y te permitirá depurarlo utilizando gdb remoto, desde otro terminal. Para ejecutar gdb, simplemente ingresas el comando `gdb kernel` en el directorio en donde se encuentre el código fuente de xv6.

## Compilación, Ejecución y Depuración

Para compilar e iniciar xv6, en el directorio en donde se encuentra el código de la tarea, se debe ejecutar el siguiente comando:

```sh 
prompt> make qemu-nox 
```

Para cerrar qemu, se debe presionar la combinación de teclas `Ctrl-a-x`.

Si se desea depurar el kernel con gdb, se debe compilar y ejecutar con 

```sh 
prompt> make qemu-nox-gdb
```

En un terminal paralelo, en el directorio del código base de la tarea se debe
ejecutar gdb:

```sh
prompt> gdb kernel
```

Algunas operaciones comunes con gdb son las siguientes:

* `c` para continuar la depuración. **Siempre se debe ingresar este comando cuando se inicia gdb**
* `b archivo:linea` para fijar un _breakpoint_ en cierto `archivo` y `linea`
  del mismo.
* `backtrace` (o `bt`) para mostrar un resumen de cómo ha sido la ejecución hasta el momento.
* `info registers` muestra el estado de registros de la CPU.
* `print`, `p` o `inspect` son útiles para evaluar una expresión.
*  Más información aquí: http://web.mit.edu/gnu/doc/html/gdb_10.html

En el kernel, puedes imprimir mensajes de depuración utilizando la función `cprintf`, la cual admite strings de formato similares a `printf` de la biblioteca estándar. Puedes ver los detalles de implementación en `kernel/console.c`.

Además, existe la función `panic` que permite detener la ejecución del kernel cuando ocurre una situación de error. Esta función muestra una traza de la ejecución hasta el momento en que es ejecutada. Los valores que muestra pueden ser buscados en `kernel/kernel.asm` para comprender cómo pudo haberse ejecutado la función.

## Ejecución en macOS con procesadores de Apple Silicon

Es posible ejecutar xv6 en un sistema con cpu Apple Silicon (M1 ó M2) con arquitectura ARM. Para esto, es necesario instalar una versión de GCC (GNU Compiler Collection) que genere ejecutables para arquitectura "i686", y usar el emulador qemu que simule una CPU compatible con dicha arquitectura.

Para instalar el compilador, necesitas el administrador de paquetes Homebrew. Instala Homebrew de acuerdo a las instrucciones en [brew.sh](https://brew.sh). Generalmente, la instalación sólo requiere un comando de una línea que se copia y pega al terminal desde dicha página.

Luego de instalar Homebrew, instala el _toolchain_ de GCC para arquitectura i686:

```sh
$ brew install i686-elf-gcc 
```

Cierra el terminal actual y abre un nuevo. Luego, en el terminal puedes escribir:

```sh
$ i6 # presiona TAB dos veces
```

Al presionar TAB dos veces luego de escribir 'i6' tu consola debiera mostrarte todos los ejecutables correspondientes a GCC para i686. Si `i686-elf-gcc` está disponible, entonces podrás compilar xv6 correctamente.

El siguiente paso de configuración es editar el `Makefile` de este proyecto. Busca la línea que define el prefijo de compilador que se usa. Se trata de la variable `TOOLPREFIX` que se encuentra alrededor de la línea 40. Modifica esta línea para que diga lo siguiente:

```Makefile
TOOLPREFIX=i686-elf-
```

Así, al momento de compilar xv6, se usarán todas las herramientas de GCC para la arquitectura i686, con el formato binario de ejecutable ELF.

En el terminal, ejecuta los siguientes comandos:

```sh
$ brew install qemu
$ which qemu-system-i386
```

El último comando debe arrojarte el path al programa `qemu-system-i386`, el cual debiese ser algo como `/opt/homebrew/bin/qemu-system-i386`.

Luego, debes configurar el `Makefile` para que xv6 ejecute en el emulador QEMU simulando la arquitectura x86 (i386). Busca la línea que contiene el comentario que dice `If the makefile can't find QEMU, specify its path here`, la cual es aproximadamente la línea 57. Debajo, declara la variable QEMU de la siguiente manera:

```Makefile
QEMU = /opt/homebrew/bin/qemu-system-i386
```

El path que asignas a la variable `QEMU` debe ser el mismo que obtuviste ejecutando el comando `which` arriba.

Finalmente, puedes compilar y ejecutar xv6 siguiendo las instrucciones en la sección "Compilación, Ejecución y Depuración" arriba.

## Test Automatizado

Contamos con un tester automatizado para probar la implementación de `getreadcount()` Para obtenerlo debes clonar [este repositorio](https://github.com/icc3201-202020/icc3201-syscalls_tester.git) en el **directorio padre de donde has clonado el código base de tarea**. Es decir, el repositorio con casos de prueba debe quedar en un directorio hermano en jerarquía al código de la tarea.

Para ejecutar un test que permite probar `getreadcount()` en forma automatizada, ingresa el siguiente comando en el directorio en donde has clonado el repositorio de xv6:

```sh
prompt> ./test-getreadcounts.sh 
```

Si implementaste todo correctamente, debieras ver un resultado diciendo que tu kernel ha pasado las pruebas. En caso contrario, podría haber algo incorrecto, por ejemplo, la sincronización para acceder al contador. Puedes ver los resultados de las pruebas en el directorio `tests-out` (en archivos `.out` y
`.err`).

El script de pruebas hace una compilación completa del código fuente de xv6 usando un makefile llamado `Makefile.test`, el cual es una variante autogenerada del `Makefile` original que incluye tests que son copiados al directorio `user` de xv6. Es posible correr tests directamente desde el shell de xv6 compilando el código con: 

```sh
prompt> cd src/ prompt> make -f Makefile.test qemu-nox
```

Puedes suprimir la recompilación de xv6 en las pruebas usando el flag `-s`. Esto te debiera permitir realizar pruebas más rápidamente.

```sh
prompt> ./test-getreadcounts.sh -s
```

Hay otros flags para pruebas disponibles en la [documentación](https://github.com/remzi-arpacidusseau/ostep-projects/blob/master/tester/README.md).

# Segunda Parte: Planificador de Lotería (_Lottery Scheduler_)

En la segunda parte de la tarea, modificarás el scheduler de xv6 para implementar planificación de procesos por lotería. Puedes ver una descripción detallada del algoritmo en [este capítulo](http://www.cs.wisc.edu/~remzi/OSFEP/cpu-sched-lottery.pdf) del libro TEP en línea. La idea es bastante sencilla: el planificador asigna a cada proceso en ejecución una parte del tiempo
del procesador proporcional a la cantidad de tickets que el proceso tiene. Mientras más tickets tiene un proceso, mayor es el tiempo dedicado a él. Cada decisión del planificador consiste en rifar un ticket de lotería y ver el proceso que gana la lotería. Procesos con más tickets tienen mayor probabilidad de ganar, obviamente, y lo contrario también se cumple.

## Llamadas al sistema requeridas

Necesitarás implementar dos llamadas al sistema para que este planificador funcione. La primera es `int settickets(int number)`, que permite asignar tickets al proceso invocador. Por defecto, cada proceso debe tener 10 tickets, sin embargo, en cualquier momento un proceso puede invocar la llamada e incrementar su proporción de ciclos de CPU. Esta rutina debe retornar 0 si es exitosa, o -1 en caso contrario (por ejemplo, si el invocador pasa un parámetro cuyo valor es menor a 1).

La segunda llamada que deberás implementar es `int getpinfo(struct pstat *)`. Esta rutina retorna información sobre los procesos en ejecución, incluyendo cuántas veces cada proceso ha sido despachado, y el PID de cada proceso. Puedes usar esta llamada al sistema para crear una variante del comando `ps` de Unix, utilizado para mostrar la información sobre procesos en ejecución.  La estructura `pstat` necesaria puede verse definida más abajo. Agrega esta estructura al código del kernel creando el archivo `pstat.h`. No debes cambiar esta estructura; tienes que usarla exactamente como viene definida. La rutina debe retornar 0 si hay éxito, o -1 en caso contrario, por ejemplo, si un puntero erróneo o nulo es pasado como parámetro al kernel.

```c 
#ifndef _PSTAT_H_
#define _PSTAT_H_ 
#include "param.h"
struct pstat { 
	int inuse[NPROC];   // whether this slot of the process table is in use (1 or 0) 
	int tickets[NPROC]; // the number of tickets this process has
	int pid[NPROC];     // the PID of each process 
	int ticks[NPROC];   // the number of times each process has been dispatched 
};

#endif // _PSTAT_H_ 
```

Encontrarás la implementación del scheduler en el archivo `proc.c`. El archivo header respectivo `header.h` también es útil para revisar. Para modificar el scheduler, no se requiere hacer grandes modificaciones. Debes rifar un número de lotería, y luego recorrer la lista de procesos viendo en qué proceso cae el ticket. Para ello, usas una variable que te permita ir acumulando los tickets de procesos revisados. Haz los cambios al scheduler en forma muy controlada, utilizando la función `cprintf` para imprimir mensajes de depuración a la consola. Puedes, por supuesto, usar gdb también poniendo _breakpoints_ en las líneas en donde desees trazar la ejecución del código. 

## Algunas consideraciones

* Debes asignar tickets a un proceso cuando es creado. Específicamente, deberás asegurarte que un proceso hijo recién creado *herede* el mismo número de tickets que su padre. En consecuencia, si el padre tiene 10 tickets, y llama a `fork()` para crear un proceso hijo, el hijo también debe quedar con 10 tickets. 
* Para rifar los tickets de lotería, puedes usar el generador de valores pseudoaleatorios disponible en el kernel, en archivos `rand.c/.h`. En particular, la función `random_at_most(long)` genera un valor aleatorio entre 1 y el valor de tipo `long` pasado como parámetro, inclusive.
* Debes comprender cómo llenar la estructura `pstat` en el kernel y pasar los resultados a espacio de usuario. La estructura debe verse como aparece definida arriba, y encontrarse en un archivo llamado `pstat.h`.  
* Puedes ver ejemplos de cómo pasar argumentos al kernel en las llamadas al sistema ya existentes. Podrías revisar cómo funciona la llamada `read()`, la cual te llevará a ver la función `sys_read()`. Esta función te mostrará cómo usar `argptr()` y funciones relacionadas, para obtener en espacio de kernel un puntero a datos pasados desde el espacio de usuario. El kernel debe tener mucha precaución con el manejo de datos provenientes desde el espacio de usuario, por la amenaza de seguridad que esto constituye. Todo argumento pasado a una llamada al sistema debe ser revisado muy cuidadosamente.

## Probando el scheduler

Para probar el funcionamiento del scheduler, puedes usar el programa [gist](https://gist.github.com/claudio-alvarez/a520ebffca00c1aa04ff8811c8c4bee1). Si agregas este archivo al directorio `user` y luego una entrada al `Makefile` para compilarlo, el programa `schedtest` quedará disponible para ejecutar desde el shell de xv6 cuando invoques make. Este programa lanza ejecuta un proceso que crea un hijo (con `fork`), de modo que el primer proceso queda con 30 tickets y el hijo con 10. Ambos procesos iteran por cientos de millones de iteraciones (el argumento de `schedtest` en la línea de comandos es cuántos cientos de millones de iteraciones deseas que ejecuten los procesos). El padre al terminar imprime una tabla que muestra cuántas veces fue despachado cada proceso. En función de la cantidad de tickets asignados, con ejecuciones repetidas del programa debiera observarse una tendencia del proceso del padre a ejecutar aproximadamente un 70% del tiempo total, y el hijo cerca de 20% restante, considerando que en xv6 hay otros procesos en ejecución.
