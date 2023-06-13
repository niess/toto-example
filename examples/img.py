#! /usr/bin/env python3

import numpy
import toto

shape = (201, 201)
a = numpy.ones(shape)
b = numpy.full(shape, 2.0)
toto.iadd(a, b)
print(a)

a = numpy.zeros(shape)
b = numpy.stack((
    numpy.full(shape, 1.0),
    numpy.full(shape, 2.0),
    numpy.full(shape, 3.0)
))
toto.iadd(a, b)
print(a)
