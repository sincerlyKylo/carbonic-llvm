// RUN: %clang_cc1 -fsyntax-only -verify -verify-ignore-unexpected=note %s
void test() {
  void *p = (void *)0x800000000000;
  auto array = __grafit_construct_at<int[3]>(p);
  // expected-error@-1 {{'construct_at<T>(ptr, ...)' can only construct a single instance}}

  auto incompleteArray = __grafit_construct_at<int[]>(p);
  // expected-error@-1 {{'construct_at<T>(ptr, ...)' can only construct a single instance}}

}