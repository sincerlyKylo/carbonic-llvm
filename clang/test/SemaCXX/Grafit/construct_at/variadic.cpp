// RUN: %clang_cc1 -fsyntax-only -verify -verify-ignore-unexpected=note %s
// expected-no-diagnostics

struct Variadic {
  template<class... Args>
  Variadic(Args...);
};

void test(void *p) {
  auto x = __grafit_construct_at<Variadic>(p, 1, 2, 3, 4);
}