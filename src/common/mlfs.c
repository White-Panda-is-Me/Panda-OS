#include "mlfs.h"
#include "string.h"
#include "stdio.h"

Root_Entries g_entries;
MLFS_Header g_BootHeader;

bool FindEntry(char* name, Entry* entry) {
    if(strncmp(g_BootHeader.mlfs_magic_word, "SFLM", 4) != 0) {
        printf("MLFS has not been initialized!\n");
        return false;
    }
    for (int i = 0;i < 5;i++) {
        ata_lba_read(g_entries.StartBlockEntries[i], 1, entry);
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

void MLFS_init() {
    ata_lba_read(0, 1, &g_BootHeader);
    ata_lba_read(1, 1, &g_entries);
}

Entry* OpenFile(char* dirStr) {
    char* dir = &dirStr[0];
    static Entry entry;
    const uint32_t dir_base = (uint32_t) dir;
    int itemindexes[2];
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
            FindEntry(substr((char*) dir_base, itemindexes[0], itemindexes[1]), &entry);
            itemindexes[1] += 2;
            itemindexes[0] = itemindexes[1];
        } else if(*dir == '\0') {
            itemindexes[1] = i;
            if(FindEntry(substr((char*) dir_base, itemindexes[0], itemindexes[1]), &entry)) {
                return &entry;
            }
        }
        dir++;
    }
    return (void*) 0;
}

void ReadFile(Entry* entry, void* buffer) {
    if((entry->inode.flags & MLFS_FLAG_READABLE) == 0) {
        printf("file %s is not readable!\n", entry->Info.file.name);
        return;
    }
    ata_lba_read(entry->Info.file.blockStart, entry->Info.file.fileSizeInSector, buffer);
}

void CloseFile(Entry* entry) {
    entry = (void*) 0;
}