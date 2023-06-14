- Build the C library and examples as

  ```
  make examples
  ```

- Build the Python package as ([cffi][CFFI] and [pcpp][PCPP] required)

  ```
  make package
  ```

- Add [toto package](toto) to your PYTHONPATH, e.g. as

  ```
  export PYTHONPATH=$PWD:$PYTHONPATH
  ```

  [CFFI]: https://pypi.org/project/cffi/
  [PCPP]: https://pypi.org/project/pcpp/
