// RUN: %clang_cc1 -fsyntax-only -verify -verify-ignore-unexpected=note %s
// expected-no-diagnostics

struct Defaults {
  Defaults(int, float = 3.14f);
};

void test(void *p) {
  auto x = __grafit_construct_at<Defaults>(p, 42);
}