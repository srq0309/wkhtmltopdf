if(MSVC)
    set(CMAKE_C_FLAGS_RELEASE_INIT        "/MD /O2 /Ob2 /D NDEBUG")
    set(CMAKE_C_FLAGS_RELWITHDEBINFO_INIT "/MD /Zi /Od /Ob0 /D NDEBUG")
endif()
