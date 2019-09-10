#ifndef SAVE_H
#define SAVE_H
#include <assert.h>
#include <hdf5.h>
#include <hdf5_hl.h>
#include <stdio.h>
#include <stdlib.h>

int save(const int N, const bool flag_chunked, const bool flag_independent);
#endif /* ifndef SAVE_H */
