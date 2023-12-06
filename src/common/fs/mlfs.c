#include "mlfs.h"
#include "string.h"
#include <stdio.h>
#include <memory.h>

Root_Entries* g_entries = (Root_Entries*) 0x5500;
MLFS_Header* g_BootHeader = (MLFS_Header*) 0x5700;

bool FindEntry(char* name, Entry* entry, device* dev) {
    if(strncmp(g_BootHeader->mlfs_magic_word, "SFLM", 4) != 0) {
        return false;
    }
    for (int i = 0;i < 5;i++) {
        ata_read(g_entries->StartBlockEntries[i], 1, entry, dev);
        if(strcmp(entry->Info.file.name, name) == 0)
            return true;
    }
    return false;
}

char* Path2Name(char* dir) {
    int charIndex = -1;
    int i = 0;
    while (*dir) {
        if(*dir == '/')
            charIndex = i + 1;
        dir++;
        i++;
    }
    dir -= i;
    if(charIndex == -1)
        return dir;
    dir += charIndex;
    return dir;
}

int subdirnum(char* dir) {
    int num = 0;
    while(*dir) {
        if(*dir == '/')
            num++;
        dir++;
    }
    return num;
}

void MLFS_init(device* dev_p) {
    ata_init(dev_p);
    ata_read(0, 1, g_BootHeader, dev_p);
    ata_read(1, 1, g_entries, dev_p);
}

Entry* OpenFile(char* dirStr, device* dev) {
    char* dir = &dirStr[0];
    static Entry entry;
    const uint32_t dir_base = (uint32_t) dir;
    int itemindexes[2] = {0, 0};
    int foundFirst = 0;
    for (int i = 0;i < (124 * subdirnum(dirStr));i++) {
        if(*dir == '/') {
            if(foundFirst == 1) {
                itemindexes[1] = i - 1;
            } else {
                itemindexes[0] = i + 1;       // + 1 otherwise it would add / to the begining of the item 
                foundFirst = 1;
                dir++;
                continue;
            }
            FindEntry(substr((char*) dir_base, itemindexes[0], itemindexes[1]), &entry, dev);
            itemindexes[1] += 2;
            itemindexes[0] = itemindexes[1];
        } else if(*dir == '\0') {
            itemindexes[1] = i;
            if(FindEntry(substr((char*) dir_base, itemindexes[0], itemindexes[1]), &entry, dev)) {
                return &entry;
            }
        }
        dir++;
    }
    return (void*) 0;
}

void ReadFile(Entry* entry, void* buffer, device* dev) {
    if((entry->inode.flags & MLFS_FLAG_READABLE) == 0) {
        return;
    }
    // printf("blockStart: %u, fileSizeInSector: %u\n", entry->Info.file.blockStart, entry->Info.file.fileSizeInSector);
    // printf("%d", dev->drive);
    ata_read(entry->Info.file.blockStart, entry->Info.file.fileSizeInSector, buffer, dev);
    // printf("%u", entry->Info.file.fileSize);
}

void CloseFile(Entry* entry) {
    entry = (void*) 0;
}