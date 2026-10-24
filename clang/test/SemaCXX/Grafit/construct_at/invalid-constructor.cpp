// RUN: %clang_cc1 -fsyntax-only -verify -verify-ignore-unexpected=note %s

struct T {
  T();
  T(int);
};

void test() {
  void *p = (void *)0x800000000000;

  auto nonexistent = __grafit_construct_at<T>(p, "hello");
  // expected-error@-1 {{no matching constructor for initialization of 'T'}}
}