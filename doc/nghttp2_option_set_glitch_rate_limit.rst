
nghttp2_option_set_glitch_rate_limit
====================================

Synopsis
--------

*#include <nghttp2/nghttp2.h>*

.. function:: void nghttp2_option_set_glitch_rate_limit(nghttp2_option *option, uint64_t burst, uint64_t rate)

    
    This function sets the rate limit for the "glitches", the
    suspicious activities from a remote endpoint.  It is a token-bucket
    based rate limiter.  *burst* specifies the number of tokens that is
    initially available.  The maximum number of tokens is capped to
    this value.  *rate* specifies the number of tokens that are
    regenerated per second.  When a suspicious activity is detected,
    some amount of tokens are consumed.  If there is no token
    available, GOAWAY is sent to tear down the connection.  *burst* and
    *rate* default to 1000 and 33 respectively.
