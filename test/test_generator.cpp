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

#include <cadmium/iadevs/basic_models/generator.h>

#include <catch.hpp>

SCENARIO("Generator basic model TA_I function", "[GENERATOR]") {
  GIVEN("A generator that outputs a 1 or 2 every 997 to 1005 millisecond") {
    WARN("Current test assumes time is in milliseconds, units are not yet implemented");
    cadmium::iadevs::basic_models::generator g{};
    WHEN("bounded_time_advance_i is called with q_state ([0, 0], [0, 0])") {
      cadmium::iadevs::basic_models::generator::state_t s{};
      s.set_bounded(0, true, 0, true);
      auto i = g.bounded_time_advance_i(s);
      THEN("interval [997, 1005] is returned") {
        REQUIRE_FALSE(i.is_empty());
        REQUIRE_FALSE(i.is_left_unbounded());
        REQUIRE_FALSE(i.is_right_unbounded());
        REQUIRE_FALSE(i.is_unbounded());
        REQUIRE(i.is_lower_endpoint_closed());
        REQUIRE(i.is_upper_endpoint_closed());
        REQUIRE(i.get_lower_endpoint_value() == 997);
        REQUIRE(i.get_upper_endpoint_value() == 1005);
      }
    }
  }
}

SCENARIO("Generator basic model time_bound_add function", "[GENERATOR]") {
  GIVEN("A generator") {
    WARN("Current test assumes time is in milliseconds, units are not yet implemented");
    cadmium::iadevs::basic_models::generator g{};
    WHEN("time_bound_add [997, 1005] to  [997, 1005]") {
      cadmium::iadevs::basic_models::generator::time_t t1{};
      t1.set_bounded(997, true, 1005, true);
      auto i = g.time_bound_add(t1, t1);
      THEN("interval [1994, 2010] is returned") {
        REQUIRE_FALSE(i.is_empty());
        REQUIRE_FALSE(i.is_left_unbounded());
        REQUIRE_FALSE(i.is_right_unbounded());
        REQUIRE_FALSE(i.is_unbounded());
        REQUIRE(i.is_lower_endpoint_closed());
        REQUIRE(i.is_upper_endpoint_closed());
        REQUIRE(i.get_lower_endpoint_value() == 1994);
        REQUIRE(i.get_upper_endpoint_value() == 2010);
      }
    }
  }
}
