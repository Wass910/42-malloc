#include "malloc.h"

t_zones g_zones = {NULL, NULL, NULL, 0};

void	ft_lstadd_back(t_block **alst, t_block *new)
{
	t_block	*lst;

	lst = *alst;
	if (*alst == NULL)
		*alst = new;
	else
	{
		while (lst->next)
			lst = lst->next;
		lst->next = new;
	}
}

t_block *new_block(size_t size)
{
    t_block *block = mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    if (block == MAP_FAILED) {
        perror("mmap");
        return NULL;
    }
    block->next = NULL;
    block->free = 0;
    block->size = size;
    return block;
}

void show_alloc_mem()
{
    if (g_zones.tiny != NULL && g_zones.small != NULL)
    {
        t_zone zone = *g_zones.tiny;
        printf("TINY ZONE : %p\n", zone.blocks);
        while (zone.blocks != NULL)
        {
            if (zone.blocks->free == 1 && zone.blocks->next != NULL)
                printf("%p - %p = %zu bytes\n", zone.blocks, zone.blocks->next, zone.blocks->size);
            else if (zone.blocks->free == 1 && zone.blocks->next == NULL)
                printf("%p - NULL = %zu bytes\n", zone.blocks, zone.blocks->size);
            zone.blocks = zone.blocks->next;
        }

        t_zone zone2 = *g_zones.small;
        printf("SMALL ZONE : %p\n", zone2.blocks);
        while (zone2.blocks != NULL)
        {
            if (zone2.blocks->free == 1 && zone2.blocks->next != NULL)
                printf("%p - %p = %zu bytes\n", zone2.blocks, zone2.blocks->next, zone2.blocks->size);
            else if (zone2.blocks->free == 1 && zone2.blocks->next == NULL)
                printf("%p - NULL = %zu bytes \n", zone2.blocks, zone2.blocks->size);
            zone2.blocks = zone2.blocks->next;
        }
    }
    if (g_zones.large != NULL)
    {
        t_zone zone3 = *g_zones.large;
        printf("LARGE ZONE : %p\n", zone3.blocks);
        while (zone3.blocks != NULL)
        {
            if (zone3.blocks->free == 1 && zone3.blocks->next != NULL)
                printf("%p - %p = %zu bytes\n", zone3.blocks, zone3.blocks->next, zone3.blocks->size);
            else if (zone3.blocks->free == 1 && zone3.blocks->next == NULL)
                printf("%p - NULL = %zu bytes\n", zone3.blocks, zone3.blocks->size);
            zone3.blocks = zone3.blocks->next;
        }
    }
    printf("Total : %zu bytes\n", g_zones.size);
    printf("----------------------\n");
}

void *allocate_zone()
{
    int i = 0;

    while(i < 100)
    {
        ft_lstadd_back(&g_zones.tiny->blocks, new_block(TINY));
        i++;
    }
    i = 0;
    while(i < 100)
    {
        ft_lstadd_back(&g_zones.small->blocks, new_block(SMALL));
        i++;
    }
    return NULL;
}

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

void	ft_lstdelone(t_block *block)
{    
    t_zone zone_large = *g_zones.large;
    while (zone_large.blocks->next != NULL)
    {
        if (zone_large.blocks->next == block)
        {
            if (zone_large.blocks->next->next != NULL)
                zone_large.blocks->next = zone_large.blocks->next->next;
            else
                zone_large.blocks->next = NULL;
            if (munmap(block, sizeof(block)) == -1) {
                perror("munmap");
                return ;
            }
            return;
        }
        zone_large.blocks = zone_large.blocks->next;
    }
}

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

int main() {
    void *str = malloc(20);
    char *str2 = malloc(2000);
    void *str4 = malloc(10000);
    char *str3 = malloc(64366);
    show_alloc_mem();
    str3 = realloc(str3, 100);
    char *str5 = malloc(259);
    str = realloc(str4, 3);
    show_alloc_mem();
    free(str);
    show_alloc_mem();
    str = realloc(str, 111);
    show_alloc_mem();
    return 0;
}