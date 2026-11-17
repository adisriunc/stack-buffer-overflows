#include <stdio.h>
#include <string.h>

static void greet(const char *input) {
    char name[8];

    strcpy(name, input);
    printf("Hello, %s!\n", name);
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: %s NAME\n", argv[0]);
        return 1;
    }

    greet(argv[1]);
    return 0;
}
