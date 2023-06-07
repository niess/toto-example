/* C standard library. */
#include <stdio.h>
/* Library API. */
#include "toto.h"


int main()
{
        /* Create images. */
        size_t height = 3;
        size_t width = 2;
        struct toto_img * a = toto_img_empty(height, width);
        struct toto_img * b = toto_img_empty(height, width);

        for (size_t i = 0; i < height; i++) {
                for (size_t j = 0; j < width; j++) {
                        a->set(a, i, j, 1.0);
                        b->set(b, i, j, 2.0);
                }
        }

        /* Add in-place and print result. */
        toto_img_iadd(a, b);

        for (size_t i = 0; i < height; i++) {
                for (size_t j = 0; j < width; j++) {
                        printf(" %g", a->get(a, i, j));
                }
                puts("");
        }

        /* Clear memory. */
        a->destroy(&a);
        b->destroy(&b);
}
