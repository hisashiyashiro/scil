// This file is part of SCIL.
//
// SCIL is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// SCIL is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with SCIL.  If not, see <http://www.gnu.org/licenses/>.

#include <string.h>
#include <hdf5.h>
#include <stdlib.h>
#include <assert.h>

#include <scil-hdf5-plugin.h>

// use the plugin to compress some floating point data.

// h5dump -A -p test-example.h5

int main(){
  hid_t fid;
  herr_t err;
  hid_t data_space;

  fid = H5Fcreate("test-example.h5", H5F_ACC_TRUNC, H5P_DEFAULT, H5P_DEFAULT);

  hid_t dcpl = H5Pcreate(H5P_DATASET_CREATE);
  hsize_t chunk_size[2] = {2,5};
  H5Pset_chunk(dcpl, 2, chunk_size);
  H5Pset_filter(dcpl, SCIL_ID, 0, 0, NULL);

  hsize_t dims[2] = {4,10};
  data_space = H5Screate_simple (2, dims, NULL);
  hid_t dset = H5Dcreate(fid, "dset", H5T_NATIVE_DOUBLE, data_space, H5P_DEFAULT, dcpl, H5P_DEFAULT);

  double * data = (double*) malloc(sizeof(double)*10*4);
  double ** buff = malloc(sizeof(double*) * 4);
  for (int i=0; i < 4; i++){
    buff[i] = & data[10*i];
    for(int j=0; j < 10; j++){
      buff[i][j] = (i+1)*j;
    }
  }

  /*
  for (int i=0; i < 4; i++){
    for(int j=0; j < 10; j++){
      printf("%f ", buff[i][j]);
    }
    printf("\n");
  }
  */

  err = H5Dwrite( dset, H5T_NATIVE_DOUBLE, H5S_ALL, H5S_ALL, H5P_DEFAULT, data);
  assert(err == 0);

  err = H5Dread( dset, H5T_NATIVE_DOUBLE, H5S_ALL, H5S_ALL, H5P_DEFAULT, data);
  assert(err == 0);

  H5Dclose(dset);
  H5Fclose(fid);

  return 0;
}
