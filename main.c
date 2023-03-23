#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/mman.h>

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


int main(int argc, char **argv){

    //Lee file descriptor y su id
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

    //Cada lectura es un struc del pulso del radar.
    //info file descriptor 

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

    printf("Size rawdata: %d bytes\n", rawdata_size_bin);
    printf("Pulso de radar (data_struct): %d bytes\n", struct_rawdata);
    printf("Cantidad de muestras: %d\n", samples);


    //Mapeo de muestras
    struct data_struct *map_rawdata= mmap(NULL,rawdata_size_bin,PROT_READ,MAP_PRIVATE, rawdata_fd, 0);
    
    //Promedio 
    int avg= 0;
    for(int i=0; i<samples; i++){
        int data_sample= map_rawdata[i].validSample;
        avg += data_sample;
    }

    avg= avg/samples;
    printf("\n--------------\n");
    printf("Promedio validSample: %d\n", avg);

    //Libero mem y cierro file
    fclose(rawdata);
    munmap(map_rawdata, rawdata_size_bin);
    
    exit(EXIT_SUCCESS);
   
    return 0;
}