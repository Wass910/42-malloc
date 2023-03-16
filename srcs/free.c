#include "../inc/libft_malloc.h"

void free(void *ptr)
{
    if (ptr == NULL)
        return;
    t_zone zone_small = *g_zones.small;
    while (zone_small.blocks != NULL)
    {
        if (zone_small.blocks->addr == ptr)
        {
            if (zone_small.blocks->free == 1)
                g_zones.size = g_zones.size - zone_small.blocks->size;
            zone_small.blocks->free = 0;
            zone_small.blocks->size = 0;
            if (munmap(ptr, sizeof(ptr)) == -1) {
                perror("munmap");
                return ;
            }
            return;
        }
        zone_small.blocks = zone_small.blocks->next;
    }
    t_zone zone_tiny = *g_zones.tiny;
    while (zone_tiny.blocks != NULL)
    {
        if (zone_tiny.blocks->addr == ptr)
        {
            if (zone_tiny.blocks->free == 1)
                g_zones.size = g_zones.size - zone_tiny.blocks->size;
            zone_tiny.blocks->free = 0;
            zone_tiny.blocks->size = 0;
            if (munmap(ptr, sizeof(ptr)) == -1) {
                perror("munmap");
                return ;
            }
            return;
        }
        zone_tiny.blocks = zone_tiny.blocks->next;
    }
    t_zone zone_large = *g_zones.large;
    while (zone_large.blocks != NULL)
    {
        if (zone_large.blocks->addr == ptr)
        {
            g_zones.size = g_zones.size - zone_large.blocks->size;
            zone_large.blocks->free = 0;
            if (munmap(ptr, sizeof(ptr)) == -1) {
                perror("munmap");
                return ;
            }
            ft_lstdelone(zone_large.blocks);
            return;
        }
        zone_large.blocks = zone_large.blocks->next;
    }
    return ;
}