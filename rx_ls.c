#include <stdio.h>
#include <stdbool.h>
#include <dirent.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#define FORMAT_STR "la"

struct cmd_options
{
    const char **m_paths;
    size_t m_path_count;
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

    size_t path_count = argc - optind;
    options.m_paths = malloc(sizeof(const char*) * path_count);
    options.m_path_count = path_count;
    // The rest are directories or files
    for (size_t i = 0; i < path_count; ++i)
    {
        options.m_paths[i] = argv[optind++];
    }
    return options;
}

// fd, file_type - out variables
bool exists(const char *path, int *fd, mode_t *file_type)
{
    *fd = open(path, O_RDONLY);
   
    struct stat buf = {0};
    if(fstat(*fd, &buf) == -1)
    {
        return false;
    }

    *file_type = buf.st_mode;

    return true;
}

int pstrcmp( const void *a, const void *b )
{
    return strcmp( *(const char**)a, *(const char**)b );
}

void sort_strings(const char **arr, size_t size)
{
    qsort(arr, size, sizeof(const char*), pstrcmp);
}

int main(int argc, char *argv[])
{
    struct cmd_options options = parse_cmd_options(argc, argv);

    sort_strings(options.m_paths, options.m_path_count);
    for(size_t i = 0; i < options.m_path_count; ++i)
    {
        int fd;
        mode_t file_type;

        if(!exists(options.m_paths[i], &fd, &file_type))
        {
            fprintf(stderr,
                    "rx_ls: cannot access '%s': No such file or directory\n",
                    options.m_paths[i]);
            continue;
        }

        printf("All good! %s\n", options.m_paths[i]);
    }
    free(options.m_paths);

    return 0;
}

