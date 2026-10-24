// RUN: %clang_cc1 -fsyntax-only -verify -verify-ignore-unexpected=note %s
// expected-no-diagnostics
struct T {
  T();
  T(int);
};

void test() {
  void *p = (void *)0x800000000000;

  auto result1 = __grafit_construct_at<int>(p);
  result1 = __grafit_construct_at<int>(p, 123);

  auto result2 = __grafit_construct_at<T>(p);
  result2 = __grafit_construct_at<T>(p, 456);
}