import ctypes


# Load the C library.
lib = ctypes.CDLL("lib/libtoto.so")


# Bytes container for C wrapper object.
WRAPPER = lib.toto_wrapper_size() * ctypes.c_byte


def wrap(a):
    """Wrap a numpy array."""

    assert(a.dtype == "f8")

    wrapper = WRAPPER()
    lib.toto_wrapper_img(
        wrapper,
        a.shape[0],
        a.shape[1],
        a.strides[0],
        a.strides[1],
        a.ctypes.data
    )

    return wrapper


def iadd(a, b):
    """Add images, inplace."""

    assert(a.shape == b.shape)

    lib.toto_img_iadd(
        wrap(a),
        wrap(b)
    )
