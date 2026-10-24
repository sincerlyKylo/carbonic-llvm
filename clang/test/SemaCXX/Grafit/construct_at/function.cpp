// RUN: %clang_cc1 -fsyntax-only -verify -verify-ignore-unexpected=note %s
void test() {
  void *p = (void *)0x800000000000;
  auto fn = __grafit_construct_at<void()>(p);
  // expected-error@-1 {{'construct_at<T>(ptr, ...)' cannot construct function type 'void ()'}}
}