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

void    free(void *ptr);
void    *malloc(size_t size);
void    *realloc(void *ptr, size_t size);
void    ft_lstadd_back(t_block **alst, t_block *new);
void	ft_lstdelone(t_block *block);
t_block *new_block(size_t size);
void    show_alloc_mem();
void    *allocate_zone();

extern t_zones g_zones;

#endif