// RUN: %clang_cc1 -fsyntax-only -verify -verify-ignore-unexpected=note %s

struct TPrivate {
private:
  TPrivate(int);

public:
  TPrivate();
};

void test() {
  void *p = (void *)0x800000000000;

  auto private_ctor = __grafit_construct_at<TPrivate>(p, 42);
  // expected-error@-1 {{calling a private constructor of class 'TPrivate'}}
}