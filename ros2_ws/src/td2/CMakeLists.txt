cmake_minimum_required(VERSION 3.8)
project(td2)

# Activer les avertissements de compilation pour le débogage
if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# Trouver les dépendances nécessaires
find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(geometry_msgs REQUIRED)
find_package(Eigen3 REQUIRED)
find_package(tf2 REQUIRED)
find_package(tf2_ros REQUIRED)
find_package(tf2_geometry_msgs REQUIRED)
find_package(visualization_msgs REQUIRED)

# Ajouter les fichiers source
add_executable(boat_simulation_node src/boat_simulation.cpp)
add_executable(boat_controller_node src/boat_controller.cpp)
add_executable(target_publisher_node src/boat_global_target.cpp)

# Lier les bibliothèques nécessaires
ament_target_dependencies(boat_simulation_node
  rclcpp
  geometry_msgs
  tf2
  tf2_ros
  tf2_geometry_msgs
  Eigen3
  visualization_msgs
)
ament_target_dependencies(boat_controller_node
  rclcpp
  geometry_msgs
  tf2
  tf2_geometry_msgs
)
ament_target_dependencies(target_publisher_node
  rclcpp
  geometry_msgs
)

# Installation de l'exécutable

 install(TARGETS
  boat_simulation_node
  boat_controller_node
  target_publisher_node
  DESTINATION lib/${PROJECT_NAME})


# Tests (facultatif, laissé tel quel)
if(BUILD_TESTING)
  find_package(ament_lint_auto REQUIRED)
  set(ament_cmake_cpplint_FOUND TRUE)
  ament_lint_auto_find_test_dependencies()
endif()

install(DIRECTORY launch
  DESTINATION share/${PROJECT_NAME}/
)
install(DIRECTORY
meshes
DESTINATION share/${PROJECT_NAME})

  
install(DIRECTORY
  config
  DESTINATION share/${PROJECT_NAME})
  
ament_package()
