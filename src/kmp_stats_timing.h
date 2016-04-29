#ifndef KMP_STATS_TIMING_H
#define KMP_STATS_TIMING_H

/** @file kmp_stats_timing.h
 * Access to real time clock and timers.
 */

/* <copyright>
    Copyright (c) 1997-2016 Intel Corporation.  All Rights Reserved.

    Redistribution and use in source and binary forms, with or without
    modification, are permitted provided that the following conditions
    are met:

      * Redistributions of source code must retain the above copyright
        notice, this list of conditions and the following disclaimer.
      * Redistributions in binary form must reproduce the above copyright
        notice, this list of conditions and the following disclaimer in the
        documentation and/or other materials provided with the distribution.
      * Neither the name of Intel Corporation nor the names of its
        contributors may be used to endorse or promote products derived
        from this software without specific prior written permission.

    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
    "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
    LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
    A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
    HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
    SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
    LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
    DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
    THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
    (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
    OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

</copyright> */


#include <stdint.h>
#include <string>
#include <limits>
#include "kmp_os.h"

class tsc_tick_count {
  private:
    int64_t my_count;

  public:
    class tsc_interval_t {
        int64_t value;
        explicit tsc_interval_t(int64_t _value) : value(_value) {}
     public:
        tsc_interval_t() : value(0) {}; // Construct 0 time duration
        double seconds() const; // Return the length of a time interval in seconds
        double ticks() const { return double(value); }
        int64_t getValue() const { return value; }

        friend class tsc_tick_count;

        friend tsc_interval_t operator-(
        const tsc_tick_count t1, const tsc_tick_count t0);
    };

    tsc_tick_count() : my_count(static_cast<int64_t>(__rdtsc())) {};
    tsc_tick_count(int64_t value) : my_count(value) {};
    int64_t getValue() const { return my_count; }
    tsc_tick_count later (tsc_tick_count const other) const {
        return my_count > other.my_count ? (*this) : other;
    }
    tsc_tick_count earlier(tsc_tick_count const other) const {
        return my_count < other.my_count ? (*this) : other;
    }
    static double tick_time(); // returns seconds per cycle (period) of clock
    static tsc_tick_count now() { return tsc_tick_count(); } // returns the rdtsc register value
    friend tsc_tick_count::tsc_interval_t operator-(const tsc_tick_count t1, const tsc_tick_count t0);
};

inline tsc_tick_count::tsc_interval_t operator-(const tsc_tick_count t1, const tsc_tick_count t0)
{
    return tsc_tick_count::tsc_interval_t( t1.my_count-t0.my_count );
}

inline double tsc_tick_count::tsc_interval_t::seconds() const
{
    return value*tick_time();
}

extern std::string formatSI(double interval, int width, char unit);

inline std::string formatSeconds(double interval, int width)
{
    return formatSI(interval, width, 'S');
}

inline std::string formatTicks(double interval, int width)
{
    return formatSI(interval, width, 'T');
}

class timePair
{
    tsc_tick_count KMP_ALIGN_CACHE start;
    tsc_tick_count end;

public:
    timePair() : start(-std::numeric_limits<int64_t>::max()), end(-std::numeric_limits<int64_t>::max()) {}
    tsc_tick_count get_start() const { return start; }
    tsc_tick_count get_end()   const { return end; }
    tsc_tick_count * get_startp()    { return &start; }
    tsc_tick_count * get_endp()      { return &end; }

    void markStart() { start = tsc_tick_count::now(); }
    void markEnd()   { end   = tsc_tick_count::now(); }
    void set_start(tsc_tick_count s) { start = s; }
    void set_end  (tsc_tick_count e) { end = e; }

    tsc_tick_count::tsc_interval_t duration() const { return end-start; }
    std::string format() const;

};

extern tsc_tick_count::tsc_interval_t computeLastInLastOutInterval(timePair * times, int nTimes);
#endif // KMP_STATS_TIMING_H
