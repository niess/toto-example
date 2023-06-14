#ifndef toto_h
#define toto_h
#ifdef __cplusplus
extern "C" {
#endif


#include <stddef.h>


/* Version macros */
#define TOTO_VERSION_MAJOR 0
#define TOTO_VERSION_MINOR 1
#define TOTO_VERSION_PATCH 0


enum toto_return {
    TOTO_SUCCESS = 0,
    TOTO_FAILURE
};


/* ============================================================================
 * Image interface
 * ============================================================================
 */
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
    /* Metadata. */
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

enum toto_return toto_img_iadd_v(
    struct toto_img * self,
    size_t size,
    const struct toto_img * others[]
);


/* ============================================================================
 * Model interface
 * ============================================================================
 */
struct toto_model;

typedef void toto_model_destroy_t (struct toto_model ** self_ptr);

struct toto_model {
    /* Metadata. */
    const size_t height;
    const size_t width;
    const double pmin;
    const double pmax;

    /* Destructor. */
    toto_model_destroy_t * destroy;
};

/* Create a model object from a parametric collection of images. */
struct toto_model * toto_model_create(
    size_t size,
    const double parameter[],
    const struct toto_img * images[]
);

/* Get a snapshot of the model for a given parameter value (by interpolation).
 */
enum toto_return toto_model_get(
    const struct toto_model * model,
    double parameter,
    struct toto_img * image
);

/* Get parameter values for a given model and observation (i.e. invert the
 * observation for the model).
 */
enum toto_return toto_model_invert(
    const struct toto_model * model,
    const struct toto_img * observation,
    struct toto_img * parameter
);

#ifdef __cplusplus
}
#endif
#endif
