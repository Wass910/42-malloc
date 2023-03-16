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

typedef struct s_history
{
    void *addr;
    struct s_history *next;
    char *str;
} t_history;

typedef struct s_zones
{
    t_zone *tiny;
    t_zone *small;
    t_zone *large;
    size_t size;
    t_history *history;
} t_zones;

void    free(void *ptr);
void    *malloc(size_t size);
void    *realloc(void *ptr, size_t size);
void    ft_lstadd_back(t_block **alst, t_block *new);
void	ft_lstdelone(t_block *block);
t_block *new_block(size_t size);
void    show_alloc_mem();
void    show_alloc_mem_ex();
void    *allocate_zone();
t_history *new_history(char str[100], void *ptr);
void	ft_lstadd_back_history(t_history **alst, t_history *new);

extern t_zones g_zones;

#endif