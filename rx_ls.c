#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <dirent.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <strings.h>

#define SUPPORTED_FLAGS "la"
#define INVALID_FILE_DESCRIPTOR -1

struct flags
{
    uint32_t m_a:1;     // -a flag
    uint32_t m_l:1;     // -l flag
};

struct cmd_options
{
    const char **m_paths;
    size_t m_path_count;
    struct flags m_flags;
};

struct cmd_options parse_cmd_options(int argc, char *argv[])
{
    struct cmd_options options = {0};

    int errflg = 0;
    int c;
    while((c = getopt(argc, argv, SUPPORTED_FLAGS)) != -1)
    {
        switch(c)
        {
        case 'l':
            options.m_flags.m_l = 1;
            break;
        case 'a':
            options.m_flags.m_a = 1;
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

// lexicographical comparison
int pstrcmp( const void *a, const void *b )
{
    const char *str_a = *(const char**)a;
    const char *str_b = *(const char**)b;

    return strcasecmp(str_a, str_b);
}

void sort_strings(const char **arr, size_t size)
{
    qsort(arr, size, sizeof(const char*), pstrcmp);
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

struct fd_arr
{
    const char **m_names;
    int *m_arr;
    size_t m_size;
};

struct fd_arr init_fd_arr(size_t init_size)
{
    struct fd_arr output;

    output.m_names = malloc(sizeof(const char*) * init_size);
    output.m_arr = malloc(sizeof(int) * init_size);
    for(size_t i = 0; i < init_size; ++i)
    {
        output.m_names[i] = NULL;
        output.m_arr[i] = INVALID_FILE_DESCRIPTOR;
    }

    output.m_size = 0;

    return output;
}

void fd_arr_push(struct fd_arr *fd_arr, const char *file_name, int fd)
{
    fd_arr->m_names[fd_arr->m_size] = file_name;
    fd_arr->m_arr[fd_arr->m_size++] = fd;
}

void fd_arr_free(struct fd_arr *fd_arr)
{
    free(fd_arr->m_names);
    free(fd_arr->m_arr);
}

void list_files(const struct fd_arr *file_arr, const struct flags flags)
{
}


int main(int argc, char *argv[])
{
    struct cmd_options options = parse_cmd_options(argc, argv);


    sort_strings(options.m_paths, options.m_path_count);

    struct fd_arr file_arr = init_fd_arr(options.m_path_count);
    struct fd_arr dir_arr = init_fd_arr(options.m_path_count);

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

        if(file_type & S_IFDIR)
        {
            fd_arr_push(&dir_arr, options.m_paths[i], fd);
        }
        else
        {
            fd_arr_push(&file_arr, options.m_paths[i], fd);
        }
    }

    fd_arr_free(&dir_arr);
    fd_arr_free(&file_arr);

    free(options.m_paths);

    return 0;
}

