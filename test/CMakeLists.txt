# Parent project does not export its library target, so this CML implicitly
# depends on being added from it, i.e. the testing is done only from the build
# tree and is not feasible from an install location

project(BrixActivityWatchTests LANGUAGES CXX)

# ---- Tests ----

add_executable(BrixActivityWatch_test source/BrixActivityWatch_test.cpp)
target_compile_features(BrixActivityWatch_test PRIVATE cxx_std_17)

add_test(NAME BrixActivityWatch_test COMMAND BrixActivityWatch_test)

# ---- End-of-file commands ----

add_folders(Test)
