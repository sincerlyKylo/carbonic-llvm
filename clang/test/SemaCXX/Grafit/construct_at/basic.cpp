// RUN: %clang_cc1 -fsyntax-only -verify -verify-ignore-unexpected=note %s
// expected-no-diagnostics
struct T {
  T();
  T(int);
};

void test() {
  void *p = (void *)0x800000000000;

  auto i = __grafit_construct_at<int>(p);
  i = __grafit_construct_at<int>(p, 42);

  auto f = __grafit_construct_at<float>(p);
  f = __grafit_construct_at<float>(p, 42);

  auto d = __grafit_construct_at<double>(p, 42);

  i = __grafit_construct_at<int>(p, 42 + 7);
  i = __grafit_construct_at<int>(p, 10 * 20);
  f = __grafit_construct_at<float>(p, 3.14f);

  auto t0 = __grafit_construct_at<T>(p);
  t0 = __grafit_construct_at<T>(p, 42);
}