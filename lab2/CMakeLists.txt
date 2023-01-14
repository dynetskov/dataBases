cmake_minimum_required(VERSION 3.22)
project(email_service_app)

set(CMAKE_CXX_STANDARD 20)
include(FetchContent)

FetchContent_Declare(
        libpqxx
        GIT_REPOSITORY https://github.com/jtv/libpqxx.git
        GIT_TAG a6b1d60e74c1427c8ac2324b85cd4a0dc2068332
)
set(PQXX_LIBRARIES pqxx_static)

add_executable(${PROJECT_NAME} main.cpp model.h view.h controller.h db_backend.h)
FetchContent_MakeAvailable(libpqxx)
target_link_libraries(${PROJECT_NAME} "${PQXX_LIBRARIES}")