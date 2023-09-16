#include <cpuid.h>
#include "cpu.h"

int edx_get_features() {
    unsigned int edx, unused;
    __get_cpuid(1, &unused, &unused, &unused, &edx);
    return edx;
}

int ecx_get_features() {
    unsigned int ebx, unused;
    __get_cpuid(1, &unused, &ebx, &unused, &unused);
    return ebx;
}