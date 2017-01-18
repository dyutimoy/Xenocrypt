cmake_minimum_required(VERSION 2.8)
project( opencv )
find_package( OpenCV REQUIRED )

add_executable( conquest src/conquest.cpp )
add_executable( crypt src/crypt.cpp)
add_executable( pseudo src/pseudo.cpp)
add_executable ( learn src/learn.cpp)
target_link_libraries( conquest ${OpenCV_LIBS} )
target_link_libraries( crypt ${OpenCV_LIBS})
target_link_libraries( pseudo ${OpenCV_LIBS})
target_link_libraries( learn ${OpenCV_LIBS})
set(CMAKE_BUILD_TYPE Debug)

