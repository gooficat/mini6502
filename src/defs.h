#pragma once

#include <stdint.h>

uint8_t typedef u8;
uint16_t typedef u16;
int8_t typedef i8;
int16_t typedef i16;

i16 typedef imax;
u16 typedef umax;

#define struc(t)        \
    struct t typedef t; \
    struct t
