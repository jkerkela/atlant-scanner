# atlant-scanner

## Description
C++ example implementation for F-Secure Atlant API file scanning (https://help.f-secure.com/product.html#business/atlant/latest/en/concept_C10CD401FFA94772AF3D88E0D2DC6710-latest-en)

## Integration
Project is build as VS solution. Project uses C++ 17.
- NOTE: Implementation is not tested with live server.

3rd party dependencies:
  - POCO
     - Precompiled to external/libs
     - Can be build and installed by following steps on https://github.com/pocoproject/poco (tested on windows, using cmake to build).
  - Gtest
     - Check reference https://docs.microsoft.com/en-us/visualstudio/test/how-to-use-google-test-for-cpp?view=vs-2019
  - Gmock
  
Project supports building with:
  - CMake
  - MSVC solution (Visual Studio solution)

