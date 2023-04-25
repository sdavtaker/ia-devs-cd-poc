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

#include <cadmium/iadevs/utils/ia_interval.h>

#include<string>

namespace cadmium::iadevs::basic_models {
/**
 * This is a simple implementation of a UA Generator model approximated
 * by using double intervals.
 * Determinism on the IA-DEVS defined functions is expected, but mutability
 * is allowed for tracing and other orthogonal concerns. The call to any of
 * the functions MAY be cached by the simulator.
 * All IA functions have to return bounded results, including TA to be computable.
 */
struct generator {
  // The state is only tracks the time until next internal event is emitted
  using state_t = cadmium::iadevs::interval<int>;
  // The time is defined as a set of integers representing ms (no unit support yet)
  using time_t = cadmium::iadevs::interval<int>;
  //At this point I'm only implementing what is required to make Simulator.init
  //function work end to end.
  //TODO: add everything else.
  //Ref: UA-DEVS paper section 6.1
  //Function init(initstate q, R^+_I t)→ void
  //state =q_state
  //t_last = (t − q_time ).apply(timebounds)
  //t_next = (t_last + a.TA(state)).apply(timebounds)

  /**
   * Bounded_time_advance_i is an optional function
   * Depending on the datatype used for time_t bounding time_advance and
   * reapplying bound after adding/subtracting t_next/t_last at the simulator
   * level can add extra error to the obtained time interval.
   * However, in cases the error is acceptable, this function can be accompanied
   * by deserialize/serialize time_advance functions to cache its calculation and
   * speed up the simulation.
   * @param state is the interval of partial states for the time_advance calculation
   * @return
   */
  time_t bounded_time_advance_i(const state_t &state) const {
    time_t l{};
    l.set_bounded(997, true, 1005, true);
    return l - state;
  }

  // The simulator needs to call this function to apply the proper bounded addition when
  time_t time_bound_t_add_time_advance(const state_t &state, const time_t &t) const {
    return time_bound_add(t, bounded_time_advance_i(state));
  }

  time_t time_bound_add(const time_t &t1, const time_t &t2) const {
    return t1 + t2;
  }

  time_t time_bound_t_subtract_time_advance(const time_t &t1, const time_t &t2) const {
    return t1 - t2;
  }

  //serialize : to enable caching
  std::string serialize_time_interval(const time_t &t) const {
    return "[0,0]";
  }

  //deserialize : to enable cache read
  time_t deserialize_time_interval(const std::string &t) const {
    return time_t{};
  }
};
}
