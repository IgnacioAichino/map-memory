### Laboratorio N6
# mmap
### SOI - FCEFyN - UNC - 2022


### Compilacion
*Para la compilacion de este proyecto debera utilizar GCC*

Dentro de este repositorio se encuentra el archivo *Makefile* que le permitira compilar.

Comandos necesarios:

- make (Realiza la compilacion)

Este comando genera un ejecutable llamado **fdmapped**

#### Ejecucion

- ./fdmapped (Ejecuta el programa)

Este programa no necesita argumentos de entrada. Lo necesario para ejecutar este programa se encuentra en este repositorio.

#### Rawdata

En la carpeta rawdata se encuentra el archivo binario llamado _datos_. El archivo contiene información obtenida por el radar meteorológio RMA1 instalado en Ciudad Universitaria (UNC). 
Este archivo contine una cierta cantidad de pulsos del radar con informacion con el siguiente formato de estructura:


| Nombre      | tipo    |  bytes      | Descripción |  
| ----------- | ----------- | ----------- | ----------- |
| version      | unsigned int | 2 | versión del dato n1 |
| drxVersion   | unsigned int | 2 | version del DRX que generó el dato n1 |
| RESERVADO   |  | 4 |  |
| initCW   | double | 8 | inicio de la ventana de recepción, en segundos |
| azimuth   | float | 4 | apuntamiento acimut en grados |
| elevation   | float | 4 | apuntamiento elevación en grados |
| idVolumen   | unsigned int  | 2 | identificador de volumen |
| idBarrido   | unsigned int  | 2 | identificador de barrido |
| idCnjunto  | unsigned int  | 2 | identificador de conjunto |
| idGrupo  | unsigned int  | 2 | identificador de grupo |
| idPulso   | unsigned int  | 2 | identificador de pulso |
| iniBarrido   | bool | 1 | bandera del primer pulso de barrido |
| finBarrido   | bool  | 1 | bandera del último pulso de barrido |
| finGrupo   | bool  | 1 |  bandera del último pulso del grupo |
| inhibido   | bool  | 1 | bandera de transmisión inhibida |
| validSamples  | unsigned int  | 2 | cantidad de muestras complejas válidas |
| nroAdquisicion  | unsigned int  | 2 | contador de adquisiciones |
| RESERVADO   |  | 2 |  |
| nroSecuencia   | unsigned int | 4 | número de secuencia |
| readTime_high   | unsigned int | 8 | campo alto de la marca de tiempo |
| readTime_low   | unsigned int | 8 | campo bajo de la marca de tiempo |
| RESERVADO   |  | 64 |  |

#### Funcionamienro de fdmapped

El codigo fuente de este programa se encuentra en el file **main.c**
Se definio la estructura para almacenar los bytes e informacion correctamente en memoria del archivo en la carpeta rawdata:

```C
struct data_struct
{
    unsigned short int version; 
    unsigned short int drxVersion;
    const int RESERVED_1;
    double initCW;
    float azimuth;
    float elevation;
    unsigned short int idVolumen;
    unsigned short int idBarrio;
    unsigned short int idCnjunto;
    unsigned short int idGrupo;
    unsigned short int idPulso;
    bool iniBarrido;
    bool finBarrido;
    bool finGrupo;
    bool inihibo;
    unsigned short int validSample;
    unsigned short int nroAdquisicion;
    const unsigned short int RESERVED_2;
    unsigned int nroSecuencia;
    unsigned long int readTime_high;
    unsigned long int readTime_low;
    const unsigned long int RESERVED_3[8];
};

```
Mediante la funcion de **fopen()** abriremos el archivo y con **fileno()** obtendremos el valor entero asignado del file descriptor del archivo _datos_ en nuestro SO. 

```C
FILE *rawdata= fopen("./rawdata/datos", "rb");
    if( rawdata == NULL){
        fprintf(stderr, "Error open rawdata");
        exit(1);
    }

    int rawdata_fd= fileno(rawdata);
    if( rawdata_fd == -1){
        fprintf(stderr, "Error get int file descriptor");
        exit(1);
    }
```

Mediante la funciones **fstat()** podremos obtener informacion del archivo binario _datos_  usando el identificador entero del file descriptor de este archivo y la estructura de datos que nos brinda _<sys/stat.h>_ para las funcion utiliada.
De esta manera podremos obtener el valor en bytes del binario _datos_ y tambien calcular el valor en bytes del tamaño nuestra esctructura que representa el pulso del radar y podremos saber cuantas muestras se tomaron: 

```C
struct  stat rawdata_binary;
    int infobinary_rawdata= fstat(rawdata_fd, &rawdata_binary);
    if(infobinary_rawdata == -1){
        fprintf(stderr, "Error info binary");
        exit(1);
    }

    //Calculo de muestras
    int rawdata_size_bin = rawdata_binary.st_size;
    int struct_rawdata = sizeof(struct data_struct);
    int samples = rawdata_size_bin/struct_rawdata;
```

De esta manera obtendremos la cantidad de muestras haciendo la division de los valores de tamaño en bytes calculados  ----->  _rawdata_size_bin/struct_rawdata_

El resultado de la operacion anterior se muestra por pantalla.

Una vez logrado estos calculos podremos usarlos para calcular el promedio de **validSamples** (cantidad de muestras complejas válidas). Este valor sera obtenido de cada muestra.
Primero se mapeara en memoria el archivo binario _data_ reservando espacio en memoria mediante la funcion **mmap()**. Usaremos nuestro strcut para poder mapear la informacion contendida en el archivo binario y podes acceder a ella.
Se le indico al kernel que decida que direccion de memoria sera el comienzo del espacio reservado. El tamaño que ha de reservar es indicado gracias a los calculos anteriores por lo que sera un argumento el tamaño en bytes que se reservan (_rawdata_size_bin_). Entre los otros argumentos de la funcion definimos la proteccion del espacio de memoria reservado como tambien el entero asociado a nuestro file descriptor _datos_.

Luego podremos obtener los valores de **validSample** de cada muestra y hacer el calculo del promedio para mostrarlo en pantalla.

```C
struct data_struct *map_rawdata= mmap(NULL,rawdata_size_bin,PROT_READ,MAP_PRIVATE, rawdata_fd, 0);
    
    //Promedio 
    int avg= 0;
    for(int i=0; i<samples; i++){
        int data_sample= map_rawdata[i].validSample;
        avg += data_sample;
    }
```

El codigo finaliza liberando el espacio de memoria reservado para el archivo _data_. 

Se libera el espacio de memoria utilizando la funcion **munmap()**.

Se cierra la lectura del archivo con **fclose()**.

### Objetivo y Conclusiones 

El principal objetivo de este proyecto en Lenguaje C, era como asignar espacio de memoria a un archivo en la ejecucion de nuestro proceso que es nuestro programa **fdmapped**.

Se mapeo en memoria los bytes del archivo binario utilizado para este proyecto y se obtuvo informacion de su contenido para imprimir por consola la informacion requirida de _cantidad de muestras_ y _promedio de validSample_.

Este repositorio cuenta con el file main.c que ha sido desarrollado para lograr estos objetivos.

Cuenta con Makefile para compilar y limpiar el directorio de los archivos generados con el comando _make clean_ .
 