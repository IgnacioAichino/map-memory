## Laboratorio N6
# mmap
### SOI - FCEFyN - UNC - 2022

## Objetivo

Con el desarrollo del siguiente Trabajo Práctico (TP), se busca:
- Utilizar archivos binarios.
- Utilizar mecanismos de mapeo de memoria.

## Duración
Con el material dado en clase, este laboratorio está diseñado para resolverse entre 6 y 8 horas.


## Introducción
Este trabajo práctico consta en la elaboración de un programa en lenguaje C sobre _GNU/Linux_. El trabajo se divide en soluciones incrementales.

## Actividades
#### 1. Acceder datos
En  la carpeta _rawdata_ se encuentra el archivo binario llamado _datos_. El archivo contiene información obtenida por el radar meteorológio RMA1 instalado en Ciudad Universitaria (UNC).  Nuestro programa debe mapear el archivo en memoria ram y castearlo al tipo de dato de acuerdo a la siguiente estructura.

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


La estructura fue extraida de la documentación de INVAP y representa el header de un pulso de radar, con los metadatos que describen al mismo. Se debe prestar atención en los tamaños de cada variables.


#### 2. Instancias
Se debe leer todo el archivo para retornar la cantidad de instancias de muestras (estructuras) contenidas en el archivo.


#### 3. validSamples
Obtener la media de la variable _validSamples_ contenidas en el archivo.


## Criterios de Corrección
- Se debe compilar el código con los flags de compilación: -Wall -Pedantic -Werror 
- Dividir el código en módulos de manera juiciosa.
- Estilo de código.
- El código no debe contener errores, ni warnings.
- El código no debe contener errores de cppcheck.


## Qué se debe Entregar
- Informe del desarrollo del proyecto (markdown).
- Código (funcionando bajo las especificaciones dadas y bajo cualquier caso de test de parámetros de entrada).
- Makefile
- Todo en el GitHub

