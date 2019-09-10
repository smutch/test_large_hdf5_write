Testing for a large (~2048^3) PHDF5 write.


```text
Usage:

    mpirun -n 2 ./test_large_hdf5_write [-c] [-i] [-N <dim>]


Options:

    -c
        Create a chunked dataset (default is no-chunking).
    
    -i
        Use independent MPIO data transfer flags for H5Dwrite (default is collective).

    -N <dim>
        Dimensionality of the grid to be written (e.g. -N128 is a 128^3 grid).

```
