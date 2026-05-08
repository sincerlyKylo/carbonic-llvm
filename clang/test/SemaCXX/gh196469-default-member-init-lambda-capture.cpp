// RUN: %clang_cc1 -std=c++20 -fsyntax-only -verify %s

struct Noisy {
  int x;
  consteval Noisy(int x) : x(x) {}
  ~Noisy() {}
};

struct Function {
  template <typename F> Function(F) {}
};

struct Options {
  int x;
  Function function{ // expected-note {{declared here}}
      // expected-error@+2 {{call to consteval function}}
      // expected-note@+1 {{implicit use of 'this' pointer is only allowed within the evaluation of a call to a 'constexpr' member function}}
      [noisy = Noisy{x}] {}};
};

int foo();
// expected-note@+1 {{in the default initializer of 'function'}}
Options options{foo()};
