cmake_minimum_required(VERSION 3.8)
project(usbl_seatrac)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# find dependencies
find_package(ament_cmake REQUIRED)

# Packages classique des noeuds
find_package(rclcpp REQUIRED)
find_package(std_msgs REQUIRED) #utilie aussi pour le msg

# Package seatrac_driver
find_package(seatrac_driver REQUIRED)

find_package(message2 REQUIRED)

if(NOT TARGET seatrac_driver)
  find_package(seatrac_driver QUIET)

  if(NOT TARGET seatrac_driver)
    include(FetchContent)
    FetchContent_Declare(seatrac_driver
      GIT_REPOSITORY https://gitlab.ensta-bretagne.fr/narvorpi/seatrac_driver.git
      GIT_TAG master
    )
    FetchContent_MakeAvailable(seatrac_driver)
  endif()
endif()

add_executable(usbl_pub_cpp src/usbl_pub_cpp.cpp)
add_executable(usbl_pub_cpp_choice_beacon src/usbl_pub_cpp_choice_beacon2.cpp)
add_executable(usbl_pub_cpp_multi src/usbl_pub_cpp_multi3.cpp)

target_link_libraries(usbl_pub_cpp seatrac_driver)
target_link_libraries(usbl_pub_cpp_choice_beacon seatrac_driver)
target_link_libraries(usbl_pub_cpp_multi seatrac_driver)

ament_target_dependencies(usbl_pub_cpp rclcpp std_msgs message2)
ament_target_dependencies(usbl_pub_cpp_choice_beacon rclcpp std_msgs message2)
ament_target_dependencies(usbl_pub_cpp_multi rclcpp std_msgs message2)


install(TARGETS
  usbl_pub_cpp
  usbl_pub_cpp_choice_beacon
  usbl_pub_cpp_multi
  DESTINATION lib/${PROJECT_NAME})


if(BUILD_TESTING)
  find_package(ament_lint_auto REQUIRED)
  # the following line skips the linter which checks for copyrights
  # comment the line when a copyright and license is added to all source files
  set(ament_cmake_copyright_FOUND TRUE)
  # the following line skips cpplint (only works in a git repo)
  # comment the line when this package is in a git repo and when
  # a copyright and license is added to all source files
  set(ament_cmake_cpplint_FOUND TRUE)
  ament_lint_auto_find_test_dependencies()
endif()

ament_package()
