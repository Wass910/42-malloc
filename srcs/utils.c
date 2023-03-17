#include "../inc/libft_malloc.h"

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
    if (*alst != NULL)
    {
        if (lst->next == NULL && lst->size == 1024)
            lst->size = 0;
    }
}

void	ft_lstadd_back_history(t_history **alst, t_history *new)
{
	t_history	*lst;

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

t_history *new_history(char *str, void *ptr)
{
    t_history *history = mmap(NULL, sizeof(t_history), PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    if (history == MAP_FAILED) {
        perror("mmap");
        return NULL;
    }
    history->next = NULL;
    history->addr = ptr;
    history->str = str;
    return history;
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

t_block *new_block(size_t size)
{
    t_block *block = mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_PRIVATE, -1, 0);
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
        printf("TINY : %p\n", zone.blocks);
        while (zone.blocks != NULL)
        {
            if (zone.blocks->free == 1 && zone.blocks->next != NULL)
                printf("%p - %p = %zu bytes\n", zone.blocks, zone.blocks->next, zone.blocks->size);
            else if (zone.blocks->free == 1 && zone.blocks->next == NULL)
                printf("%p - NULL = %zu bytes\n", zone.blocks, zone.blocks->size);
            zone.blocks = zone.blocks->next;
        }

        t_zone zone2 = *g_zones.small;
        printf("SMALL : %p\n", zone2.blocks);
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
        printf("LARGE : %p\n", zone3.blocks);
        while (zone3.blocks->next != NULL)
        {
            if (zone3.blocks->free == 1 && zone3.blocks->next != NULL)
                printf("%p - %p = %zu bytes\n", zone3.blocks, zone3.blocks->next, zone3.blocks->size);
            else if (zone3.blocks->free == 1 && zone3.blocks->next == NULL)
                printf("%p - NULL = %zu bytes\n", zone3.blocks, zone3.blocks->size);
            zone3.blocks = zone3.blocks->next;
        }
    }
    printf("Total : %zu bytes\n", g_zones.size);
}

void show_alloc_mem_ex()
{
    if (g_zones.history != NULL)
    {
        t_history *history = g_zones.history;
        printf("History of allocation : \n\n");
        while (history->next != NULL)
        {
            printf("%s %p\n", history->str, history->addr);
            history = history->next;
        }
        printf("%s %p\n", history->str, history->addr);
    }
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