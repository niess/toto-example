#! /usr/bin/env python3
from io import StringIO
from os import linesep
from pathlib import Path
import platform
import re

from cffi import FFI
from pcpp.preprocessor import Preprocessor


PREFIX = Path(__file__).parent.resolve()

MODULES = ("toto", "wrapper")


def source():
    source = [f"#include \"{PREFIX}/{module}.h\"" for module in MODULES]
    return linesep.join(source)


def definitions():
    cpp = Preprocessor()

    definitions = []
    for module in MODULES:
        with open(PREFIX / f"{module}.h") as f:
            src = f.read()

        src = re.sub(r'(?m)^#include.*\n?', '', src) # remove includes
        cpp.parse(src) # Parse other preprocessor statements
        output = StringIO()
        cpp.write(output)
        definitions.append(output.getvalue())

    return linesep.join(definitions)


def objects():
    return [str(PREFIX / f"{module}.o")
            for module in MODULES if module != "toto"]


def rpath():
    if platform.system() == "Linux":
        return ["-Wl,-rpath,$ORIGIN/lib",]
    elif platform.system() == "Darwin":
        return ["-Wl,-rpath,@loader_path/lib",]
    else:
        return None


ffi = FFI()
ffi.set_source("toto.wrapper", source(),
    extra_link_args=rpath(),
    extra_objects=objects(),
    libraries=["toto",],
    library_dirs=["lib",],
)
ffi.cdef(definitions())


if __name__ == "__main__":
    ffi.compile(verbose=True)
