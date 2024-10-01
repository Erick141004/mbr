#ifndef MBR_H
#define MBR_H

#define QUANTITY_OF_PARTITIONS 4
#define TOTAL_SIZE 512
#define PARTITION_SIZE 16
#define START_FIRST_PARTITION 446
#define SIGNATURE 510
#define DISK_ID 440

typedef struct{
    __uint8_t status;
    __uint8_t chs_start[3];
    __uint8_t partition_type;
    __uint8_t chs_end[3];
    __uint32_t lba_sector;
    __uint32_t number_sectors;
} Partition;

#endif