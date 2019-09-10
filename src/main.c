#include <stdlib.h>
#include <assert.h>
#include "save.h"

int main(int argc, char *argv[])
{
    int i_rank, n_rank;
    MPI_Init(&argc, &argv);

    assert(argc == 2);
    save(atoi(argv[1]));

    MPI_Finalize();
    return 0;
}
