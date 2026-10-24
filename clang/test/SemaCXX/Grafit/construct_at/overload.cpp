// RUN: %clang_cc1 -fsyntax-only -verify -verify-ignore-unexpected=note %s

struct TOverloaded {
  TOverloaded(int);
  TOverloaded(float);
};

struct TTwoArgs {
  TTwoArgs(int, float);
};

void test() {
  void *p = (void *)0x800000000000;

  auto overloaded_int = __grafit_construct_at<TOverloaded>(p, 42);
  auto overloaded_float = __grafit_construct_at<TOverloaded>(p, 42.0f);

  auto two_args = __grafit_construct_at<TTwoArgs>(p, 42, 3.14f);

  auto too_many = __grafit_construct_at<TOverloaded>(p, 1, 2, 3);
  // expected-error@-1 {{no matching constructor for initialization of 'TOverloaded'}}

  auto too_few = __grafit_construct_at<TTwoArgs>(p, 42);
  // expected-error@-1 {{no matching constructor for initialization of 'TTwoArgs'}}
}