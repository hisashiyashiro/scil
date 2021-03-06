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

#include <scil.h>

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

  // See:  https://support.hdfgroup.org/HDF5/doc/Advanced/DynamicallyLoadedFilters/HDF5DynamicallyLoadedFilters.pdf
  scil_user_hints_t hints;
  scil_user_hints_initialize(& hints);
  hints.absolute_tolerance = 1;
  hints.force_compression_methods = "abstol,lz4";
  H5Pset_scil_user_hints_t(dcpl, & hints);

  double value = 54.34;
  err = H5Pset_fill_value(dcpl, H5T_NATIVE_DOUBLE, & value);

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

  err = H5Dwrite( dset, H5T_NATIVE_DOUBLE, H5S_ALL, H5S_ALL, H5P_DEFAULT, data);
  assert(err == 0);


  // check the results, this comes from the cache!
  memset(data, -1, sizeof(double)*10*4);

  err = H5Dread( dset, H5T_NATIVE_DOUBLE, H5S_ALL, H5S_ALL, H5P_DEFAULT, data);
  assert(err == 0);

  for (int i=0; i < 4; i++){
    buff[i] = & data[10*i];
    for(int j=0; j < 10; j++){
      printf("%f ", buff[i][j]);
      assert(buff[i][j] == (i+1)*j);
    }
    printf("\n");
  }

  H5Dclose(dset);
  H5Fclose(fid);

  printf("Now run: h5dump -p test-example.h5 to check the results\n");

  return 0;
}
