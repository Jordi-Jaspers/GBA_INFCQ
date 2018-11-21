#include <stdio.h>
#include <stdlib.h>

typedef unsigned char  uint8;
typedef unsigned short uint16;
typedef unsigned int   uint32;

typedef uint32 tile_4bpp[8];        // 8 rijen, elk 1 bit
typedef tile_4bpp tile_block[512];  // tile block = 8 screen blocks, 512 tiles 
typedef uint16 palette[16];         // 16 palettes beschikbaar

typedef struct object {
    uint16 attr0;
    uint16 attr1;
    uint16 attr2;
    uint16 unused;
} __attribute__((packed, aligned(4))) object;

typedef struct sprite {
    int x;  
    int y;
    int dx; 
    int dy;
    uint8 width;  
    uint8 height;
    volatile object *obj;
} sprite;