// generated from rosidl_generator_c/resource/idl__struct.h.em
// with input from message2:msg/Usbl.idl
// generated code does not contain a copyright notice

#ifndef MESSAGE2__MSG__DETAIL__USBL__STRUCT_H_
#define MESSAGE2__MSG__DETAIL__USBL__STRUCT_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>


// Constants defined in the message

// Include directives for member types
// Member 'header'
#include "std_msgs/msg/detail/header__struct.h"
// Member 'position'
#include "geometry_msgs/msg/detail/point__struct.h"

/// Struct defined in msg/Usbl in the package message2.
typedef struct message2__msg__Usbl
{
  std_msgs__msg__Header header;
  double range;
  double elevation;
  double azimuth;
  double depth;
  double yaw;
  double pitch;
  double roll;
  /// Local position from the source beacon: x,y,z
  geometry_msgs__msg__Point position;
  double beacon_id;
} message2__msg__Usbl;

// Struct for a sequence of message2__msg__Usbl.
typedef struct message2__msg__Usbl__Sequence
{
  message2__msg__Usbl * data;
  /// The number of valid items in data
  size_t size;
  /// The number of allocated items in data
  size_t capacity;
} message2__msg__Usbl__Sequence;

#ifdef __cplusplus
}
#endif

#endif  // MESSAGE2__MSG__DETAIL__USBL__STRUCT_H_
