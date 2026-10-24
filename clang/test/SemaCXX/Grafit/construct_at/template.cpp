// RUN: %clang_cc1 -fsyntax-only -verify -verify-ignore-unexpected=note %s
// expected-no-diagnostics

template<class T>
auto make(void *p) {
  return __grafit_construct_at<T>(p);
}

template<class T>
auto make(void *p, T value) {
  return __grafit_construct_at<int>(p, value);
}

struct X {
  X();
};

void test(void *p) {
  make<int>(p);
  /* make<X>(p);
  make(p, 3.14f); */
}