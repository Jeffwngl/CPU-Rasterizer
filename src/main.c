
#include <stdio.h>

#include "Application.h"

#define WIDTH 800
#define HEIGHT 600

int main(int argc, char **argv)
{
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <model-path.obj>\n", argv[0]);
        return 1;
    }

    Application application;

    if (!application_create(
            &application,
            argv[1],
            WIDTH,
            HEIGHT)) {
        return 1;
    }

    application_run(&application);
    application_destroy(&application);

    return 0;
}
