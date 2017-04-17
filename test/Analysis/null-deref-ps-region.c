// RUN: %clang_analyze_cc1 -analyzer-checker=core,alpha.core,unix,alpha.unix -std=gnu99 -analyzer-store=region -verify %s

#include "Inputs/system-header-simulator.h"

typedef __typeof(sizeof(int)) size_t;
void *memset(void *__s, int __c, size_t __n);
void *malloc(size_t __size);
void free(void *__ptr);

// The store for 'a[1]' should not be removed mistakenly. SymbolicRegions may
// also be live roots.
void f14(int *a) {
  int i;
  a[1] = 1;
  i = a[1];
  if (i != 1) {
    int *p = 0;
    i = *p; // no-warning
  }
}

void f531() {
  int *x = 0;
  memset(x, 0, 1); // expected-warning {{Null pointer argument in call to memory set function}}
}

void f61() {
  char buf[13];
  memset(buf, 0, 1); // no-warning
}

void f611() {
  char *buf = (char *)malloc(13);
  memset(buf, 0, 1); // no-warning
  free(buf);
}

void f66() {
  char buf[1];
  memset(buf, 0, 1024); // expected-warning {{Memory set function accesses out-of-bound array element}}
}

void f666() {
  char *buf = (char *)malloc(1);
  memset(buf, 0, 1024); // expected-warning {{Memory set function accesses out-of-bound array element}}
  free(buf);
}

void f77() {
  char buf[1];
  memset(buf, 0, sizeof(buf)); // no-warning
}

void f777() {
  char *buf = (char *)malloc(1);
  memset(buf, 0, 1); // no-warning
  free(buf);
}
