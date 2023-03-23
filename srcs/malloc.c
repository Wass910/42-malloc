#include "../inc/libft_malloc.h"

t_zones g_zones = {NULL, NULL, NULL, 0, NULL};



void *tiny_malloc(size_t size)
{
    t_zone zone = *g_zones.tiny;
    while (zone.blocks != NULL)
    {
        if (zone.blocks->free == 0)
        {
            zone.blocks->addr = mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANON, -1, 0);
            if (zone.blocks->addr == MAP_FAILED) {
                write(1, "mmap tiny malloc failed\n", 24);
                return NULL;
            }
            zone.blocks->free = 1;
            zone.blocks->size = size;
            ft_lstadd_back_history(&g_zones.history, new_history("New tiny malloc called by : ", zone.blocks));
            return zone.blocks->addr;
        }
        zone.blocks = zone.blocks->next;
    }
    write(1, "no More place for allocation\n", 29);
    return NULL;
}

void *small_malloc(size_t size)
{
    t_zone zone = *g_zones.small;
    while (zone.blocks != NULL)
    {
        if (zone.blocks->free == 0)
        {
            zone.blocks->addr = mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANON, -1, 0);
            if (zone.blocks->addr == MAP_FAILED) {
                write(1, "mmap small malloc failed\n", 25);
                return  NULL;
            }
            zone.blocks->free = 1;
            zone.blocks->size = size;
            ft_lstadd_back_history(&g_zones.history, new_history("New small malloc called by : ", zone.blocks));
            return zone.blocks->addr;
        }
        zone.blocks = zone.blocks->next;
    }
    write(1, "no More place for allocation\n", 29);
    return NULL;
}

void *large_malloc(size_t size)
{
    if (g_zones.large == NULL){
        g_zones.large = mmap(NULL, size  , PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANON, -1, 0);
        if (g_zones.large == MAP_FAILED){
            write(1, "mmap large malloc failed\n", 25);
            g_zones.large = NULL;
            return NULL;
        }
    }
    else{
        t_zone zone = *g_zones.large;
        g_zones.large = mmap(NULL, size + g_zones.large->size , PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANON, -1, 0);
        //write(1, "aaak\n", 5);
        if (g_zones.large == MAP_FAILED){
            write(1, "mmap large1 malloc failed\n", 26);
            return NULL;
        }
        g_zones.large->blocks = zone.blocks;
    }
    ft_lstadd_back(&g_zones.large->blocks, new_block(size));
    t_zone zone = *g_zones.large;
    while (zone.blocks != NULL)
    {
        //write(1, "okok\n", 5);
        if (zone.blocks->free == 0)
        {
            zone.blocks->addr = mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANON, -1, 0);
            if (zone.blocks->addr == MAP_FAILED) {
                write(1, "mmap large2 malloc failed\n", 26);
                return NULL;
            }
            zone.blocks->free = 1;
            zone.blocks->size = size;
            ft_lstadd_back_history(&g_zones.history, new_history("New large malloc called by : ", zone.blocks));
            return zone.blocks->addr;
        }
        zone.blocks = zone.blocks->next;
    }
    write(1, "no More place for allocation\n", 29);
    return NULL;
}

void *malloc(size_t size)
{
    if (g_zones.tiny == NULL && g_zones.small == NULL)
    {
        size_t tiny_init_pages = (128 * TINY / getpagesize());
        size_t small_init_pages = (128 * SMALL / getpagesize());
        g_zones.tiny = mmap(NULL, tiny_init_pages, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANON, -1, 0);
        if(g_zones.tiny == MAP_FAILED){
            write(1, "tiny zone malloc fail \n", 23);
            g_zones.tiny = NULL;
            return NULL;
        }
        g_zones.small = mmap(NULL, small_init_pages, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANON, -1, 0);
        if (g_zones.small == MAP_FAILED){
            write(1, "small zone malloc failed \n", 26);
            g_zones.small = NULL;
            return NULL;
        }
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



