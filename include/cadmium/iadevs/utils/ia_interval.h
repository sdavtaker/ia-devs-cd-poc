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
 * The order and bound functions are required to apply over domain_t values only.
 */
#include <concepts>
#include <stdexcept>

namespace cadmium::iadevs {

template<typename T>
struct bound {
  void set_value(T value, bool closed) {
    _value = value;
    _closed = closed;
  }
  [[nodiscard]] bool is_closed() const {
    return _closed;
  }

  T get_value() const {
    if (is_inf()) {
      throw std::out_of_range("Getting finite value from infinite bound");
    }
    return bound<T>::_value;
  }

  [[nodiscard]] bool is_inf() const {
    return _inf;
  }

  void set_inf() {
    _inf = true;
    _closed = false;
  }

private:
  bool _closed;
  bool _inf;
  T _value;
};

/**
 * Simple representation of an interval
 * In this context: left unbounded interval is an interval starting in -\inf with a finite upper bound;
 * right unbounded interval is an interval starting in a finite lower bound and extending to +\inf;
 * empty interval is an interval with no elements, and unbounded interval is the (-\inf, \inf) interval.
 * @tparam domain_t
 */
template<typename domain_t> requires std::totally_ordered<domain_t>
struct interval {
  using domain_bound_t = bound<domain_t>;
  /**
   * @return Is this a interval empty of elements?
   */
  [[nodiscard]] bool is_empty() const {
    return _empty;
  }

  /**
   * @return Is this interval lacking of a finite upper bound?
   */
  [[nodiscard]] bool is_right_unbounded() const {
    return is_bound_inf(_upper_bound);
  }

  /**
   * @return Is this interval lacking of a finite lower bound?
   */
  [[nodiscard]] bool is_left_unbounded() const {
    return is_bound_inf(_lower_bound);
  }

  /**
   * @return Is this interval unbounded at both ends?
   */
  [[nodiscard]] bool is_unbounded() const {
    return is_left_unbounded() && is_right_unbounded();
  }

  /**
   * @return Is this interval upper endpoint closed?
   */
  [[nodiscard]] bool is_upper_endpoint_closed() const {
    return is_bound_closed(_upper_bound);
  }

  /**
   * @return Is this interval lower endpoint closed?
   */
  [[nodiscard]] bool is_lower_endpoint_closed() const {
    return is_bound_closed(_lower_bound);
  }

  /**
   * @return lower endpoint finite value
   */
  domain_t get_lower_endpoint_value() const {
    return get_bound_value(_lower_bound);
  }

  /**
   * @return upper endpoint finite value
   */
  domain_t get_upper_endpoint_value() {
    return get_bound_value(_upper_bound);
  }

  /**
   * Set the interval as empty, including no elements
   */
  void set_empty() {
    _lower_bound.set_value(0, false);
    _upper_bound.set_value(0, false);
  }

  /**
   * Set the interval to have no lower bound, and a finite upper endpoint
   * @param value the upper endpoint
   * @param closed if the upper endpoint is itself included
   */
  void set_left_unbounded_with_upper_endpoint_value(domain_t value, bool closed) {
    set_finite_bound(_upper_bound, value, closed);
    set_infinite_bound(_lower_bound);
  }

  /**
   * Set the interval to have no upper bound, and a finite lower endpoint
   * @param value the upper endpoint
   * @param closed if the lower endpoint is itself included
   */
  void set_right_unbounded_with_lower_endpoint_value(domain_t value, bool closed) {
    set_finite_bound(_lower_bound, value, closed);
    set_infinite_bound(_upper_bound);
  }
  /**
   * Set the interval to have both endpoints with finite values
   * @param lower_value the lower bound value
   * @param lower_closed is the lower bound closed?
   * @param upper_value the upper bound value
   * @param upper_closed is the upper bound closed?
   */
  void set_bounded(domain_t lower_value, bool lower_closed, domain_t upper_value, bool upper_closed) {
    if (lower_value == upper_value && (!lower_closed || !upper_closed)) {
      throw std::domain_error("There value cannot be included and excluded at the same time");
    }
    if (upper_value < lower_value) {
      throw std::domain_error("Upper endpoint has to be grater than lower endpoint");
    }
    set_finite_bound(_lower_bound, lower_value, lower_closed);
    set_finite_bound(_upper_bound, upper_value, upper_closed);
  }
  /**
   * Set the interval to have no lower and no upper bounds effectively including all elements
   */
  void set_unbounded() {
    set_infinite_bound(_lower_bound);
    set_infinite_bound(_upper_bound);
  }

  //TODO: add comparison and arithmetic operations
private:
  bool _empty = true;
  domain_bound_t _lower_bound;
  domain_bound_t _upper_bound;

  bool is_bound_inf(const domain_bound_t &bound) const {
    return bound.is_inf();
  }
  bool is_bound_closed(const domain_bound_t &bound) const {
    return bound.is_closed();
  }
  domain_t get_bound_value(const domain_bound_t &bound) const {
    if (_empty) {
      throw std::out_of_range("There is no value on empty intervals");
    }
    return bound.get_value();
  }

  void set_finite_bound(domain_bound_t &bound, domain_t value, bool closed) {
    _empty = false;
    bound.set_value(value, closed);
  }
  void set_infinite_bound(domain_bound_t &bound) {
    _empty = false;
    bound.set_inf();
  }
};
}