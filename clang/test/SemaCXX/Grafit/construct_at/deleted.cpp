// RUN: %clang_cc1 -fsyntax-only -verify -verify-ignore-unexpected=note %s

struct TDeleted {
  TDeleted() = delete;
};

void test() {
  void *p = (void *)0x800000000000;

  auto deleted = __grafit_construct_at<TDeleted>(p);
  // expected-error@-1 {{call to deleted constructor of 'TDeleted'}}
}