/*
 * nghttp2 - HTTP/2 C Library
 *
 * Copyright (c) 2016 Tatsuhiro Tsujikawa
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
#include "shrpx_router_test.h"

#include "munitxx.h"

#include "shrpx_router.h"

using namespace std::literals;

namespace shrpx {

namespace {
const MunitTest tests[]{
  munit_void_test(test_shrpx_router_match),
  munit_void_test(test_shrpx_router_match_wildcard),
  munit_void_test(test_shrpx_router_match_prefix),
  munit_test_end(),
};
} // namespace

const MunitSuite router_suite{
  .prefix = "/router",
  .tests = tests,
};

struct Pattern {
  std::string_view pattern;
  size_t idx;
  bool wildcard;
};

void test_shrpx_router_match(void) {
  auto patterns = std::vector<Pattern>{
    {"nghttp2.org/"sv, 0},
    {"nghttp2.org/alpha"sv, 1},
    {"nghttp2.org/alpha/"sv, 2},
    {"nghttp2.org/alpha/bravo/"sv, 3},
    {"www.nghttp2.org/alpha/"sv, 4},
    {"/alpha"sv, 5},
    {"example.com/alpha/"sv, 6},
    {"nghttp2.org/alpha/bravo2/"sv, 7},
    {"www2.nghttp2.org/alpha/"sv, 8},
    {"www2.nghttp2.org/alpha2/"sv, 9},
  };

  Router router;

  for (auto &p : patterns) {
    router.add_route(p.pattern, p.idx);
  }

  constexpr auto badval = std::numeric_limits<size_t>::max();

  assert_size(0, ==, router.match("nghttp2.org"sv, "/"sv).value_or(badval));

  assert_size(1, ==,
              router.match("nghttp2.org"sv, "/alpha"sv).value_or(badval));

  assert_size(2, ==,
              router.match("nghttp2.org"sv, "/alpha/"sv).value_or(badval));

  assert_size(
    2, ==, router.match("nghttp2.org"sv, "/alpha/charlie"sv).value_or(badval));

  assert_size(
    3, ==, router.match("nghttp2.org"sv, "/alpha/bravo/"sv).value_or(badval));

  // matches pattern when last '/' is missing in path
  assert_size(3, ==,
              router.match("nghttp2.org"sv, "/alpha/bravo"sv).value_or(badval));

  assert_size(8, ==,
              router.match("www2.nghttp2.org"sv, "/alpha"sv).value_or(badval));

  assert_size(5, ==, router.match(""sv, "/alpha"sv).value_or(badval));

  assert_false(router.match("example.com", "/"sv));

  assert_false(router.match("www3.nghttp2.org", "/"sv));
}

void test_shrpx_router_match_wildcard(void) {
  constexpr auto patterns = std::to_array<Pattern>({
    {"nghttp2.org/"sv, 0},
    {"nghttp2.org/"sv, 1, true},
    {"nghttp2.org/alpha/"sv, 2},
    {"nghttp2.org/alpha/"sv, 3, true},
    {"nghttp2.org/bravo"sv, 4},
    {"nghttp2.org/bravo"sv, 5, true},
  });

  Router router;

  for (auto &p : patterns) {
    router.add_route(p.pattern, p.idx, p.wildcard);
  }

  constexpr auto badval = std::numeric_limits<size_t>::max();

  assert_size(0, ==, router.match("nghttp2.org"sv, "/"sv).value_or(badval));

  assert_size(1, ==, router.match("nghttp2.org"sv, "/a"sv).value_or(badval));

  assert_size(1, ==,
              router.match("nghttp2.org"sv, "/charlie"sv).value_or(badval));

  assert_size(2, ==,
              router.match("nghttp2.org"sv, "/alpha"sv).value_or(badval));

  assert_size(2, ==,
              router.match("nghttp2.org"sv, "/alpha/"sv).value_or(badval));

  assert_size(3, ==,
              router.match("nghttp2.org"sv, "/alpha/b"sv).value_or(badval));

  assert_size(4, ==,
              router.match("nghttp2.org"sv, "/bravo"sv).value_or(badval));

  assert_size(
    5, ==, router.match("nghttp2.org"sv, "/bravocharlie"sv).value_or(badval));

  assert_size(5, ==,
              router.match("nghttp2.org"sv, "/bravo/"sv).value_or(badval));

  assert_false(router.match("www.nghttp2.org"sv, "/"sv).has_value());
}

void test_shrpx_router_match_prefix(void) {
  auto patterns = std::vector<Pattern>{
    {"gro.2ptthgn."sv, 0},
    {"gro.2ptthgn.www."sv, 1},
    {"gro.2ptthgn.gmi."sv, 2},
    {"gro.2ptthgn.gmi.ahpla."sv, 3},
  };

  Router router;

  for (auto &p : patterns) {
    router.add_route(p.pattern, p.idx);
  }

  const RNode *node;
  size_t nread;

  constexpr auto badval = std::numeric_limits<size_t>::max();

  node = nullptr;

  assert_size(
    0, ==,
    router.match_prefix(&nread, &node, "gro.2ptthgn.gmi.ahpla.ovarb"sv)
      .value_or(badval));
  assert_size(12, ==, nread);

  assert_size(
    2, ==,
    router.match_prefix(&nread, &node, "gmi.ahpla.ovarb"sv).value_or(badval));
  assert_size(4, ==, nread);

  assert_size(
    3, ==,
    router.match_prefix(&nread, &node, "ahpla.ovarb"sv).value_or(badval));
  assert_size(6, ==, nread);

  assert_false(router.match_prefix(&nread, &node, "c.b.c"sv).has_value());
}

} // namespace shrpx
