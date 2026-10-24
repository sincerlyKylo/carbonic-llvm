// RUN: %clang_cc1 -fsyntax-only -verify -verify-ignore-unexpected=note %s

struct NoDefault {
  NoDefault(int);
};

void test(void *p) {
  auto x = __grafit_construct_at<NoDefault>(p);
  // expected-error@-1 {{no matching constructor for initialization of 'NoDefault'}}
}