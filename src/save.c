#include "save.h"

int save(const int N, const bool flag_chunked, const bool flag_independent)
{

    int i_rank, n_ranks;
    MPI_Comm_rank(MPI_COMM_WORLD, &i_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &n_ranks);

    const int local_size = (int)(N / n_ranks);
    assert(local_size * n_ranks == N);
    const int block_size = 1;

    // create the file (in parallel)
    hid_t plist_id = H5Pcreate(H5P_FILE_ACCESS);
    H5Pset_fapl_mpio(plist_id, MPI_COMM_WORLD, MPI_INFO_NULL);
    hid_t file_id = H5Fcreate("test_dump.h5", H5F_ACC_TRUNC, H5P_DEFAULT, plist_id);
    H5Pclose(plist_id);

    // create the filespace
    hsize_t dims[3] = { N, N, N };
    hid_t fspace_id = H5Screate_simple(3, dims, NULL);

    // create the memspace
    hsize_t mem_dims[3] = { block_size, N, N };
    hid_t memspace_id = H5Screate_simple(3, mem_dims, NULL);

    // create the dataset
    hid_t dcpl_id = H5P_DEFAULT;
    if (flag_chunked) {
        // set the dataset creation property list to use chunking along x-axis
        dcpl_id = H5Pcreate(H5P_DATASET_CREATE);
        H5Pset_chunk(dcpl_id, 3, (hsize_t[3]) { 1, N, N });
    }

    hid_t dset_id = H5Dcreate(file_id, "data", H5T_NATIVE_FLOAT, fspace_id,
        H5P_DEFAULT, dcpl_id, H5P_DEFAULT);

    if (flag_chunked) {
        H5Pclose(dcpl_id);
    }

    // create the property list
    plist_id = H5Pcreate(H5P_DATASET_XFER);
    if (flag_independent) {
        H5Pset_dxpl_mpio(plist_id, H5FD_MPIO_INDEPENDENT);
    } else {
        H5Pset_dxpl_mpio(plist_id, H5FD_MPIO_COLLECTIVE);
    }

    // allocate some space for the dummy data
    const int n_elem = block_size * N * N;
    float* data = calloc(n_elem, sizeof(float));

    // loop through blocks and write some dummy data
    for (int block_start = i_rank * local_size; block_start < (i_rank + 1) * local_size; block_start += block_size) {
        // select a hyperslab in the filespace
        hsize_t start[3] = { block_start, 0, 0 };
        hsize_t count[3] = { block_size, N, N };
        H5Sselect_hyperslab(fspace_id, H5S_SELECT_SET, start, NULL, count, NULL);

        // create the dummy data
        for (int ii = 0; ii < n_elem; ii++) {
            data[ii] = block_start * N * N + ii;
        }

        // write the dataset
        H5Dwrite(dset_id, H5T_NATIVE_FLOAT, memspace_id, fspace_id, plist_id, data);
    }

    // cleanup
    free(data);
    H5Pclose(plist_id);
    H5Dclose(dset_id);
    H5Sclose(memspace_id);
    H5Sclose(fspace_id);
    H5Fclose(file_id);

    return 1;
}
