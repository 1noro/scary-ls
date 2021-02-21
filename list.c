#include <stdio.h>
// #include <stdlib.h>
#include <dirent.h>

void print_argv(int argc, char const *argv[]) {
    printf("argc: %d\n", argc);
    printf("argv: [");
    for (size_t i = 0; i < argc; i++) {
        printf("'%s'", argv[i]);
        if (i != argc - 1) {
            printf(", ");
        }
    }
    printf("]\n");
}

int list_files(const char *dirname) {
    struct dirent *pDirent;
    DIR *pDir;

    // Ensure we can open directory.
    pDir = opendir(dirname);
    if (pDir == NULL) {
        printf ("Cannot open directory '%s'\n", dirname);
        return 1;
    }

    // Process each entry.
    while ((pDirent = readdir(pDir)) != NULL) {
        printf ("%s/%s\n", dirname, pDirent->d_name);
    }

    // Close directory and exit.
    closedir(pDir);
    return 0;
}

int main(int argc, char const *argv[]) {
    // print_argv(argc, argv);

    // Si hay argumentos abro todos los directorios, sino solo el actual
    if (argc > 1) {
        for (int i = 1; i < argc; i++) {
            list_files(argv[i]);
        }
    } else {
        char current_dir[] = ".";
        list_files(current_dir);
    }

    return 0;
}
