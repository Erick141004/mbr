#include <stdlib.h>
#include <stdio.h>

#define QUANTITY_OF_PARTITIONS 4
#define TOTAL_SIZE 512
#define PARTITION_SIZE 16
#define START_FIRST_PARTITION 446

typedef struct{
    __uint8_t status;
    __uint8_t chsStart[3];
    __uint8_t partitionType;
    __uint8_t chsEnd[3];
    __uint32_t lbaSector;
    __uint32_t numberSectors;
} Partition;

char *get_partition_type(__uint8_t partition){
    switch (partition)
    {
        case 0x00:
            return "Empty";
        case 0x82:
            return "Linux Swap";
        case 0x83:
            return "Linux";
        default:
            return "Unknown Partition";
    }
}

int main(int argc, char** argv){
    FILE *file = fopen("mbr.bin", "rb");

    if(file == NULL){
        printf("Não foi possivel ler o arquivo");
        return 0;
    }

    __uint8_t *content = (__uint8_t *)malloc(sizeof(__uint8_t)* TOTAL_SIZE);
    fread(content, 1, TOTAL_SIZE, file);
    fclose(file);

    __uint16_t boot_signature = content[510];
    printf("BOOT SIGNATURE: %x\n\n", boot_signature);

    for(int i = 0; i < TOTAL_SIZE; i++){
        if (i % 16 == 0){
            printf("\n");
        }else if(i % 4 == 0){
            printf(" ");
        }

        printf("%02x", content[i]);
    }

    int partitionIndex = START_FIRST_PARTITION;
    Partition *p = malloc(sizeof(Partition) * QUANTITY_OF_PARTITIONS);

    for(int i = 0; i < 1; i++){
        p[i].status = *(content + partitionIndex);
        printf("\n\nSTATUS DISCO: %x", p[i].status);
    }

    free(content);
    free(p);

    return 0;
}