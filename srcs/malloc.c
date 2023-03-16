#include "../inc/libft_malloc.h"

t_zones g_zones = {NULL, NULL, NULL, 0};



void *tiny_malloc(size_t size)
{
    t_zone zone = *g_zones.tiny;
    while (zone.blocks != NULL)
    {
        if (zone.blocks->free == 0)
        {
            zone.blocks->addr = mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
            if (zone.blocks->addr == MAP_FAILED) {
                perror("mmap");
                return NULL;
            }
            zone.blocks->free = 1;
            zone.blocks->size = size;
            return zone.blocks->addr;
        }
        zone.blocks = zone.blocks->next;
    }
   
    return NULL;
}

void *small_malloc(size_t size)
{
    t_zone zone = *g_zones.small;
    while (zone.blocks != NULL)
    {
        if (zone.blocks->free == 0)
        {
            zone.blocks->addr = mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
            if (zone.blocks->addr == MAP_FAILED) {
                perror("mmap");
                return  NULL;
            }
            zone.blocks->free = 1;
            zone.blocks->size = size;
            return zone.blocks->addr;
        }
        zone.blocks = zone.blocks->next;
    }
    return NULL;
}

void *large_malloc(size_t size)
{
    if (g_zones.large == NULL)
        g_zones.large = mmap(NULL, size  , PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    else{
        t_zone zone = *g_zones.large;
        g_zones.large = mmap(NULL, size + g_zones.large->size , PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
        g_zones.large->blocks = zone.blocks;
    }
    ft_lstadd_back(&g_zones.large->blocks, new_block(size));
    t_zone zone = *g_zones.large;
    while (zone.blocks != NULL)
    {
        if (zone.blocks->free == 0)
        {
            zone.blocks->addr = mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
            if (zone.blocks->addr == MAP_FAILED) {
                perror("mmap");
                return NULL;
            }
            zone.blocks->free = 1;
            zone.blocks->size = size;
            return zone.blocks->addr;
        }
        zone.blocks = zone.blocks->next;
    }
    return NULL;
}

void *malloc(size_t size)
{
    if (g_zones.tiny == NULL && g_zones.small == NULL)
    {
        size_t tiny_init_pages = (128 * TINY / getpagesize());
        size_t small_init_pages = (128 * SMALL / getpagesize());
        g_zones.tiny = mmap(NULL, tiny_init_pages, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
        g_zones.small = mmap(NULL, small_init_pages, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
        allocate_zone();
    }
    g_zones.size = size + g_zones.size;
    if (size <= TINY)
        return tiny_malloc(size);
    else if (size <= SMALL)
        return small_malloc(size);
    else
        return large_malloc(size);
    return NULL;
}


