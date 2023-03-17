#include "../inc/libft_malloc.h"


int main() {
    // void *str = malloc(20);
    // char *str2 = malloc(2000);
    // void *str4 = malloc(10000);
    char *str3 = malloc(10);
    show_alloc_mem();
    // printf("\n*******************************\n\n");
    // str3 = realloc(str3, 100);
    // char *str5 = malloc(259);
    // str = realloc(str4, 3);
    // show_alloc_mem();
    // printf("\n*******************************\n\n");
    // free(str);
    // show_alloc_mem();
    // printf("\n*******************************\n\n");
    // str = realloc(str, 111);
    // show_alloc_mem();
    // printf("\n*******************************\n\n");
    // //show_alloc_mem_ex();
    // printf("\n*******************************\n\n");
    // free(str2);
    free(str3);
    // free(str4);
    // free(str5);
    // free(str);
    // show_alloc_mem();
    return 0;
}

// int main()
// {
//     char *test;

//     test = malloc(10);
//     test[0] = 'a';
//     test[1] = 'b';
//     test[2] = 'c';
//     test[3] = 'd';
//     test[4] = '\n';
//     test[5] = 0;

//     printf("test = %s\n", test);

//     char *test2;

//     test2 = malloc(10);
//     test2[0] = 'a';
//     test2[1] = 'b';
//     test2[2] = 'c';
//     test2[3] = 'd';
//     test2[4] = '\n';
//     test2[5] = 0;

//     printf("test2 = %s\n", test2);

//     show_alloc_mem();
//     free(test);
//     free(test2);

//     printf("*******************************\n");

//     int i = 0;
//     char *addr;

//     while (i < 1024)
//     {
//         addr = (char*)malloc(1024);
//         addr[0] = 42;
//         free(addr);
//         i++;
//     }

//     show_alloc_mem();

//     printf("*******************************\n");

//     char *tab[42];
//     for (int i = 0; i < 42; i++)
//     {
//         tab[i] = (char*)malloc(42);
//         tab[i][0] = 42;
//     }

//     show_alloc_mem();

//     printf("*******************************\n");

//     for (int i = 0; i < 42; i++)
//     {
//         free(tab[i]);
//     }

//     show_alloc_mem();
    
//     return (0);
// }