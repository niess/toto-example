#! /usr/bin/env python3

import numpy
import toto

shape = (3, 2)
a = numpy.ones(shape)
b = numpy.full(shape, 2.0)
toto.iadd(a, b)
print(a)
