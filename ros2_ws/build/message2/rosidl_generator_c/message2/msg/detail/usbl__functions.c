// generated from rosidl_generator_c/resource/idl__functions.c.em
// with input from message2:msg/Usbl.idl
// generated code does not contain a copyright notice
#include "message2/msg/detail/usbl__functions.h"

#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "rcutils/allocator.h"


// Include directives for member types
// Member `header`
#include "std_msgs/msg/detail/header__functions.h"
// Member `position`
#include "geometry_msgs/msg/detail/point__functions.h"

bool
message2__msg__Usbl__init(message2__msg__Usbl * msg)
{
  if (!msg) {
    return false;
  }
  // header
  if (!std_msgs__msg__Header__init(&msg->header)) {
    message2__msg__Usbl__fini(msg);
    return false;
  }
  // range
  // elevation
  // azimuth
  // depth
  // yaw
  // pitch
  // roll
  // position
  if (!geometry_msgs__msg__Point__init(&msg->position)) {
    message2__msg__Usbl__fini(msg);
    return false;
  }
  // beacon_id
  return true;
}

void
message2__msg__Usbl__fini(message2__msg__Usbl * msg)
{
  if (!msg) {
    return;
  }
  // header
  std_msgs__msg__Header__fini(&msg->header);
  // range
  // elevation
  // azimuth
  // depth
  // yaw
  // pitch
  // roll
  // position
  geometry_msgs__msg__Point__fini(&msg->position);
  // beacon_id
}

bool
message2__msg__Usbl__are_equal(const message2__msg__Usbl * lhs, const message2__msg__Usbl * rhs)
{
  if (!lhs || !rhs) {
    return false;
  }
  // header
  if (!std_msgs__msg__Header__are_equal(
      &(lhs->header), &(rhs->header)))
  {
    return false;
  }
  // range
  if (lhs->range != rhs->range) {
    return false;
  }
  // elevation
  if (lhs->elevation != rhs->elevation) {
    return false;
  }
  // azimuth
  if (lhs->azimuth != rhs->azimuth) {
    return false;
  }
  // depth
  if (lhs->depth != rhs->depth) {
    return false;
  }
  // yaw
  if (lhs->yaw != rhs->yaw) {
    return false;
  }
  // pitch
  if (lhs->pitch != rhs->pitch) {
    return false;
  }
  // roll
  if (lhs->roll != rhs->roll) {
    return false;
  }
  // position
  if (!geometry_msgs__msg__Point__are_equal(
      &(lhs->position), &(rhs->position)))
  {
    return false;
  }
  // beacon_id
  if (lhs->beacon_id != rhs->beacon_id) {
    return false;
  }
  return true;
}

bool
message2__msg__Usbl__copy(
  const message2__msg__Usbl * input,
  message2__msg__Usbl * output)
{
  if (!input || !output) {
    return false;
  }
  // header
  if (!std_msgs__msg__Header__copy(
      &(input->header), &(output->header)))
  {
    return false;
  }
  // range
  output->range = input->range;
  // elevation
  output->elevation = input->elevation;
  // azimuth
  output->azimuth = input->azimuth;
  // depth
  output->depth = input->depth;
  // yaw
  output->yaw = input->yaw;
  // pitch
  output->pitch = input->pitch;
  // roll
  output->roll = input->roll;
  // position
  if (!geometry_msgs__msg__Point__copy(
      &(input->position), &(output->position)))
  {
    return false;
  }
  // beacon_id
  output->beacon_id = input->beacon_id;
  return true;
}

message2__msg__Usbl *
message2__msg__Usbl__create()
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  message2__msg__Usbl * msg = (message2__msg__Usbl *)allocator.allocate(sizeof(message2__msg__Usbl), allocator.state);
  if (!msg) {
    return NULL;
  }
  memset(msg, 0, sizeof(message2__msg__Usbl));
  bool success = message2__msg__Usbl__init(msg);
  if (!success) {
    allocator.deallocate(msg, allocator.state);
    return NULL;
  }
  return msg;
}

void
message2__msg__Usbl__destroy(message2__msg__Usbl * msg)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  if (msg) {
    message2__msg__Usbl__fini(msg);
  }
  allocator.deallocate(msg, allocator.state);
}


bool
message2__msg__Usbl__Sequence__init(message2__msg__Usbl__Sequence * array, size_t size)
{
  if (!array) {
    return false;
  }
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  message2__msg__Usbl * data = NULL;

  if (size) {
    data = (message2__msg__Usbl *)allocator.zero_allocate(size, sizeof(message2__msg__Usbl), allocator.state);
    if (!data) {
      return false;
    }
    // initialize all array elements
    size_t i;
    for (i = 0; i < size; ++i) {
      bool success = message2__msg__Usbl__init(&data[i]);
      if (!success) {
        break;
      }
    }
    if (i < size) {
      // if initialization failed finalize the already initialized array elements
      for (; i > 0; --i) {
        message2__msg__Usbl__fini(&data[i - 1]);
      }
      allocator.deallocate(data, allocator.state);
      return false;
    }
  }
  array->data = data;
  array->size = size;
  array->capacity = size;
  return true;
}

void
message2__msg__Usbl__Sequence__fini(message2__msg__Usbl__Sequence * array)
{
  if (!array) {
    return;
  }
  rcutils_allocator_t allocator = rcutils_get_default_allocator();

  if (array->data) {
    // ensure that data and capacity values are consistent
    assert(array->capacity > 0);
    // finalize all array elements
    for (size_t i = 0; i < array->capacity; ++i) {
      message2__msg__Usbl__fini(&array->data[i]);
    }
    allocator.deallocate(array->data, allocator.state);
    array->data = NULL;
    array->size = 0;
    array->capacity = 0;
  } else {
    // ensure that data, size, and capacity values are consistent
    assert(0 == array->size);
    assert(0 == array->capacity);
  }
}

message2__msg__Usbl__Sequence *
message2__msg__Usbl__Sequence__create(size_t size)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  message2__msg__Usbl__Sequence * array = (message2__msg__Usbl__Sequence *)allocator.allocate(sizeof(message2__msg__Usbl__Sequence), allocator.state);
  if (!array) {
    return NULL;
  }
  bool success = message2__msg__Usbl__Sequence__init(array, size);
  if (!success) {
    allocator.deallocate(array, allocator.state);
    return NULL;
  }
  return array;
}

void
message2__msg__Usbl__Sequence__destroy(message2__msg__Usbl__Sequence * array)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  if (array) {
    message2__msg__Usbl__Sequence__fini(array);
  }
  allocator.deallocate(array, allocator.state);
}

bool
message2__msg__Usbl__Sequence__are_equal(const message2__msg__Usbl__Sequence * lhs, const message2__msg__Usbl__Sequence * rhs)
{
  if (!lhs || !rhs) {
    return false;
  }
  if (lhs->size != rhs->size) {
    return false;
  }
  for (size_t i = 0; i < lhs->size; ++i) {
    if (!message2__msg__Usbl__are_equal(&(lhs->data[i]), &(rhs->data[i]))) {
      return false;
    }
  }
  return true;
}

bool
message2__msg__Usbl__Sequence__copy(
  const message2__msg__Usbl__Sequence * input,
  message2__msg__Usbl__Sequence * output)
{
  if (!input || !output) {
    return false;
  }
  if (output->capacity < input->size) {
    const size_t allocation_size =
      input->size * sizeof(message2__msg__Usbl);
    rcutils_allocator_t allocator = rcutils_get_default_allocator();
    message2__msg__Usbl * data =
      (message2__msg__Usbl *)allocator.reallocate(
      output->data, allocation_size, allocator.state);
    if (!data) {
      return false;
    }
    // If reallocation succeeded, memory may or may not have been moved
    // to fulfill the allocation request, invalidating output->data.
    output->data = data;
    for (size_t i = output->capacity; i < input->size; ++i) {
      if (!message2__msg__Usbl__init(&output->data[i])) {
        // If initialization of any new item fails, roll back
        // all previously initialized items. Existing items
        // in output are to be left unmodified.
        for (; i-- > output->capacity; ) {
          message2__msg__Usbl__fini(&output->data[i]);
        }
        return false;
      }
    }
    output->capacity = input->size;
  }
  output->size = input->size;
  for (size_t i = 0; i < input->size; ++i) {
    if (!message2__msg__Usbl__copy(
        &(input->data[i]), &(output->data[i])))
    {
      return false;
    }
  }
  return true;
}
