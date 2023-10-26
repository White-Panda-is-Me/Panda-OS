#pragma once
#include "stdbool.h"
#include "ata.h"
#include <stdint.h>

#define PACKED                     __attribute__((packed))

#define BLOCK_SIZE                 512
#define INODE_COUNT                512
#define MAX_ROOT_DIR_ENTRIES       128
#define MLFS_FLAG_READABLE         0x01
#define MLFS_FLAG_WRITABLE         0x02
#define MLFS_FLAG_EXECUTABALE      0x04
#define MLFS_FLAG_HIDDEN           0x08
#define MLFS_FLAG_FOLDER           0x10

#define MLFS_FLAG_UNUSED           0x00

typedef struct {
    uint8_t  JumpInstruction[3];
    uint16_t mlfs_bps;
    uint16_t mlfs_total_blocks;
    char  mlfs_magic_word[4];
    uint8_t  mlfs_max_root_dir_entries;
    uint8_t  mlfs_reserved_blocks;
    uint8_t  mlfs_spt;
    uint8_t  mlfs_heads;
    uint16_t mlfs_cylinders;
} PACKED MLFS_Header;

typedef struct {
    uint16_t mlfs_bps;
    uint16_t mlfs_total_blocks;
    uint8_t  mlfs_magic_word[4];
    uint8_t  mlfs_max_root_dir_entries;
    uint8_t  mlfs_reserved_blocks;
    uint8_t  mlfs_spt;
    uint8_t  mlfs_heads;
    uint16_t mlfs_cylinders;
} PACKED SuperBlock;

typedef struct {
    uint32_t StartBlockEntries[MAX_ROOT_DIR_ENTRIES]; // 128 max entries, each have 4 bytes wide for inode_num
} PACKED Root_Entries;

typedef struct {
    uint32_t inode_num;
    uint8_t flags;
    uint16_t CreatedDate;
    uint16_t CreatedTime;
} PACKED INode;

typedef struct {
    char name[120];
    uint32_t blockStart;
    uint32_t fileSize;
    uint32_t fileSizeInSector;
} PACKED File;

typedef struct {
    char name[123];
    uint32_t entries[95];
} PACKED Folder;

typedef struct {
    INode inode;
    union {
        File file;
        Folder folder;
    } Info;
} PACKED Entry;

char* Path2Name(char* dir);
void MLFS_init();
Entry* OpenFile(char* dirStr);
void ReadFile(Entry* entry, void* buffer);
void CloseFile(Entry* entry);