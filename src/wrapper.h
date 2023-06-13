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
