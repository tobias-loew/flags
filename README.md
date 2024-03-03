# flags
type-safe and non-intrusive bitwise-operators for C++ enums

[read the docs](https://tobias-loew.github.io/flags)

# ToDo

* update boost-list probing email to reflect latest changes (C++11 conformance, std::less stuff, godbolt example(see below))
* add tests for usage in sorted containers, order based algorithms
* add tests for constexpr usage 
* add support for fail on cuda
* create example on godbolt and put link here, into the docs and boost-list probing email
* add github action with test-matrix to test on all kind of compilers with all kind of C++ versions
* uniform macro handling
* update docs with latest changes (C++11 conformance), adjust macros
 
    * weak symbols prblem with gcc on mingw  

* finally, remove ToDo list

[![CI](https://github.com/tobias-loew/flags/actions/workflows/ci.yml/badge.svg)](https://github.com/tobias-loew/flags/actions/workflows/ci.yml)

[![License](https://img.shields.io/badge/license-boost-brightgreen.svg)](LICENSE_1_0.txt)
