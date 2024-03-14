#include <stdio.h>

extern int i;

int main(int argc, char **argv)
{
    (void) argc; (void) argv;
    printf("Hello, World! (%d)\n", i);
}
