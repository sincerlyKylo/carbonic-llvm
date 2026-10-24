// RUN: %clang_cc1 -fsyntax-only -verify -verify-ignore-unexpected=note %s
void test() {
  void *p = (void *)0x800000000000;
  auto void_object = __grafit_construct_at<void>(p);
  // expected-error@-1 {{provided type cannot be constructed}}
}