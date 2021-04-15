# atlant-scanner

## Description
C++ example implementation for F-Secure Atlant API file scanning (https://help.f-secure.com/product.html#business/atlant/latest/en/concept_C10CD401FFA94772AF3D88E0D2DC6710-latest-en)

## Integration
Project is build as VS solution. Project uses C++ 17.
NOTE: Implementation is not tested with live server (see: Things to do).
NOTE: The libary linking is not complete, all dependency libraries needs to be precompiled to, added to and used from external/libs (see: Things to do). 
Currently linking problem is circumnvented by adding requried libs manually to output folder.

3rd party dependencies:
  - POCO
     - Precompiled to external/libs
     - Can be build and installed by following steps on https://github.com/pocoproject/poco (tested on windows, using cmake to build).
  - Gtest
     - Check reference https://docs.microsoft.com/en-us/visualstudio/test/how-to-use-google-test-for-cpp?view=vs-2019
  - Gmock

## Things to do:
  - Finalize implementation: (see //TODO items on sources)
    - Implement command line argument parser to AtlantScanner.cpp
	- Implement result printing to AtlantScanner.cpp
	- Encapsulate ~all 3rd party lib accessing code behind interface classes
	- Add unit tests for classes that are missing them
	- Fix libary linking / VS project configs
	- Add cmake build support