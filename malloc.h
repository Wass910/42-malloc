#ifndef LIBFT_MALLOC_H
#define LIBFT_MALLOC_H

#include <sys/mman.h>
#include <unistd.h>
#include <stdio.h>

#define TINY 128
#define SMALL 512

typedef struct s_block
{
    size_t size;
    void *addr;
    struct s_block *next;
    int free;
} t_block;

typedef struct s_zone
{
    size_t size;
    struct s_zone *next;
    t_block *blocks;
} t_zone;

typedef struct s_zones
{
    t_zone *tiny;
    t_zone *small;
    t_zone *large;
    size_t size;
} t_zones;



#endif