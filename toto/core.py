from .wrapper import ffi, lib


def iadd(a, b):
    """Add images, inplace."""

    if len(b.shape) == 3:
        # Second argument is a collection of 2d images stored as a 3d numpy
        # ndarray. Let us first check that images all have the same shape.
        assert(a.shape == b.shape[1:])

        # Then, we call the vectorized C function.
        lib.toto_img_iadd_v_w(
            (*a.shape, *a.strides),
            ffi.from_buffer(a),
            (*b.shape, *b.strides),
            ffi.from_buffer(b),
        )

    else:
        # Second argument is a 2d image. Let us first check that both images
        # have the same shape.
        assert(a.shape == b.shape)

        # Then, we call the C function.
        lib.toto_img_iadd_w(
            (*a.shape, *a.strides),
            ffi.from_buffer(a),
            (*b.shape, *b.strides),
            ffi.from_buffer(b),
        )
