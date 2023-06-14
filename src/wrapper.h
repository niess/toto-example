/* Library API. */
#include "toto.h"

/* Numpy array wrapper for img_iadd. */
enum toto_return toto_img_iadd_w(
    size_t self_properties[4],
    void * self_data,
    size_t other_properties[4],
    void * other_data
);

enum toto_return toto_img_iadd_v_w(
    size_t self_properties[4],
    void * self_data,
    size_t other_properties[6],
    void * other_data
);

struct toto_model * toto_model_create_w(
    size_t parameter_stride,
    const void * parameter_values,
    const size_t images_properties[6],
    void * images_data
);

enum toto_return toto_model_get_w(
    const struct toto_model * model,
    double parameter,
    const size_t image_properties[4],
    void * image_data
);

enum toto_return toto_model_invert_w(
    const struct toto_model * model,
    const size_t observation_properties[4],
    void * observation_data,
    const size_t parameter_properties[4],
    void * parameter_data
);
