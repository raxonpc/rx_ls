#include <stdio.h>
#include <stdbool.h>
#include <dirent.h>
#include <unistd.h>
#include <stdint.h>
#include <stdlib.h>

#define FORMAT_STR "la"

struct dir_list
{
    struct dir_list *m_next;
    const char *m_name;
};

struct cmd_options
{
    struct dir_list *m_head;
    uint32_t m_l:1;     // -l flag
    uint32_t m_a:1;     // -a flag
};

struct cmd_options parse_cmd_options(int argc, char *argv[])
{
    struct cmd_options options = {0};

    int errflg = 0;
    int c;
    while((c = getopt(argc, argv, FORMAT_STR)) != -1)
    {
        switch(c)
        {
        case 'l':
            options.m_l = 1;
            break;
        case 'a':
            options.m_a = 1;
            break;
        case '?':
            ++errflg;
        }
    }

    if(errflg > 0)
    {
        fprintf(stderr,
                "Don't try 'rx_ls --help' for more information. It is not implemented :(\n");
        return options;
    }

    struct dir_list **it = &options.m_head;
    
    // The rest are directories or files
    for ( ; optind < argc; optind++)
    {
        *it = malloc(sizeof(struct dir_list));
        (*it)->m_name = argv[optind];
        (*it)->m_next = NULL;
        it = &(*it)->m_next;
    }
    return options;
}

int main(int argc, char *argv[])
{
    /*const char* dir_name = 
        (argc > 1) ? argv[1] : ".";

    DIR *dir = opendir(dir_name);
    if(dir == NULL)
    {
        fprintf(stderr,
                "cannot access '%s': No such directory\n", dir_name);
        return 1;
    }

    struct dirent *directory;
    while((directory = readdir(dir)) != NULL)
    {
        printf("%s ", directory->d_name);
    }
    putchar('\n');
    int temp_close = closedir(dir);
    if(temp_close == -1)
    {
        fprintf(stderr,
        "Error closing directory %s\n", dir_name);
        return 1;
    } */

    struct cmd_options options = parse_cmd_options(argc, argv);

    return 0;
}

