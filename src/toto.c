/* C standard library. */
#include <stdlib.h>
/* Public API. */
#include "toto.h"


/* IMG implementation. */
struct img {
        /* Public interface. */
        struct toto_img pub;

        /* Placeholder for data. */
        double data[];
};


static double img_get(
    const struct toto_img * self,
    size_t i,
    size_t j) 
{
        struct img * img = (void *)self;
        return img->data[i * self->width + j];
}


static double * img_ref(
    struct toto_img * self,
    size_t i, size_t j)
{
        struct img * img = (void *)self;
        return img->data + i * self->width + j;
}


static void img_set(
    struct toto_img * self,
    size_t i, size_t j,
    double v)
{
        struct img * img = (void *)self;
        img->data[i * self->width + j] = v;
}


static void img_destroy(struct toto_img ** self_ptr)
{
        if ((self_ptr == NULL) || (*self_ptr == NULL)) return;
        free(*self_ptr);
        self_ptr = NULL;
}

static struct toto_img * create_img(
    size_t height,
    size_t width,
    void * (*allocator)(size_t)
    )
{
        /* Allocate memory. */
        struct img * img = allocator(
            (sizeof *img) + height * width * (sizeof *img->data));
        if (img == NULL) return NULL;

        /* Set meta-data. */
        *(size_t *)&img->pub.height = height;
        *(size_t *)&img->pub.width = width;

        /* Set methods. */
        img->pub.get = &img_get;
        img->pub.ref = &img_ref;
        img->pub.set = &img_set;
        img->pub.destroy = &img_destroy;

        /* Return public interface. */
        return &img->pub;
}


struct toto_img * toto_img_empty(
    size_t height,
    size_t width)
{
        return create_img(
            height,
            width,
            &malloc
        );
}


static void * zalloc(size_t size) { return calloc(1, size); }

struct toto_img * toto_img_zeros(
    size_t height,
    size_t width)
{
        return create_img(
            height,
            width,
            &zalloc
        );
}


/* Inplace addition(s). */
enum toto_return toto_img_iadd(
    struct toto_img * self,
    const struct toto_img * other)
{
        if ((self->height != other->height) ||
            (self->width != other->width)) {
                return TOTO_FAILURE;
        }

        toto_img_ref_t * ref = self->ref;
        toto_img_get_t * get = other->get;
        for (size_t i = 0; i < self->height; i++) {
                for (size_t j = 0; j < self->width; j++) {
                        *ref(self, i, j) += get(other, i, j);
                }
        }

        return TOTO_SUCCESS;
}

enum toto_return toto_img_iadd_v(
    struct toto_img * self,
    size_t size,
    const struct toto_img * others[])
{
        size_t i;
        for (i = 0; i < size; i++) {
                enum toto_return r = toto_img_iadd(self, others[i]);
                if (r != TOTO_SUCCESS) {
                        return r;
                }
        }
        return TOTO_SUCCESS;
}
