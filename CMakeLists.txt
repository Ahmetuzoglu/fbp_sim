    cmake_minimum_required(VERSION 3.10)
    project(fbg_sim)

    set(CMAKE_CXX_STANDARD 17)

    # OpenMP paketini aktif et
    find_package(OpenMP REQUIRED)

    add_executable(scan 
        src/main.cpp 
        src/Common.h 
        src/PhantomGenerator.h 
        src/CTProcessor.h
    )

    # OpenMP'yi executable'a bağla
    if(OpenMP_CXX_FOUND)
        target_link_libraries(scan PUBLIC OpenMP::OpenMP_CXX)
    endif()