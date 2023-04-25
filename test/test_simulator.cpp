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
#include <cadmium/iadevs/engine/simulator.h>

#include <catch.hpp>

SCENARIO("Generator is simulated", "[SIMULATOR]") {
  GIVEN("A simulator for a generator model") {
    WARN("Current test assumes time is in milliseconds, units are not yet implemented");
    cadmium::iadevs::engine::simulator<cadmium::iadevs::basic_models::generator> sg{};
    WHEN("init is called with [0,0] initial S and [0, 0] initial T") {
      cadmium::iadevs::basic_models::generator::time_t t{};
      t.set_bounded(0, true, 0, true);
      cadmium::iadevs::basic_models::generator::state_t s{};
      s.set_bounded(0, true, 0, true);
      auto sg_init_ret = sg.init(s, t);
      THEN("same state and t_last is returned, t_next is now [997, 1005]") {
        cadmium::iadevs::basic_models::generator::time_t t_next_expected{};
        t_next_expected.set_bounded(997, true, 1005, true);
        REQUIRE(sg_init_ret.state == s);
        REQUIRE(sg_init_ret.t_last == t);
        REQUIRE(sg_init_ret.t_next == t_next_expected);
      }
    }
  }
}