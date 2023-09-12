## Yeah, this is kinda LOLZ but before 
## bootstrapping project, it will be built
## using CMake and Make...

cmake_minimum_required(VERSION 3.25)
project(bbult CXX)

set(EXEC_NAME ${PROJECT_NAME})

file(GLOB_RECURSE
    ALL_WITH_CPP
    "**.cpp"
)

file(GLOB_RECURSE
    ALL_WITH_CXX
    "**.cxx"
)

file(GLOB_RECURSE
    ALL_WITH_CC
    "**.cc"
)

file(GLOB_RECURSE
    ALL_WITH_C
    "**.c"
)

set(CMAKE_CXX_STANDARD 23)
set(SOURCE_FILES ${ALL_WITH_CPP} ${ALL_WITH_CXX} ${ALL_WITH_CC} ${ALL_WITH_C})

add_executable(${EXEC_NAME} ${SOURCE_FILES})
