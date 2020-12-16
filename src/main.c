#include "save.h"
#include <assert.h>
#include <ctype.h>
#include <stdlib.h>
#include <unistd.h>

#define ABORT(sigterm)                                                                            \
    do {                                                                                          \
        fprintf(stderr, "\nIn file: %s\tfunc: %s\tline: %i\n", __FILE__, __FUNCTION__, __LINE__); \
        MPI_Finalize();                                                                           \
        return sigterm;                                                                           \
    } while (0)

int main(int argc, char* argv[])
{
    MPI_Init(&argc, &argv);
    int i_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &i_rank);

    // default options
    long N = 128;
    bool flag_chunked = false;
    bool flag_independent = false;

    // parse the command line
    {
        int c;
        opterr = 0;
        while ((c = getopt(argc, argv, "N:ci")) != -1)
            switch (c) {
            case 'c':
                flag_chunked = true;
                break;
            case 'i':
                flag_independent = true;
                break;
            case 'N':
                N = atol(optarg);
                break;
            case '?':
                if (optopt == 'N')
                    fprintf(stderr, "Option -%c requires an argument.\n", optopt);
                else if (isprint(optopt))
                    fprintf(stderr, "Unknown option `-%c'.\n", optopt);
                else
                    fprintf(stderr, "Unknown option character `\\x%x'.\n", optopt);
                ABORT(1);
            default:
                ABORT(1);
            }

        if (optind != argc) {
            fprintf(stderr, "Unknown extra arguments!...\n");
            ABORT(1);
        }

        if (i_rank == 0)
            printf("N=%ld, flag_chunked=%d, flag_independent=%d\n", N, (int)flag_chunked, (int)flag_independent);
    }

    save(N, flag_chunked, flag_independent);

    MPI_Finalize();
    return 0;
}
