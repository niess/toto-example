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
static struct toto_img * wrapper_img(
    struct wrapper * wrapper,
    const size_t properties[4],
    void * data)
{
        /* Map raw bytes. */
        struct wrapper * w = wrapper;

        /* Set public metadata. */
        *(size_t *)&w->pub.height = properties[0];
        *(size_t *)&w->pub.width = properties[1];

        w->pub.get = &wrapper_get;
        w->pub.ref = &wrapper_ref;
        w->pub.set = &wrapper_set;
        w->pub.destroy = NULL;

        /* Set numpy (meta)data. */
        w->stride_h = properties[2] / (sizeof w->data);
        w->stride_w = properties[3] / (sizeof w->data);
        w->data = data;

        return &w->pub;
}


/* Wrapping inplace addition. */
enum toto_return toto_img_iadd_w(
    size_t self_properties[4],
    void * self_data,
    size_t other_properties[4],
    void * other_data) 
{
        /* Wrap images. */
        struct wrapper self_wrapper, other_wrapper;
        struct toto_img * self = wrapper_img(
            &self_wrapper,
            self_properties,
            self_data
        );
        struct toto_img * other = wrapper_img(
            &other_wrapper,
            other_properties,
            other_data
        );

        /* Call the library function. */
        return toto_img_iadd(self, other);
}

enum toto_return toto_img_iadd_v_w(
    size_t self_properties[4],
    void * self_data,
    size_t other_properties[6],
    void * other_data)
{
        /* Wrap image. */
        struct wrapper self_wrapper;
        struct toto_img * self = wrapper_img(
            &self_wrapper,
            self_properties,
            self_data
        );

        /* Wrap images collection. */
        size_t size = other_properties[0];
        struct wrapper others_wrapper[size];
        const struct toto_img * others[size];
        size_t others_properties[4] = {
            other_properties[1],
            other_properties[2],
            other_properties[4],
            other_properties[5]
        };
        size_t i;
        for (i = 0; i < size; i++) {
                others[i] = wrapper_img(
                    others_wrapper + i,
                    others_properties,
                    other_data + i * other_properties[3]
                );
        }

        /* Call the library function. */
        return toto_img_iadd_v(self, size, others);
}
