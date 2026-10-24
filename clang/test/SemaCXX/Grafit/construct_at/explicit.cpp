// RUN: %clang_cc1 -fsyntax-only -verify -verify-ignore-unexpected=note %s
// expected-no-diagnostics
struct TExplicit {
  explicit TExplicit(int);
};
void test() {
  void *p = (void *)0x800000000000;
  auto explicit_ctor = __grafit_construct_at<TExplicit>(p, 42);
}