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

#include <cadmium/iadevs/concepts.h>

namespace cadmium::iadevs::engine {

/**
 * sim_state_triplet wraps state, t_next, t_last to be passed back to coordinators
 * @tparam STATE
 * @tparam TIME
 */
template<typename STATE, typename TIME>
struct sim_state_triplet {
  STATE state;
  TIME t_last;
  TIME t_next;
};

/**
 * This implements the simulation algorithms for IA-DEVS atomic models
 * @tparam model_t an atomic IA model
 */
template<typename model_t> requires cadmium::iadevs::is_atomic<model_t>
struct simulator {
  using sim_state_t = sim_state_triplet<typename model_t::state_t, typename model_t::time_t>;

  sim_state_t init(typename model_t::state_t state, typename model_t::time_t time) {
    model_t m;
    auto time_advance = m.bounded_time_advance_i(state);
    auto t_next = m.time_bound_add(time, time_advance);
    return sim_state_t{state, time, t_next};
  }
};
}