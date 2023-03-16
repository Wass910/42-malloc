#include "../inc/libft_malloc.h"

void *find_address(void *ptr)
{
    if (ptr == NULL)
        return NULL;
    t_zone zone_small = *g_zones.small;
    while (zone_small.blocks != NULL)
    {
        if (zone_small.blocks->addr == ptr)
            return zone_small.blocks->addr;
        zone_small.blocks = zone_small.blocks->next;
    }
    t_zone zone_tiny = *g_zones.tiny;
    while (zone_tiny.blocks != NULL)
    {
        if (zone_tiny.blocks->addr == ptr)
            return zone_tiny.blocks->addr;
        zone_tiny.blocks = zone_tiny.blocks->next;
    }
    t_zone zone_large = *g_zones.large;
    while (zone_large.blocks != NULL)
    {
        if (zone_large.blocks->addr == ptr)
            return zone_large.blocks->addr;
        zone_large.blocks = zone_large.blocks->next;
    }
    return NULL;
}

void *bigger_tiny(t_block *zone,size_t size)
{
    if (zone->free == 1)
        g_zones.size = g_zones.size - zone->size;
    free(zone->addr);
    return (malloc(size));       
}

void *not_small(t_block *zone,size_t size)
{
    if (zone->free == 1)
        g_zones.size = g_zones.size - zone->size;
    free(zone->addr);
    return (malloc(size));       
}

void *smaller_large(t_block *zone,size_t size)
{
    if (zone->free == 1)
        g_zones.size = g_zones.size - zone->size;
    ft_lstdelone(zone);
    return (malloc(size));       
}

void *new_allocation(void *ptr, size_t size)
{
    t_zone zone_tiny = *g_zones.tiny;
    while (zone_tiny.blocks != NULL)
    {
        if (zone_tiny.blocks->addr == ptr){
            if (size <= TINY)
            {
                zone_tiny.blocks->free = 1;
                g_zones.size = g_zones.size - zone_tiny.blocks->size;
                zone_tiny.blocks->size = size;
                g_zones.size = g_zones.size + zone_tiny.blocks->size;
                return zone_tiny.blocks->addr;
            }
            else
                return bigger_tiny(zone_tiny.blocks, size);
        }
        zone_tiny.blocks = zone_tiny.blocks->next;
    }
    t_zone zone_small = *g_zones.small;
    while (zone_small.blocks != NULL)
    {
        if (zone_small.blocks->addr == ptr){
            if (size <= SMALL)
            {
                zone_small.blocks->free = 1;
                g_zones.size = g_zones.size - zone_small.blocks->size;
                zone_small.blocks->size = size;
                g_zones.size = g_zones.size + zone_small.blocks->size;
                return zone_small.blocks->addr;
            }
            else
                return not_small(zone_tiny.blocks, size);
        }
        zone_small.blocks = zone_small.blocks->next;
    }
    t_zone zone_large = *g_zones.large;
    while (zone_large.blocks != NULL)
    {
        if (zone_large.blocks->addr == ptr){
            if (size >= SMALL){
                zone_large.blocks->free = 1;
                g_zones.size = g_zones.size - zone_large.blocks->size;
                zone_large.blocks->size = size;
                g_zones.size = g_zones.size + zone_large.blocks->size;
                return zone_large.blocks->addr;
            }
            return smaller_large(zone_large.blocks, size);
        }
        zone_large.blocks = zone_large.blocks->next;
    }
    return NULL;
}

void *realloc(void *ptr, size_t size)
{
    void *new_ptr = find_address(ptr);
    if (new_ptr == NULL){
        return malloc(size);
    }
    return (new_allocation(ptr, size));
}