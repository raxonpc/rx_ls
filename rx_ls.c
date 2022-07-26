#include <stdio.h>
#include <dirent.h>


int main(int argc, char *argv[])
{
    const char* dir_name = 
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
    }

    return 0;
}

