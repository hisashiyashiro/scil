//#include <stdint.h>

#include <scil-util.h>
#include <scil.h>

#include <scil-patterns.h>

int main()
{
    const size_t count = 100;

    float* source = (float*)SAFE_MALLOC(count * sizeof(float));

    scil_dims dims;
    scil_init_dims_1d(&dims, count);

    scilP_create_pattern_float(&dims, source, "random", -100.0f, 100.0f, 0.0f, 0.0f);

    size_t dest_size = scil_compress_buffer_size_bound(SCIL_TYPE_FLOAT, &dims);
    byte* dest       = (byte*)SAFE_MALLOC(dest_size);

    scil_context_p ctx;
    scil_hints hints;
    hints.absolute_tolerance        = 1e-100;
    hints.force_compression_methods = "1";
    scil_init_hints(&hints);
    scil_create_compression_context(&ctx, SCIL_TYPE_FLOAT, &hints);

    int ret = scil_compress(dest, dest_size, source, &dims, &dest_size, ctx);
    printf("%d %d\n", ret, SCIL_PRECISION_ERR);

    free(source);
    free(dest);
    free(ctx);

    return ret != SCIL_PRECISION_ERR;
}
