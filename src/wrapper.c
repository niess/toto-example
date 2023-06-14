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


/* Wrapping parametric model constructor. */
struct toto_model * toto_model_create_w(
    size_t parameter_stride,
    const void * parameter_data,
    const size_t images_properties[6],
    void * images_data)
{
        /* Wrap parameter vector. */
        size_t depth = images_properties[0];
        double parameter[depth];
        size_t i;
        for (i = 0; i < depth; i++) {
                parameter[i] =
                    *(double *)(parameter_data + i * parameter_stride);
        }

        /* Wrap images collection. */
        struct wrapper collection_wrapper[depth];
        const struct toto_img * collection[depth];
        size_t collection_properties[4] = {
            images_properties[1],
            images_properties[2],
            images_properties[4],
            images_properties[5]
        };
        for (i = 0; i < depth; i++) {
                collection[i] = wrapper_img(
                    collection_wrapper + i,
                    collection_properties,
                    images_data + i * images_properties[3]
                );
        }

        /* Call the library function. */
        return toto_model_create(
            depth,
            parameter,
            collection
        );
}


enum toto_return toto_model_get_w(
    const struct toto_model * model,
    double parameter,
    const size_t image_properties[4],
    void * image_data)
{
        /* Wrap output image. */
        struct wrapper image_wrapper;
        struct toto_img * image = wrapper_img(
            &image_wrapper,
            image_properties,
            image_data
        );

        return toto_model_get(
            model,
            parameter,
            image
        );
}


enum toto_return toto_model_invert_w(
    const struct toto_model * model,
    const size_t observation_properties[4],
    void * observation_data,
    const size_t parameter_properties[4],
    void * parameter_data)
{
        /* Wrap input observation. */
        struct wrapper observation_wrapper;
        struct toto_img * observation = wrapper_img(
            &observation_wrapper,
            observation_properties,
            observation_data
        );

        /* Wrap output parameter values. */
        struct wrapper parameter_wrapper;
        struct toto_img * parameter = wrapper_img(
            &parameter_wrapper,
            parameter_properties,
            parameter_data
        );

        return toto_model_invert(
            model,
            observation,
            parameter
        );
}
