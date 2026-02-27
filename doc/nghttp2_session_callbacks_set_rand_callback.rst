
nghttp2_session_callbacks_set_rand_callback
===========================================

Synopsis
--------

*#include <nghttp2/nghttp2.h>*

.. function:: void nghttp2_session_callbacks_set_rand_callback( nghttp2_session_callbacks *cbs, nghttp2_rand_callback rand_callback)

    
    Sets callback function invoked when unpredictable data is needed.
    Although this callback is optional due to the backward
    compatibility, it is recommended to specify it to harden the
    runtime behavior against suspicious activities of a remote
    endpoint.
