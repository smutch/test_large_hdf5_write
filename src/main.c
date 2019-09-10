#include <stdlib.h>
#include <assert.h>
#include "save.h"

int main(int argc, char *argv[])
{
    assert(argc == 2);
    save(atoi(argv[1]));
    return 0;
}
