;============================================================================;
; ATA read sectors (CHS mode)                                                ;
; Max head index is 15, giving 16 possible heads                             ;
; Max cylinder index can be a very large number (up to 65535)                ;
; Sector is usually always 1-63, sector 0 reserved, max 255 sectors/track    ;
; If using 63 sectors/track, max disk size = 31.5GB                          ;
; If using 255 sectors/track, max disk size = 127.5GB                        ;
;                                                                            ;
; @param EBX The CHS values; 2 bytes, 1 byte (BH), 1 byte (BL) accordingly   ;
; @param CH The number of sectors to read                                    ;
; @param RDI The address of buffer to put data obtained from disk            ;  
;                                                                            ;
; @return None                                                               ;
;============================================================================;

global chs_ata_read
chs_ata_read:
