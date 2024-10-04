#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include "mbr.h"

void get_real_address(void* value, size_t size){
    __uint8_t* bytes = (__uint8_t *) value;
    for(int i = 0; i < size / 2; i++){
        __uint8_t aux = bytes[i];
        bytes[i] =  bytes[size - i - 1];
        bytes[size - i - 1] = aux;
    }
}

char *get_partition_type(__uint8_t partition){
    switch (partition)
    {
        case 0x00:
            return "Empty";
        case 0x0b:
            return "W95 FAT32";
        case 0x83:
            return "Linux";
        default:
            return "Unknown Partition";
    }
}

int main(int argc, char** argv){
    FILE *file = fopen("mbr.bin", "rb");

    if(file == NULL){
        printf("Não foi possivel ler o arquivo!");
        return 0;
    }

    __uint8_t *content = (__uint8_t *)malloc(sizeof(__uint8_t)* TOTAL_SIZE);
    fread(content, 1, TOTAL_SIZE, file);
    fclose(file);

    __uint16_t boot_signature = *((__uint16_t*)(content + SIGNATURE));
    get_real_address(&boot_signature, sizeof(boot_signature));
    printf("BOOT SIGNATURE: 0x%x\n\n", boot_signature);

    __uint32_t disk_id = *((__uint32_t*)(content + DISK_ID));
    printf("DISK ID: 0x%x\n\n", disk_id);

    if(boot_signature != 0x55aa){
        printf("A assinatura não é válida");
        return 0;
    }

    for(int i = 0; i < TOTAL_SIZE; i++){
        if (i % 16 == 0){
            printf("\n");
        }else if(i % 2 == 0){
            printf(" ");
        }

        printf("%02x", content[i]);
    }

    int partition_index = START_FIRST_PARTITION;
    Partition *p = malloc(sizeof(Partition) * QUANTITY_OF_PARTITIONS);
    __uint32_t total_sectors = 0;

    for(int i = 0; i < QUANTITY_OF_PARTITIONS; i++){
        printf("\n\n===================BEGIN NEW PARTITION=======================");
        __uint8_t next_value = 0;

        p[i].status = *(content + partition_index);
        printf("\n\nSTATUS DISCO: %x", p[i].status);
        next_value += sizeof(p[i].status);

        p[i].chs_start[0] = *(content + partition_index + next_value);
        next_value += sizeof(p[i].chs_start[0]);
        p[i].chs_start[1] = *(content + partition_index + next_value);
        next_value += sizeof(p[i].chs_start[1]);
        p[i].chs_start[2] = *(content + partition_index + next_value);
        next_value += sizeof(p[i].chs_start[2]);
        get_real_address(p[i].chs_start, sizeof(p[i].chs_start));
        printf("\nVALUE CHSSTART: %x", p[i].chs_start[0]);
        printf("\nVALUE CHSSTART: %x", p[i].chs_start[1]);
        printf("\nVALUE CHSSTART: %x", p[i].chs_start[2]);  

        p[i].partition_type = *(content + partition_index + next_value);
        printf("\nVALUE PARTITION_TYPE: %x", p[i].partition_type);
        next_value += sizeof(p[i].partition_type);

        p[i].chs_end[0] = *(content + partition_index + next_value);
        next_value += sizeof(p[i].chs_end[0]);
        p[i].chs_end[1] = *(content + partition_index + next_value);
        next_value += sizeof(p[i].chs_end[1]);
        p[i].chs_end[2] = *(content + partition_index + next_value);
        next_value += sizeof(p[i].chs_end[2]);
        get_real_address(p[i].chs_end, sizeof(p[i].chs_end));
        printf("\nVALUE CHSEND: %x", p[i].chs_end[0]);
        printf("\nVALUE CHSEND: %x", p[i].chs_end[1]);
        printf("\nVALUE CHSEND: %x", p[i].chs_end[2]);

        p[i].lba_sector = *((__uint32_t*)(content + partition_index + next_value));
        //get_real_address(&p[i].lba_sector, sizeof(p[i].lba_sector));
        printf("\nVALUE LBASECTOR: %x", p[i].lba_sector);
        next_value += sizeof(p[i].lba_sector);

        p[i].number_sectors = *((__uint32_t*)(content + partition_index + next_value));
        //get_real_address(&p[i].number_sectors, sizeof(p[i].number_sectors));
        printf("\nVALUE NUMBER_SECTORS: %x", p[i].number_sectors);

        printf("\n\n===================END PARTITION=======================");

        partition_index += sizeof(Partition);
        total_sectors += p[i].number_sectors;
    }

    __uint64_t total_bytes = TOTAL_SIZE * total_sectors;
    double size_gb = (double)total_bytes / pow(1024, 3);

    printf("\n\nDisco /dev/sda: %.2f GiB, %lu bytes, %u setores", size_gb, total_bytes, total_sectors);
    printf("\nModelo de disco: VBO HARDDISK");
    printf("\nUnidades: setor de 1 * %d = %d bytes", TOTAL_SIZE, TOTAL_SIZE);
    printf("\nTamanho do setor (lógico/físico): %d bytes / %d bytes", TOTAL_SIZE, TOTAL_SIZE);
    printf("\nTamanho E/S (mínimo/ótimo): %d bytes / %d bytes", TOTAL_SIZE, TOTAL_SIZE);
    printf("\nTipo de rótulo do disco: dos");
    printf("\nIdentificador do disco: 0x%8x", disk_id);

    printf("\n\nDispositivo  Inicializar  Início   Fim       Setores   Tamanho     Id   Tipo");
    for(int i = 0; i < QUANTITY_OF_PARTITIONS; i++){
        if(p[i].partition_type != 0){

            char bootable = p[i].status == 0x80 ? '*' : ' ';
            __uint32_t lba_end = p[i].lba_sector + p[i].number_sectors;
            double size = ((double)p[i].number_sectors * TOTAL_SIZE / pow(1024, 2)); 

            printf("\n/dev/sda%d    %c          %8u  %8u   %8u  %3.1fG     %2x   %s",
                i + 1,
                bootable,
                p[i].lba_sector,
                lba_end,
                p[i].number_sectors,
                size,
                p[i].partition_type,
                get_partition_type(p[i].partition_type)
                );
        }
    }

    free(content);
    free(p);

    return 0;
}