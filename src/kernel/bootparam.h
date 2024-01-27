#pragma once

typedef struct {
    unsigned int    *framebuffer;
    unsigned int    width;
    unsigned int    height;
    unsigned int    pitch;
    int             argc;
    char            **argv;
} bootparam_t;

