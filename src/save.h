#ifndef SAVE_H
#define SAVE_H
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <hdf5.h>
#include <hdf5_hl.h>

int save(const int N, const bool flag_chunked);
#endif /* ifndef SAVE_H */
