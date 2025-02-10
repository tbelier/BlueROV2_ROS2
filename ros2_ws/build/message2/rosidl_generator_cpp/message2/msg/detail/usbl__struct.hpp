// generated from rosidl_generator_cpp/resource/idl__struct.hpp.em
// with input from message2:msg/Usbl.idl
// generated code does not contain a copyright notice

#ifndef MESSAGE2__MSG__DETAIL__USBL__STRUCT_HPP_
#define MESSAGE2__MSG__DETAIL__USBL__STRUCT_HPP_

#include <algorithm>
#include <array>
#include <memory>
#include <string>
#include <vector>

#include "rosidl_runtime_cpp/bounded_vector.hpp"
#include "rosidl_runtime_cpp/message_initialization.hpp"


// Include directives for member types
// Member 'header'
#include "std_msgs/msg/detail/header__struct.hpp"
// Member 'position'
#include "geometry_msgs/msg/detail/point__struct.hpp"

#ifndef _WIN32
# define DEPRECATED__message2__msg__Usbl __attribute__((deprecated))
#else
# define DEPRECATED__message2__msg__Usbl __declspec(deprecated)
#endif

namespace message2
{

namespace msg
{

// message struct
template<class ContainerAllocator>
struct Usbl_
{
  using Type = Usbl_<ContainerAllocator>;

  explicit Usbl_(rosidl_runtime_cpp::MessageInitialization _init = rosidl_runtime_cpp::MessageInitialization::ALL)
  : header(_init),
    position(_init)
  {
    if (rosidl_runtime_cpp::MessageInitialization::ALL == _init ||
      rosidl_runtime_cpp::MessageInitialization::ZERO == _init)
    {
      this->range = 0.0;
      this->elevation = 0.0;
      this->azimuth = 0.0;
      this->depth = 0.0;
      this->yaw = 0.0;
      this->pitch = 0.0;
      this->roll = 0.0;
      this->beacon_id = 0.0;
    }
  }

  explicit Usbl_(const ContainerAllocator & _alloc, rosidl_runtime_cpp::MessageInitialization _init = rosidl_runtime_cpp::MessageInitialization::ALL)
  : header(_alloc, _init),
    position(_alloc, _init)
  {
    if (rosidl_runtime_cpp::MessageInitialization::ALL == _init ||
      rosidl_runtime_cpp::MessageInitialization::ZERO == _init)
    {
      this->range = 0.0;
      this->elevation = 0.0;
      this->azimuth = 0.0;
      this->depth = 0.0;
      this->yaw = 0.0;
      this->pitch = 0.0;
      this->roll = 0.0;
      this->beacon_id = 0.0;
    }
  }

  // field types and members
  using _header_type =
    std_msgs::msg::Header_<ContainerAllocator>;
  _header_type header;
  using _range_type =
    double;
  _range_type range;
  using _elevation_type =
    double;
  _elevation_type elevation;
  using _azimuth_type =
    double;
  _azimuth_type azimuth;
  using _depth_type =
    double;
  _depth_type depth;
  using _yaw_type =
    double;
  _yaw_type yaw;
  using _pitch_type =
    double;
  _pitch_type pitch;
  using _roll_type =
    double;
  _roll_type roll;
  using _position_type =
    geometry_msgs::msg::Point_<ContainerAllocator>;
  _position_type position;
  using _beacon_id_type =
    double;
  _beacon_id_type beacon_id;

  // setters for named parameter idiom
  Type & set__header(
    const std_msgs::msg::Header_<ContainerAllocator> & _arg)
  {
    this->header = _arg;
    return *this;
  }
  Type & set__range(
    const double & _arg)
  {
    this->range = _arg;
    return *this;
  }
  Type & set__elevation(
    const double & _arg)
  {
    this->elevation = _arg;
    return *this;
  }
  Type & set__azimuth(
    const double & _arg)
  {
    this->azimuth = _arg;
    return *this;
  }
  Type & set__depth(
    const double & _arg)
  {
    this->depth = _arg;
    return *this;
  }
  Type & set__yaw(
    const double & _arg)
  {
    this->yaw = _arg;
    return *this;
  }
  Type & set__pitch(
    const double & _arg)
  {
    this->pitch = _arg;
    return *this;
  }
  Type & set__roll(
    const double & _arg)
  {
    this->roll = _arg;
    return *this;
  }
  Type & set__position(
    const geometry_msgs::msg::Point_<ContainerAllocator> & _arg)
  {
    this->position = _arg;
    return *this;
  }
  Type & set__beacon_id(
    const double & _arg)
  {
    this->beacon_id = _arg;
    return *this;
  }

  // constant declarations

  // pointer types
  using RawPtr =
    message2::msg::Usbl_<ContainerAllocator> *;
  using ConstRawPtr =
    const message2::msg::Usbl_<ContainerAllocator> *;
  using SharedPtr =
    std::shared_ptr<message2::msg::Usbl_<ContainerAllocator>>;
  using ConstSharedPtr =
    std::shared_ptr<message2::msg::Usbl_<ContainerAllocator> const>;

  template<typename Deleter = std::default_delete<
      message2::msg::Usbl_<ContainerAllocator>>>
  using UniquePtrWithDeleter =
    std::unique_ptr<message2::msg::Usbl_<ContainerAllocator>, Deleter>;

  using UniquePtr = UniquePtrWithDeleter<>;

  template<typename Deleter = std::default_delete<
      message2::msg::Usbl_<ContainerAllocator>>>
  using ConstUniquePtrWithDeleter =
    std::unique_ptr<message2::msg::Usbl_<ContainerAllocator> const, Deleter>;
  using ConstUniquePtr = ConstUniquePtrWithDeleter<>;

  using WeakPtr =
    std::weak_ptr<message2::msg::Usbl_<ContainerAllocator>>;
  using ConstWeakPtr =
    std::weak_ptr<message2::msg::Usbl_<ContainerAllocator> const>;

  // pointer types similar to ROS 1, use SharedPtr / ConstSharedPtr instead
  // NOTE: Can't use 'using' here because GNU C++ can't parse attributes properly
  typedef DEPRECATED__message2__msg__Usbl
    std::shared_ptr<message2::msg::Usbl_<ContainerAllocator>>
    Ptr;
  typedef DEPRECATED__message2__msg__Usbl
    std::shared_ptr<message2::msg::Usbl_<ContainerAllocator> const>
    ConstPtr;

  // comparison operators
  bool operator==(const Usbl_ & other) const
  {
    if (this->header != other.header) {
      return false;
    }
    if (this->range != other.range) {
      return false;
    }
    if (this->elevation != other.elevation) {
      return false;
    }
    if (this->azimuth != other.azimuth) {
      return false;
    }
    if (this->depth != other.depth) {
      return false;
    }
    if (this->yaw != other.yaw) {
      return false;
    }
    if (this->pitch != other.pitch) {
      return false;
    }
    if (this->roll != other.roll) {
      return false;
    }
    if (this->position != other.position) {
      return false;
    }
    if (this->beacon_id != other.beacon_id) {
      return false;
    }
    return true;
  }
  bool operator!=(const Usbl_ & other) const
  {
    return !this->operator==(other);
  }
};  // struct Usbl_

// alias to use template instance with default allocator
using Usbl =
  message2::msg::Usbl_<std::allocator<void>>;

// constant definitions

}  // namespace msg

}  // namespace message2

#endif  // MESSAGE2__MSG__DETAIL__USBL__STRUCT_HPP_
