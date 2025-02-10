// generated from rosidl_generator_cpp/resource/idl__builder.hpp.em
// with input from message2:msg/Usbl.idl
// generated code does not contain a copyright notice

#ifndef MESSAGE2__MSG__DETAIL__USBL__BUILDER_HPP_
#define MESSAGE2__MSG__DETAIL__USBL__BUILDER_HPP_

#include <algorithm>
#include <utility>

#include "message2/msg/detail/usbl__struct.hpp"
#include "rosidl_runtime_cpp/message_initialization.hpp"


namespace message2
{

namespace msg
{

namespace builder
{

class Init_Usbl_beacon_id
{
public:
  explicit Init_Usbl_beacon_id(::message2::msg::Usbl & msg)
  : msg_(msg)
  {}
  ::message2::msg::Usbl beacon_id(::message2::msg::Usbl::_beacon_id_type arg)
  {
    msg_.beacon_id = std::move(arg);
    return std::move(msg_);
  }

private:
  ::message2::msg::Usbl msg_;
};

class Init_Usbl_position
{
public:
  explicit Init_Usbl_position(::message2::msg::Usbl & msg)
  : msg_(msg)
  {}
  Init_Usbl_beacon_id position(::message2::msg::Usbl::_position_type arg)
  {
    msg_.position = std::move(arg);
    return Init_Usbl_beacon_id(msg_);
  }

private:
  ::message2::msg::Usbl msg_;
};

class Init_Usbl_roll
{
public:
  explicit Init_Usbl_roll(::message2::msg::Usbl & msg)
  : msg_(msg)
  {}
  Init_Usbl_position roll(::message2::msg::Usbl::_roll_type arg)
  {
    msg_.roll = std::move(arg);
    return Init_Usbl_position(msg_);
  }

private:
  ::message2::msg::Usbl msg_;
};

class Init_Usbl_pitch
{
public:
  explicit Init_Usbl_pitch(::message2::msg::Usbl & msg)
  : msg_(msg)
  {}
  Init_Usbl_roll pitch(::message2::msg::Usbl::_pitch_type arg)
  {
    msg_.pitch = std::move(arg);
    return Init_Usbl_roll(msg_);
  }

private:
  ::message2::msg::Usbl msg_;
};

class Init_Usbl_yaw
{
public:
  explicit Init_Usbl_yaw(::message2::msg::Usbl & msg)
  : msg_(msg)
  {}
  Init_Usbl_pitch yaw(::message2::msg::Usbl::_yaw_type arg)
  {
    msg_.yaw = std::move(arg);
    return Init_Usbl_pitch(msg_);
  }

private:
  ::message2::msg::Usbl msg_;
};

class Init_Usbl_depth
{
public:
  explicit Init_Usbl_depth(::message2::msg::Usbl & msg)
  : msg_(msg)
  {}
  Init_Usbl_yaw depth(::message2::msg::Usbl::_depth_type arg)
  {
    msg_.depth = std::move(arg);
    return Init_Usbl_yaw(msg_);
  }

private:
  ::message2::msg::Usbl msg_;
};

class Init_Usbl_azimuth
{
public:
  explicit Init_Usbl_azimuth(::message2::msg::Usbl & msg)
  : msg_(msg)
  {}
  Init_Usbl_depth azimuth(::message2::msg::Usbl::_azimuth_type arg)
  {
    msg_.azimuth = std::move(arg);
    return Init_Usbl_depth(msg_);
  }

private:
  ::message2::msg::Usbl msg_;
};

class Init_Usbl_elevation
{
public:
  explicit Init_Usbl_elevation(::message2::msg::Usbl & msg)
  : msg_(msg)
  {}
  Init_Usbl_azimuth elevation(::message2::msg::Usbl::_elevation_type arg)
  {
    msg_.elevation = std::move(arg);
    return Init_Usbl_azimuth(msg_);
  }

private:
  ::message2::msg::Usbl msg_;
};

class Init_Usbl_range
{
public:
  explicit Init_Usbl_range(::message2::msg::Usbl & msg)
  : msg_(msg)
  {}
  Init_Usbl_elevation range(::message2::msg::Usbl::_range_type arg)
  {
    msg_.range = std::move(arg);
    return Init_Usbl_elevation(msg_);
  }

private:
  ::message2::msg::Usbl msg_;
};

class Init_Usbl_header
{
public:
  Init_Usbl_header()
  : msg_(::rosidl_runtime_cpp::MessageInitialization::SKIP)
  {}
  Init_Usbl_range header(::message2::msg::Usbl::_header_type arg)
  {
    msg_.header = std::move(arg);
    return Init_Usbl_range(msg_);
  }

private:
  ::message2::msg::Usbl msg_;
};

}  // namespace builder

}  // namespace msg

template<typename MessageType>
auto build();

template<>
inline
auto build<::message2::msg::Usbl>()
{
  return message2::msg::builder::Init_Usbl_header();
}

}  // namespace message2

#endif  // MESSAGE2__MSG__DETAIL__USBL__BUILDER_HPP_
