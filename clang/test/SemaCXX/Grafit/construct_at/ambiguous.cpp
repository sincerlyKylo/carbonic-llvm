// RUN: %clang_cc1 -fsyntax-only -verify -verify-ignore-unexpected=note %s
struct Ambiguous {
  Ambiguous(long);
  Ambiguous(double);
};

void test(void *p) {
  auto x = __grafit_construct_at<Ambiguous>(p, 42);
  // expected-error@-1 {{call to constructor of 'Ambiguous' is ambiguous}}
}