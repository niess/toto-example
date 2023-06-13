import ctypes
import weakref


# Load the C library.
lib = ctypes.CDLL("lib/libtoto.so")

# Prototype C library functions.
lib.toto_wrapper_size.argstypes = None
lib.toto_wrapper_size.restype = ctypes.c_size_t

lib.toto_wrapper_img.argstypes = [
    ctypes.c_void_p,
    ctypes.c_size_t,
    ctypes.c_size_t,
    ctypes.c_size_t,
    ctypes.c_size_t,
    ctypes.c_void_p
]
lib.toto_wrapper_img.restype = ctypes.c_void_p

lib.toto_img_iadd.argtypes = [
    ctypes.c_void_p,
    ctypes.c_void_p,
]
lib.toto_img_iadd.restype = None

lib.toto_img_iadd_v.argtypes = [
    ctypes.c_void_p,
    ctypes.c_size_t,
    ctypes.c_void_p,
]
lib.toto_img_iadd_v.restype = None


# Bytes container for C wrapper object.
WRAPPER = lib.toto_wrapper_size() * ctypes.c_byte


def wrap2(a):
    """Wrap a numpy 2d array as a C image."""

    assert(a.dtype == "f8")

    wrapper = WRAPPER()
    lib.toto_wrapper_img(
        wrapper,
        a.shape[0],
        a.shape[1],
        a.strides[0],
        a.strides[1],
        a.ctypes.data_as(ctypes.c_void_p)
    )

    return wrapper


def wrap3(a):
    """Wrap a numpy 3d array as a collection of C images."""

    assert(a.dtype == "f8")

    # Let us first create a set of C wrappers, for each image of the collection.
    # Note that these wrappers must be kept in scope until the C library call
    # (otherwise, the corresponding memory could be deallocated).
    wrappers = [wrap2(ai) for ai in a]

    # Then, we create an array of pointers to these wrappers. That is, to
    # the C images.
    size = a.shape[0]
    images = (size * ctypes.c_void_p)(
        *[ctypes.addressof(wi) for wi in wrappers]
    )

    # Below, we register a finalizer for images, depending on wrappers. This
    # forces keeping wrappers alive until images are garbage collected.
    weakref.finalize(images, lambda x: x, wrappers)

    return images


def iadd(a, b):
    """Add images, inplace."""

    if len(b.shape) == 3:
        # Second argument is a collection of 2d images stored as a 3d numpy
        # ndarray. Let us first check that images all have the same shape.
        assert(a.shape == b.shape[1:])

        # Then, we call the vectorized C function.
        lib.toto_img_iadd_v(
            wrap2(a),
            b.shape[0],
            wrap3(b)
        )

    else:
        # Second argument is a 2d image. Let us first check that both images
        # have the same shape.
        assert(a.shape == b.shape)

        # Then, we call the C function.
        lib.toto_img_iadd(
            wrap2(a),
            wrap2(b)
        )
