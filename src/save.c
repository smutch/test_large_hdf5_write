#include "save.h"

int save(const int N) {

    int i_rank, n_ranks;
    MPI_Comm_rank(MPI_COMM_WORLD, &i_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &n_ranks);

    const int chunk_size = (int)(N / n_ranks);
    assert(chunk_size * n_ranks == N);
    
    // create the file (in parallel)
    hid_t plist_id = H5Pcreate(H5P_FILE_ACCESS);
    H5Pset_fapl_mpio(plist_id, MPI_COMM_WORLD, MPI_INFO_NULL);
    hid_t file_id = H5Fcreate("test_dump.h5", H5F_ACC_TRUNC, H5P_DEFAULT, plist_id);
    H5Pclose(plist_id);

    // create the filespace
    hsize_t dims[3] = { N, N, N };
    hid_t fspace_id = H5Screate_simple(3, dims, NULL);

    // create the memspace
    hsize_t mem_dims[3] = { chunk_size, N, N };
    hid_t memspace_id = H5Screate_simple(3, mem_dims, NULL);

    // create the dataset (NO CHUNKING IN THIS TEST)
    hid_t dset_id = H5Dcreate(file_id, "data", H5T_NATIVE_FLOAT, fspace_id,
        H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);

    // create the property list
    plist_id = H5Pcreate(H5P_DATASET_XFER);
    H5Pset_dxpl_mpio(plist_id, H5FD_MPIO_COLLECTIVE);
    
    // select a hyperslab in the filespace
    hsize_t start[3] = { i_rank*chunk_size, 0, 0 };
    hsize_t count[3] = { chunk_size, N, N };
    H5Sselect_hyperslab(fspace_id, H5S_SELECT_SET, start, NULL, count, NULL);

    // create the dummy data
    const int n_elem = chunk_size * N * N;
    float *data = calloc(n_elem, sizeof(float));
    for(int ii=0; ii < n_elem; ii++) {
        data[ii] = n_elem*i_rank + ii;
    }

    // write the dataset
    H5Dwrite(dset_id, H5T_NATIVE_FLOAT, memspace_id, fspace_id, plist_id, data);

    // cleanup
    free(data);
    H5Pclose(plist_id);
    H5Dclose(dset_id);
    H5Sclose(memspace_id);
    H5Sclose(fspace_id);
    H5Fclose(file_id);

    return 1;
}
