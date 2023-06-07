#ifndef toto_h
#define toto_h
#ifdef __cplusplus
extern "C" {
#endif


#include <stddef.h>


enum toto_return {
    LIB_SUCCESS = 0,
    LIB_FAILURE
};

struct toto_img;

typedef double (toto_img_get_t) (
    const struct toto_img * self,
    size_t i,
    size_t j
);

typedef double * toto_img_ref_t (
    struct toto_img * self,
    size_t i,
    size_t j
);

typedef void toto_img_set_t (
    struct toto_img * self,
    size_t i,
    size_t j,
    double v
);

typedef void toto_img_destroy_t (struct toto_img ** self_ptr);

struct toto_img {
    /* Meta data. */
    const size_t height;
    const size_t width;

    /* Get/setters. */
    toto_img_get_t * get;
    toto_img_ref_t * ref;
    toto_img_set_t * set;

    /* Destructor. */
    toto_img_destroy_t * destroy;
};

/* Create an empty image. */
struct toto_img * toto_img_empty(
    size_t height,
    size_t width
);

/* Create a zeroed image. */
struct toto_img * toto_img_zeros(
    size_t height,
    size_t width
);

/* Inplace addition. */
enum toto_return toto_img_iadd(
    struct toto_img * self,
    const struct toto_img * other
);


#ifdef __cplusplus
}
#endif
#endif