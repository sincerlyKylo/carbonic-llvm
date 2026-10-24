// RUN: %clang_cc1 -fsyntax-only -verify -verify-ignore-unexpected=note %s

void* get_storage();

void test() {
  void *void_storage = (void *)0x800000000000;
  char *char_storage = (char *)0x800000000000;
  int *int_storage = (int *)0x800000000000;
  void *p = (void *)0x800000000000;

  // Any pointer type may be used as storage.
  auto a = __grafit_construct_at<int>(void_storage, 42);
  auto b = __grafit_construct_at<int>(char_storage, 42);
  auto c = __grafit_construct_at<int>(int_storage, 42);

  // The destination itself must be a pointer.
  auto integer = __grafit_construct_at<int>(42);
  // expected-error@-1 {{first argument for 'construct_at<T>(ptr, ...)' is not a pointer}}
  auto floating = __grafit_construct_at<int>(42.0);
  // expected-error@-1 {{first argument for 'construct_at<T>(ptr, ...)' is not a pointer}}

  a = __grafit_construct_at<int>(int_storage + 4, 42);
  a = __grafit_construct_at<int>(get_storage(), 42);

  a = __grafit_construct_at<int>(p + 4, 42);
  // expected-error@-1 {{arithmetic on a pointer to void}}
}