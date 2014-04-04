/*  'lsc' - List colored folders

    Usage:
        1. Create a label in a folder you wish to color:
           touch .label[red|green|yellow|blue|magenta|cyan|white]
           (ex. touch .labelred)
        2. Run lsc:
           lsc [-l] [dir]
           (ex. lsc -l)

    By Risto Stevcev    */

#include <stddef.h>
#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>
#include <string.h>

#define NORMAL  "\x1B[0m"
#define RED     "\x1B[91m"
#define GREEN   "\x1B[92m"
#define YELLOW  "\x1B[93m"
#define BLUE    "\x1B[94m"
#define MAGENTA "\x1B[95m"
#define CYAN    "\x1B[96m"
#define WHITE   "\x1B[97m"

#define LABEL_PREFIX ".label"

int 
ends_with (const char *str, const char *suffix)
{
    if (!str || !suffix)
        return 0;

    size_t lenstr = strlen(str);
    size_t lensuffix = strlen(suffix);
    
    if (lensuffix >  lenstr)
        return 0;
    
    return strncmp(str + lenstr - lensuffix, suffix, lensuffix) == 0;
}

char *
find_label(char *dirname)
{
    if (strcmp(dirname, ".") == 0 || strcmp(dirname, "..") == 0)
        return NORMAL;

    DIR *dp;
    struct dirent *ep;

    dp = opendir (dirname);
    if (dp != NULL) {
        while (ep = readdir (dp)) {
            if (strncmp(ep->d_name, LABEL_PREFIX, strlen(LABEL_PREFIX)) == 0) {
                if (ends_with(ep->d_name, "red"))
                    return RED;
                else if (ends_with(ep->d_name, "green"))
                    return GREEN;
                else if (ends_with(ep->d_name, "yellow"))
                    return YELLOW;
                else if (ends_with(ep->d_name, "blue"))
                    return BLUE;
                else if (ends_with(ep->d_name, "magenta"))
                    return MAGENTA;
                else if (ends_with(ep->d_name, "cyan"))
                    return CYAN;
                else if (ends_with(ep->d_name, "white"))
                    return WHITE;
            }
        }
        (void) closedir (dp);
    }
    return NORMAL;
}

int
main (int argc, char **argv)
{
    char *lookup_dir;
    int as_list = 0;

    if (argc == 2)
        if (strcmp(*(argv + 1), "-l") == 0) {
            as_list = 1;
            lookup_dir = ".";
        }
        else
            lookup_dir = *(argv + 1);
    else if (argc == 3 && strcmp(*(argv + 1), "-l") == 0) {
        as_list = 1;
        lookup_dir = *(argv + 2);
    }
    else
        lookup_dir = ".";

    DIR *dp;
    struct dirent *ep;

    dp = opendir (lookup_dir);
    if (dp != NULL) {
        while (ep = readdir (dp)) {
            if (ep->d_type == DT_DIR) {
                char current_path[strlen(lookup_dir) + strlen(ep->d_name) + 1];
                strcpy(current_path, lookup_dir);
                strcat(current_path, "/");
                strcat(current_path, ep->d_name);

                char *label = find_label(current_path);

                if (as_list)
                    printf("%s%s%s\n", label, ep->d_name, NORMAL);
                else
                    printf("%s%s%s | ", label, ep->d_name, NORMAL);
            }
        }
        if (!as_list)
            printf("\n");
        (void) closedir (dp);
    }
    else
        printf("%s\n", "Couldn't open the directory.");

    return 0;
}
