// RUN: %clang_cc1 -fsyntax-only -verify -verify-ignore-unexpected=note %s
// expected-no-diagnostics

void test() {
  void *p = (void *)0x800000000000;

  auto c = __grafit_construct_at<const int>(p, 42);
  auto v = __grafit_construct_at<volatile int>(p, 42);
  auto cv = __grafit_construct_at<const volatile int>(p, 42);
}
