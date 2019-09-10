#include <stdlib.h>
#include <assert.h>
#include "save.h"

int main(int argc, char *argv[])
{
    int i_rank, n_rank;
    MPI_Init(&argc, &argv);

    assert(argc == 3);
    save(atoi(argv[1]), atoi(argv[2]));

    MPI_Finalize();
    return 0;
}
