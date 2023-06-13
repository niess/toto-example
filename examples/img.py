#! /usr/bin/env python3

import numpy
import toto

shape = (201, 201)
a = numpy.ones(shape)
b = numpy.full(shape, 2.0)
toto.iadd(a, b)
print(a)

a = numpy.zeros(shape)
b = numpy.full([4, *shape], 2.0)
toto.iadd(a, b)
print(a)
