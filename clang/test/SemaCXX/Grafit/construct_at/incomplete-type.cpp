// RUN: %clang_cc1 -fsyntax-only -verify -verify-ignore-unexpected=note %s
struct Incomplete;
void test() {
  void *p = (void *)0x800000000000;
  auto incomplete = __grafit_construct_at<Incomplete>(p);
  // expected-error@-1 {{provided type cannot be constructed by 'construct_at<T>(ptr, ...)' 'Incomplete'}}
}