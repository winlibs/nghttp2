/*
 * nghttp2 - HTTP/2 C Library
 *
 * Copyright (c) 2026 nghttp2 contributors
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
 * LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
 * OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
 * WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */
#ifndef ERRORS_H
#define ERRORS_H

#ifdef HAVE_CONFIG_H
#  include <config.h>
#endif // defined(HAVE_CONFIG_H)

#include <string_view>
#include <format>

using namespace std::string_view_literals;

namespace nghttp2 {

enum class Error {
  // the generic errors that are not covered by more specific error
  // codes.
  INTERNAL,
  // function arguments are invalid
  INVALID_ARGUMENT,
  // integer overflow error
  INTEGER_OVERFLOW,
  // file I/O error
  IO,
  // function is not implemented yet
  NOT_IMPLEMENTED,
  // the operation or the function is not supported
  UNSUPPORTED,
  // file is not found
  FILE_NOT_FOUND,
  // crypto related error (e.g., error from TLS stack)
  CRYPTO,
  // system call error
  SYSCALL,
  // C library error (e.g., error from getaddrinfo)
  LIBC,
  // HTTP1 library error (e.g., error from llhttp API)
  HTTP1,
  // HTTP2 library error (e.g., error from nghttp2 API)
  HTTP2,
  // HTTP3 library error (e.g., error from nghttp3 API)
  HTTP3,
  // QUIC library error (e.g., error from ngtcp2 API)
  QUIC,
  // QUIC token is unreadable, and should be ignored.
  QUIC_UNREADABLE_TOKEN,
  // Verification of QUIC token failed.
  QUIC_VERIFY_TOKEN,
  // Network related error in general.
  NETWORK,
  // EOF is received from socket.
  RECV_EOF,
  // TLS handshake is in-progress.
  TLS_HANDSHAKE_INPROGRESS,
  // Downstream connection has been canceled.
  DCONN_CANCELED,
  // Downstream connection should be retried.
  DCONN_RETRY,
  // TLS is required.
  TLS_REQUIRED,
  // PEM type (e.g., PRIVATE KEY) is not expected one.
  INVALID_PEM_TYPE,
  // Entity is not found.
  ENTITY_NOT_FOUND,
  // Everything done for this connection.
  DONE,
  // TLS peer verification error.
  TLS_VERIFY_PEER,
  // Connection cannot be established.
  CONNECT_FAIL,
  // ALPN negotiation failure.
  ALPN,
  // No downstream address available.
  NO_AVAIL_DOWNSTREAM,
  // HTTP messaging violation.
  HTTP,
  // DNS related error.
  DNS,
  // WebSocket handshake failure.
  WEBSOCKET_HANDSHAKE,
  // PROXY-protocol error.
  PROXY_PROTO,
  // HTTP fields are too large.
  HTTP_FIELD_TOO_LARGE,
  // mruby library error (e.g., error from mruby API)
  MRUBY,
  // memcached protocol error.
  MEMCACHED,
  // BPF library error (e.g., error from libbpf API)
  BPF,
  // General configuration error.
  INVALID_CONFIG,
  // Stream creation is disallowed temporarily due to stream ID limit.
  STREAM_ID_BLOCKED,
};

} // namespace nghttp2

template <>
struct std::formatter<nghttp2::Error>
  : public std::formatter<std::string_view> {
  template <typename FormatContext>
  auto format(nghttp2::Error e, FormatContext &ctx) const {
    auto s = "unknown"sv;

    switch (e) {
    case nghttp2::Error::INTERNAL:
      s = "internal"sv;
      break;
    case nghttp2::Error::INVALID_ARGUMENT:
      s = "invalid argument"sv;
      break;
    case nghttp2::Error::INTEGER_OVERFLOW:
      s = "integer overflow"sv;
      break;
    case nghttp2::Error::IO:
      s = "I/O"sv;
      break;
    case nghttp2::Error::NOT_IMPLEMENTED:
      s = "not implemented"sv;
      break;
    case nghttp2::Error::UNSUPPORTED:
      s = "unsupported"sv;
      break;
    case nghttp2::Error::FILE_NOT_FOUND:
      s = "file not found"sv;
      break;
    case nghttp2::Error::CRYPTO:
      s = "crypto"sv;
      break;
    case nghttp2::Error::SYSCALL:
      s = "syscall"sv;
      break;
    case nghttp2::Error::LIBC:
      s = "libc"sv;
      break;
    case nghttp2::Error::HTTP1:
      s = "HTTP1"sv;
      break;
    case nghttp2::Error::HTTP2:
      s = "HTTP2"sv;
      break;
    case nghttp2::Error::HTTP3:
      s = "HTTP3"sv;
      break;
    case nghttp2::Error::QUIC:
      s = "QUIC"sv;
      break;
    case nghttp2::Error::QUIC_UNREADABLE_TOKEN:
      s = "QUIC unreadable token"sv;
      break;
    case nghttp2::Error::QUIC_VERIFY_TOKEN:
      s = "QUIC token verification error"sv;
      break;
    case nghttp2::Error::NETWORK:
      s = "network"sv;
      break;
    case nghttp2::Error::RECV_EOF:
      s = "received EOF"sv;
      break;
    case nghttp2::Error::TLS_HANDSHAKE_INPROGRESS:
      s = "TLS handshake inprogress"sv;
      break;
    case nghttp2::Error::DCONN_CANCELED:
      s = "downstream connection canceled"sv;
      break;
    case nghttp2::Error::DCONN_RETRY:
      s = "retry downstream connection"sv;
      break;
    case nghttp2::Error::TLS_REQUIRED:
      s = "TLS required"sv;
      break;
    case nghttp2::Error::INVALID_PEM_TYPE:
      s = "invalid PEM type"sv;
      break;
    case nghttp2::Error::ENTITY_NOT_FOUND:
      s = "entity not found"sv;
      break;
    case nghttp2::Error::DONE:
      s = "done"sv;
      break;
    case nghttp2::Error::TLS_VERIFY_PEER:
      s = "TLS peer verification failed"sv;
      break;
    case nghttp2::Error::CONNECT_FAIL:
      s = "connection failure"sv;
      break;
    case nghttp2::Error::ALPN:
      s = "ALPN negotiation failure"sv;
      break;
    case nghttp2::Error::NO_AVAIL_DOWNSTREAM:
      s = "no downstream address available"sv;
      break;
    case nghttp2::Error::HTTP:
      s = "HTTP messaging violation"sv;
      break;
    case nghttp2::Error::DNS:
      s = "DNS failure"sv;
      break;
    case nghttp2::Error::WEBSOCKET_HANDSHAKE:
      s = "WebSocket handshake failure"sv;
      break;
    case nghttp2::Error::PROXY_PROTO:
      s = "PROXY-protocol failure"sv;
      break;
    case nghttp2::Error::HTTP_FIELD_TOO_LARGE:
      s = "HTTP fields are too large"sv;
      break;
    case nghttp2::Error::MRUBY:
      s = "mruby"sv;
      break;
    case nghttp2::Error::MEMCACHED:
      s = "memcached protocol error"sv;
      break;
    case nghttp2::Error::BPF:
      s = "BPF"sv;
      break;
    case nghttp2::Error::INVALID_CONFIG:
      s = "invalid configuration"sv;
      break;
    case nghttp2::Error::STREAM_ID_BLOCKED:
      s = "Stream creation blocked"sv;
      break;
    }

    return std::formatter<std::string_view>::format(s, ctx);
  }
};

#endif // !defined(ERRORS_H)
