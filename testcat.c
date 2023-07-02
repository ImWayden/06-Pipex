#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void cat_file(char *filename) {
    char *args[] = {"cat", "/dev/stdin", NULL};
    execve("/bin/cat", args, NULL);
    perror("Erreur lors de l'exécution de cat");
    exit(EXIT_FAILURE);
}

int main() {
    char *filename = "infile";
    cat_file(filename);

    return 0;
}