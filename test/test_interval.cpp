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
    cadmium::iadevs::interval<int> i{};
    WHEN("the intervals is created") {
      THEN("the interval is empty") {
        REQUIRE(i.is_empty());
        REQUIRE_FALSE(i.is_left_unbounded());
        REQUIRE_FALSE(i.is_right_unbounded());
        REQUIRE_FALSE(i.is_unbounded());
        REQUIRE_FALSE(i.is_lower_endpoint_closed());
        REQUIRE_FALSE(i.is_upper_endpoint_closed());
      } AND_THEN("get_value throws out of range") {
        REQUIRE_THROWS_AS(i.get_upper_endpoint_value(), std::out_of_range);
      }
    }WHEN("set to (inf-, inf+)") {
      i.set_unbounded();
      THEN("observing it returns (inf-, inf+)") {
        REQUIRE_FALSE(i.is_empty());
        REQUIRE(i.is_left_unbounded());
        REQUIRE(i.is_right_unbounded());
        REQUIRE(i.is_unbounded());
        REQUIRE_FALSE(i.is_lower_endpoint_closed());
        REQUIRE_FALSE(i.is_upper_endpoint_closed());
        AND_WHEN("assigning a new empty interval") {
          i = cadmium::iadevs::interval<int>{};
          THEN("the interval is again empty") {
            REQUIRE(i.is_empty());
          }
        }
      }
    }WHEN("set to (5, inf+)") {
      i.set_right_unbounded_with_lower_endpoint_value(5, false);
      THEN("observing it returns (5, inf+)") {
        REQUIRE_FALSE(i.is_empty());
        REQUIRE_FALSE(i.is_left_unbounded());
        REQUIRE(i.is_right_unbounded());
        REQUIRE_FALSE(i.is_unbounded());
        REQUIRE_FALSE(i.is_lower_endpoint_closed());
        REQUIRE_FALSE(i.is_upper_endpoint_closed());
        REQUIRE(i.get_lower_endpoint_value() == 5);
      } AND_THEN("getting the value of the inf+ bound throws runtime exception") {
        REQUIRE_THROWS_AS(i.get_upper_endpoint_value(), std::out_of_range);
      }
    }WHEN("set to (inf-, 4]") {
      i.set_left_unbounded_with_upper_endpoint_value(4, true);
      THEN("observing it returns (inf-, 4]") {
        REQUIRE_FALSE(i.is_empty());
        REQUIRE(i.is_left_unbounded());
        REQUIRE_FALSE(i.is_right_unbounded());
        REQUIRE_FALSE(i.is_unbounded());
        REQUIRE_FALSE(i.is_lower_endpoint_closed());
        REQUIRE(i.is_upper_endpoint_closed());
        REQUIRE(i.get_upper_endpoint_value() == 4);
      } AND_THEN("getting the value of the inf- bound throws runtime exception") {
        REQUIRE_THROWS_AS(i.get_lower_endpoint_value(), std::out_of_range);
      }
    }WHEN("set to [1, 2)") {
      i.set_bounded(1, true, 2, false);
      THEN("observing it returns [1, 2)") {
        REQUIRE_FALSE(i.is_empty());
        REQUIRE_FALSE(i.is_left_unbounded());
        REQUIRE_FALSE(i.is_right_unbounded());
        REQUIRE_FALSE(i.is_unbounded());
        REQUIRE(i.is_lower_endpoint_closed());
        REQUIRE_FALSE(i.is_upper_endpoint_closed());
        REQUIRE(i.get_lower_endpoint_value() == 1);
        REQUIRE(i.get_upper_endpoint_value() == 2);
      }
    }WHEN("set to [1, 1]") {
      i.set_bounded(1, true, 1, true);
      THEN("observing it returns [1, 2)") {
        REQUIRE_FALSE(i.is_empty());
        REQUIRE_FALSE(i.is_left_unbounded());
        REQUIRE_FALSE(i.is_right_unbounded());
        REQUIRE_FALSE(i.is_unbounded());
        REQUIRE(i.is_lower_endpoint_closed());
        REQUIRE(i.is_upper_endpoint_closed());
        REQUIRE(i.get_lower_endpoint_value() == 1);
        REQUIRE(i.get_upper_endpoint_value() == 1);
      }
    }
  }
}

SCENARIO("Basic errors with intervals", "[INTERVALS]") {
  GIVEN("an infinite integer interval") {
    cadmium::iadevs::interval<int> i{};
    WHEN("set to [0,0)") {
      REQUIRE_THROWS_AS(i.set_bounded(0, true, 0, false), std::domain_error);
    }WHEN("set to (0,0)") {
      REQUIRE_THROWS_AS(i.set_bounded(0, false, 0, false), std::domain_error);
    }WHEN("set to (1, 0)") {
      REQUIRE_THROWS_AS(i.set_bounded(1, false, 0, false), std::domain_error);
    }
  }
}

SCENARIO("Intervals additions", "[INTERVALS]") {
  GIVEN("two intervals") {
    cadmium::iadevs::interval<int> i{};
    cadmium::iadevs::interval<int> j{};
    WHEN("an interval is empty") {
      j.set_bounded(1, false, 3, true);
      THEN("their addition is out of the domain") {
        REQUIRE_THROWS_AS(i + j, std::domain_error);
      }
    }WHEN("both intervals are unbounded") {
      i.set_unbounded();
      j.set_unbounded();
      THEN("their addition is unbounded") {
        auto k = i + j;
        REQUIRE(k.is_unbounded());
      }
    }WHEN("an interval is unbounded") {
      i.set_unbounded();
      j.set_bounded(2, false, 3, true);
      THEN("their addition is unbounded") {
        auto k = i + j;
        REQUIRE(k.is_unbounded());
      }
    }WHEN("both intervals are bounded and all endpoints closed") {
      i.set_bounded(1, true, 2, true);
      j.set_bounded(5, true, 7, true);
      THEN("their addition is bounded and the endpoints closed") {
        auto k = i + j;
        REQUIRE(k.is_lower_endpoint_closed());
        REQUIRE(k.is_upper_endpoint_closed());
        REQUIRE(k.get_lower_endpoint_value() == 6);
        REQUIRE(k.get_upper_endpoint_value() == 9);
      }
    } WHEN("an interval is left unbounded") {
      i.set_bounded(1, true, 2, true);
      j.set_left_unbounded_with_upper_endpoint_value(7, true);
      THEN("their addition is left unbounded") {
        auto k = i + j;
        REQUIRE(k.is_left_unbounded());
        REQUIRE(k.is_upper_endpoint_closed());
        REQUIRE(k.get_upper_endpoint_value() == 9);
      }
    }WHEN("an interval is right unbounded") {
      i.set_bounded(1, true, 2, true);
      j.set_right_unbounded_with_lower_endpoint_value(7, true);
      THEN("their addition is right unbounded") {
        auto k = i + j;
        REQUIRE(k.is_right_unbounded());
        REQUIRE(k.is_lower_endpoint_closed());
        REQUIRE(k.get_lower_endpoint_value() == 8);
      }
    }WHEN("an interval endpoint is open") {
      i.set_bounded(1, false, 2, true);
      j.set_bounded(5, true, 7, false);
      THEN("their addition matching endpoints are open") {
        auto k = i + j;
        REQUIRE_FALSE(k.is_lower_endpoint_closed());
        REQUIRE_FALSE(k.is_upper_endpoint_closed());
        REQUIRE(k.get_lower_endpoint_value() == 6);
        REQUIRE(k.get_upper_endpoint_value() == 9);
      }
    }WHEN("both intervals endpoints are open") {
      i.set_bounded(1, false, 2, false);
      j.set_bounded(5, false, 7, false);
      THEN("their addition matching endpoints are open") {
        auto k = i + j;
        REQUIRE_FALSE(k.is_lower_endpoint_closed());
        REQUIRE_FALSE(k.is_upper_endpoint_closed());
        REQUIRE(k.get_lower_endpoint_value() == 6);
        REQUIRE(k.get_upper_endpoint_value() == 9);
      }
    }
  }
}

SCENARIO("Equality of intervals", "[INTERVALS]") {
  GIVEN("an empty interval") {
    cadmium::iadevs::interval<int> i{};
    WHEN("comparing to a empty interval") {
      cadmium::iadevs::interval<int> j{};
      THEN("they are seen as equal") {
        REQUIRE(i == j);
      }
    }WHEN("comparing to an unbounded interval") {
      cadmium::iadevs::interval<int> j{};
      j.set_unbounded();
      THEN("they are considered not equal") {
        REQUIRE_FALSE(i == j);
      }
    }WHEN("comparing to a bounded interval [1, 2)") {
      cadmium::iadevs::interval<int> j{};
      j.set_bounded(1, true, 2, false);
      THEN("they are considered not equal") {
        REQUIRE_FALSE(i == j);
      }
    }
  }GIVEN("a unbounded interval") {
    cadmium::iadevs::interval<int> i{};
    i.set_unbounded();
    WHEN("comparing to an unbounded interval") {
      cadmium::iadevs::interval<int> j{};
      j.set_unbounded();
      THEN("they are considered not equal") {
        REQUIRE(i == j);
      }
    }WHEN("comparing to a bounded interval [1, 2)") {
      cadmium::iadevs::interval<int> j{};
      j.set_bounded(1, true, 2, false);
      THEN("they are considered not equal") {
        REQUIRE_FALSE(i == j);
      }
    }
  }GIVEN("a bounded interval [1, 2)") {
    cadmium::iadevs::interval<int> i{};
    i.set_bounded(1, true, 2, false);
    WHEN("comparing to a bounded interval [1, 2)") {
      cadmium::iadevs::interval<int> j{};
      j.set_bounded(1, true, 2, false);
      THEN("they are considered equal") {
        REQUIRE(i == j);
      }
    }WHEN("comparing to a bounded interval [1, 2]") {
      cadmium::iadevs::interval<int> j{};
      j.set_bounded(1, true, 2, true);
      THEN("they are not considered equal") {
        REQUIRE_FALSE(i == j);
      }
    }WHEN("comparing to a bounded interval (1, 2)") {
      cadmium::iadevs::interval<int> j{};
      j.set_bounded(1, false, 2, false);
      THEN("they are not considered equal") {
        REQUIRE_FALSE(i == j);
      }
    }WHEN("comparing to a bounded interval [1, 3)") {
      cadmium::iadevs::interval<int> j{};
      j.set_bounded(1, true, 3, false);
      THEN("they are not considered equal") {
        REQUIRE_FALSE(i == j);
      }
    }WHEN("comparing to a bounded interval [0, 2)") {
      cadmium::iadevs::interval<int> j{};
      j.set_bounded(0, true, 2, false);
      THEN("they are not considered equal") {
        REQUIRE_FALSE(i == j);
      }
    }WHEN("comparing to a semi bounded interval (inf-, 2)") {
      cadmium::iadevs::interval<int> j{};
      j.set_right_unbounded_with_lower_endpoint_value(2, false);
      THEN("they are not considered equal") {
        REQUIRE_FALSE(i == j);
      }
    }WHEN("comparing to a semi bounded interval [1, inf+)") {
      cadmium::iadevs::interval<int> j{};
      j.set_left_unbounded_with_upper_endpoint_value(1, true);
      THEN("they are not considered equal") {
        REQUIRE_FALSE(i == j);
      }
    }
  }
}
