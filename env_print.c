#include <stdio.h>

int main(int argc, char* argv[], char* env[]) {
    int i;
    char** env_ptr = env;

    printf("Arguments:\n");
    for(i = 0; i < argc; i++)
        printf("\t%s\n", argv[i]);

    printf("\nEnvironment:\n");
    for(env_ptr = env; *env_ptr != NULL; env_ptr++)
        printf("\t%s\n", *env_ptr);
}
