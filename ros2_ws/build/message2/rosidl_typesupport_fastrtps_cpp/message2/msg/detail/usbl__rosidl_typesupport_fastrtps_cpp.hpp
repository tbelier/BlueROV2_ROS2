// generated from rosidl_typesupport_fastrtps_cpp/resource/idl__rosidl_typesupport_fastrtps_cpp.hpp.em
// with input from message2:msg/Usbl.idl
// generated code does not contain a copyright notice

#ifndef MESSAGE2__MSG__DETAIL__USBL__ROSIDL_TYPESUPPORT_FASTRTPS_CPP_HPP_
#define MESSAGE2__MSG__DETAIL__USBL__ROSIDL_TYPESUPPORT_FASTRTPS_CPP_HPP_

#include "rosidl_runtime_c/message_type_support_struct.h"
#include "rosidl_typesupport_interface/macros.h"
#include "message2/msg/rosidl_typesupport_fastrtps_cpp__visibility_control.h"
#include "message2/msg/detail/usbl__struct.hpp"

#ifndef _WIN32
# pragma GCC diagnostic push
# pragma GCC diagnostic ignored "-Wunused-parameter"
# ifdef __clang__
#  pragma clang diagnostic ignored "-Wdeprecated-register"
#  pragma clang diagnostic ignored "-Wreturn-type-c-linkage"
# endif
#endif
#ifndef _WIN32
# pragma GCC diagnostic pop
#endif

#include "fastcdr/Cdr.h"

namespace message2
{

namespace msg
{

namespace typesupport_fastrtps_cpp
{

bool
ROSIDL_TYPESUPPORT_FASTRTPS_CPP_PUBLIC_message2
cdr_serialize(
  const message2::msg::Usbl & ros_message,
  eprosima::fastcdr::Cdr & cdr);

bool
ROSIDL_TYPESUPPORT_FASTRTPS_CPP_PUBLIC_message2
cdr_deserialize(
  eprosima::fastcdr::Cdr & cdr,
  message2::msg::Usbl & ros_message);

size_t
ROSIDL_TYPESUPPORT_FASTRTPS_CPP_PUBLIC_message2
get_serialized_size(
  const message2::msg::Usbl & ros_message,
  size_t current_alignment);

size_t
ROSIDL_TYPESUPPORT_FASTRTPS_CPP_PUBLIC_message2
max_serialized_size_Usbl(
  bool & full_bounded,
  bool & is_plain,
  size_t current_alignment);

}  // namespace typesupport_fastrtps_cpp

}  // namespace msg

}  // namespace message2

#ifdef __cplusplus
extern "C"
{
#endif

ROSIDL_TYPESUPPORT_FASTRTPS_CPP_PUBLIC_message2
const rosidl_message_type_support_t *
  ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_fastrtps_cpp, message2, msg, Usbl)();

#ifdef __cplusplus
}
#endif

#endif  // MESSAGE2__MSG__DETAIL__USBL__ROSIDL_TYPESUPPORT_FASTRTPS_CPP_HPP_
