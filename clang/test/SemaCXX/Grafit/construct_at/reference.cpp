// RUN: %clang_cc1 -fsyntax-only -verify -verify-ignore-unexpected=note %s
void test() {
  void *p = (void *)0x800000000000;
  auto ref = __grafit_construct_at<int&>(p);
  // expected-error@-1 {{construct_at<T>(ptr, ...)' cannot construct reference type 'int &'}}
  auto rref = __grafit_construct_at<int&&>(p);
  // expected-error@-1 {{'construct_at<T>(ptr, ...)' cannot construct reference type 'int &&'}}
}