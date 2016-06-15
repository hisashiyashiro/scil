#ifndef SCIL_QUANTIZER_H
#define SCIL_QUANTIZER_H

#include <stdlib.h>
#include <stdint.h>

/**
 * \brief Quantizes the values of the given buffer.
 * \param buf_in The Buffer containing the data to quantize
 * \param buf_out The Buffer which will hold the quantized data.
 * \param count Number of elements in the buffer
 * \param absolute_tolerance The maximal tolerated error through quantizing
 * \pre buf_in != NULL
 * \pre buf_out != NULL
 * \return SCIL error code
 */
 int scil_quantize_buffer_double(const double* const restrict buf_in,
                                 uint64_t* const restrict buf_out,
                                 const size_t count,
                                 const double absolute_tolerance);

 /**
  * \brief Quantizes the values of the given buffer.
  * \param buf_in The Buffer containing the data to quantize
  * \param buf_out The Buffer which will hold the quantized data.
  * \param count Number of elements in the buffer
  * \param absolute_tolerance The maximal tolerated error through quantizing
  * \pre buf_in != NULL
  * \pre buf_out != NULL
  * \return SCIL error code
  */
  int scil_quantize_buffer_float(const float* const restrict buf_in,
                                 uint64_t* const restrict buf_out,
                                 const size_t count,
                                 const double absolute_tolerance);

#endif /* SCIL_QUANTIZER_H */
