/**
 * Copyright (c) 2023, Damian Vicino
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 * 1. Redistributions of source code must retain the above copyright notice,
 * this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following disclaimer in the documentation
 * and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#pragma once

/**
 * Definition of a generic interval struct to use in IA models definition.
 * The order and bound functions are required to apply over bound_image_t values only.
 */
#include <cassert>
#include <concepts>
#include <limits>
namespace cadmium::iadevs {

template<typename T>
struct bound {
  void set_value(T value, bool closed) {
    _value = value;
    _closed = closed;
  }
  void set_inf() {
    _value = std::numeric_limits<T>::max();
    _closed = false;
  }
  bool is_closed() {
    return _closed;
  }

  T get_value() {
    return bound<T>::_value;
  }

protected:
  bool _closed;
  T _value;
};

// Reserving max _value as infinite representation, check for infinity before
// comparing values is on user to do.
template<typename T>
struct bound_with_inf : public bound<T> {
  bool is_inf() {
    return std::numeric_limits<T>::max() == bound<T>::_value;
  }

  T get_value() {
    assert(!is_inf());
    return bound<T>::_value;
  }
};

#include <concepts>

template<typename bound_image_t, bool allow_infinity = true> requires std::totally_ordered<bound_image_t>
struct interval {
  using bounded_image_t = std::conditional_t<allow_infinity, bound_with_inf<bound_image_t>, bound<bound_image_t>>;
  bounded_image_t lower_bound;
  bounded_image_t upper_bound;
  bool operator<(const interval &rhs) {
    return this->lower_bound._value < rhs.lower_bound._value;
  }
};
}

