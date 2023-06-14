import numpy
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


class Model:
    """Parametric model of images, parametrised by a scalar.

       Note: The model is assumed to be continuous w.r.t. the scalar parameter.
    """

    @property
    def pmax(self):
        """Model parameter upper bound."""
        return float(self._c[0].pmax)

    @property
    def pmin(self):
        """Model parameter lower bound."""
        return float(self._c[0].pmin)

    @property
    def shape(self):
        """Shape of model images."""
        return (
            int(self._c[0].height),
            int(self._c[0].width)
        )

    def __init__(self, parameter, images):

        # Sanity checks.
        assert(parameter.dtype == "f8")
        assert(images.dtype == "f8")
        assert(parameter.ndim == 1)
        assert(images.ndim == 3)
        assert(images.shape[0] == parameter.shape[0])

        # Allocate the C object.
        c_model = ffi.new("struct toto_model *[1]")
        c_model[0] = lib.toto_model_create_w(
            parameter.strides[0],
            ffi.from_buffer(parameter),
            (*images.shape, *images.strides),
            ffi.from_buffer(images),
        )

        # Register a garbage collector for the C object.
        self._c = ffi.gc(
            c_model,
            lambda x: x[0].destroy(x)
        )

    def __call__(self, parameter):
        """Get a snapshot of the model for the given parameter value."""

        image = numpy.empty(self.shape)
        lib.toto_model_get_w(
            self._c[0],
            parameter,
            (*image.shape, *image.strides),
            ffi.from_buffer(image)
        )
        return image

    def invert(self, observation):
        """Get parameter values for a given observation."""

        assert(observation.dtype == "f8")
        assert(observation.shape == self.shape)

        parameter = numpy.empty(self.shape)
        lib.toto_model_invert_w(
            self._c[0],
            (*observation.shape, *observation.strides),
            ffi.from_buffer(observation),
            (*parameter.shape, *parameter.strides),
            ffi.from_buffer(parameter)
        )
        return parameter
