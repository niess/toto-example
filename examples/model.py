#! /usr/bin/env python3

import matplotlib.pyplot as plot
import numpy
import toto


# Parametric model.
f = lambda x: 4 - x**2

# Sample the model over 11 points.
parameter = numpy.linspace(1.0, 2.0, 11)
shape = (201, 201)
images = numpy.stack([
    numpy.full(shape, f(x)) for x in parameter
])

# Create an interpolation model.
model = toto.Model(parameter, images)
print(f"shape = {model.shape}")
print(f"pmin  = {model.pmin}")
print(f"pmax  = {model.pmax}")

# Check interpolation at nodes.
for x in parameter:
    snapshot = model(x)
    assert((snapshot == f(x)).all())

# Compare interpolation to true model.
parameter = numpy.linspace(parameter[0], parameter[-1], 101)
interpolation = numpy.empty(parameter.shape)
for i, xi in enumerate(parameter):
    snapshot = model(xi)
    interpolation[i] = numpy.mean(snapshot)

plot.figure()
plot.plot(parameter, f(parameter), "r-", label="truth")
plot.plot(parameter, interpolation, "k.", label="interpolation")
plot.xlabel("parameter")
plot.ylabel("model")
plot.legend()

# Compare backward interpolation to true inverse model.
g = lambda x: numpy.sqrt(4.0 - x)

observation = numpy.linspace(f(parameter[0]), f(parameter[-1]), 101)
parameter = numpy.empty(observation.shape)
for i, yi in enumerate(observation):
    inverse = model.invert(numpy.full(shape, yi))
    parameter[i] = numpy.mean(inverse)

plot.figure()
plot.plot(observation, g(observation), "r-", label="truth")
plot.plot(observation, parameter, "k.", label="inverse")
plot.xlabel("observation")
plot.ylabel("parameter")
plot.legend()

plot.show()
