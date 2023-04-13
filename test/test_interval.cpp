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

#include <cadmium/iadevs/utils/ia_interval.h>

#include <catch.hpp>

SCENARIO("Basic operations with intervals", "[INTERVALS]") {
  GIVEN("an infinite integer interval") {
    cadmium::iadevs::interval<int, true> i{};
    WHEN("set to (inf-, inf+)") {
      i.lower_bound.set_inf();
      i.upper_bound.set_inf();
      THEN("observing it returns (inf-, inf+)") {
        REQUIRE(i.lower_bound.is_inf() == true);
        REQUIRE(i.lower_bound.is_closed() == false);
        REQUIRE(i.upper_bound.is_inf() == true);
        REQUIRE(i.upper_bound.is_closed() == false);
      }
    }WHEN("set to (5, inf+)") {
      i.lower_bound.set_value(5, false);
      i.upper_bound.set_inf();
      THEN("observing it returns (5, inf+)") {
        REQUIRE(i.lower_bound.is_inf() == false);
        REQUIRE(i.lower_bound.is_closed() == false);
        REQUIRE(i.lower_bound.get_value() == 5);
        REQUIRE(i.upper_bound.is_inf() == true);
        REQUIRE(i.upper_bound.is_closed() == false);
      }
    }WHEN("set to [1, 2)") {
      i.lower_bound.set_value(1, true);
      i.upper_bound.set_value(2, false);
      THEN("observing it returns [1, 2)") {
        REQUIRE(i.lower_bound.is_inf() == false);
        REQUIRE(i.lower_bound.is_closed() == true);
        REQUIRE(i.lower_bound.get_value() == 1);
        REQUIRE(i.upper_bound.is_inf() == false);
        REQUIRE(i.upper_bound.is_closed() == false);
        REQUIRE(i.upper_bound.get_value() == 2);
      }
    }WHEN("set to [1, 1]") {
      i.lower_bound.set_value(1, true);
      i.upper_bound.set_value(1, true);
      THEN("observing it returns [1, 2)") {
        REQUIRE(i.lower_bound.is_inf() == false);
        REQUIRE(i.lower_bound.is_closed() == true);
        REQUIRE(i.lower_bound.get_value() == 1);
        REQUIRE(i.upper_bound.is_inf() == false);
        REQUIRE(i.upper_bound.is_closed() == true);
        REQUIRE(i.upper_bound.get_value() == 1);
      }
    }
  }
}

SCENARIO("Basic errors with intervals", "[INTERVALS]") {
  GIVEN("an infinite integer interval") {
    cadmium::iadevs::interval<int, true> i{};
    WHEN("set to [0,0)") {
      i.lower_bound.set_value(0, true);
      i.upper_bound.set_value(0, false);
    }WHEN("set to (0,0)") {
      i.lower_bound.set_value(0, false);
      i.upper_bound.set_value(0, false);
    }WHEN("set to (1, 0)") {
      i.lower_bound.set_value(1, false);
      i.upper_bound.set_value(0, false);
    }
  }
}