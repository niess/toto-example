#include "toto.h"
#include "wrapper.h"


/* Wrapper for numpy arrays. */
struct wrapper {
        struct toto_img pub;
        size_t stride_h;
        size_t stride_w;
        double * data;
};


static double wrapper_get(
    const struct toto_img * self,
    size_t i,
    size_t j) 
{
        struct wrapper * wrapper = (void *)self;
        return wrapper->data[i * wrapper->stride_h + j * wrapper->stride_w];
}


static double * wrapper_ref(
    struct toto_img * self,
    size_t i, size_t j)
{
        struct wrapper * wrapper = (void *)self;
        return wrapper->data + i * wrapper->stride_h + j * wrapper->stride_w;
}


static void wrapper_set(
    struct toto_img * self,
    size_t i, size_t j,
    double v)
{
        struct wrapper * wrapper = (void *)self;
        wrapper->data[i * wrapper->stride_h + j * wrapper->stride_w] = v;
}


/* Create an image wrapping an existing numpy array. */
struct toto_img * toto_wrapper_img(
    void * wrapper,
    const size_t height,
    const size_t width,
    const size_t stride_h,
    const size_t stride_w,
    void * data)
{
        /* Map raw bytes. */
        struct wrapper * w = wrapper;

        /* Set public metadata. */
        *(size_t *)&w->pub.height = height;
        *(size_t *)&w->pub.width = width;

        w->pub.get = &wrapper_get;
        w->pub.ref = &wrapper_ref;
        w->pub.set = &wrapper_set;
        w->pub.destroy = NULL;

        /* Set numpy (meta)data. */
        w->stride_h = stride_h / (sizeof w->data);
        w->stride_w = stride_w / (sizeof w->data);
        w->data = data;

        return &w->pub;
}
