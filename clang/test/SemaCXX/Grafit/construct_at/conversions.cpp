// RUN: %clang_cc1 -fsyntax-only -verify -verify-ignore-unexpected=note %s

struct TConversion {
  TConversion(int);
};

struct Convertible {
  operator int();
};

void test() {
  void *p = (void *)0x800000000000;

  auto converted_float = __grafit_construct_at<float>(p, 42);
  auto converted_double = __grafit_construct_at<double>(p, 42);

  auto converted_int = __grafit_construct_at<int>(p, 42.5);
  // expected-warning@-1 {{implicit conversion from 'double' to 'int' changes value from 42.5 to 42}}

  char c = 42;
  auto converted_char = __grafit_construct_at<int>(p, c);

  Convertible convertible;
  auto user_conversion = __grafit_construct_at<TConversion>(p, convertible);
}