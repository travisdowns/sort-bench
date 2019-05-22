/* An alternative to qsort, with an identical interface.
   This file is part of the GNU C Library.
   Copyright (C) 1992-2016 Free Software Foundation, Inc.
   Written by Mike Haertel, September 1988.

   The GNU C Library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation; either
   version 2.1 of the License, or (at your option) any later version.

   The GNU C Library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public
   License along with the GNU C Library; if not, see
   <http://www.gnu.org/licenses/>.  */

#define _GNU_SOURCE 1

#include <alloca.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <assert.h>
#include <atomic>

struct msort_param
{
  size_t s;
  size_t var;
  char *t;
};

template <typename C>
void msort_with_tmp (const struct msort_param *p, void *b, size_t n, C cmp)
{
  char *b1, *b2;
  size_t n1, n2;

  if (n <= 1)
    return;

  n1 = n / 2;
  n2 = n - n1;
  b1 = (char *)b;
  b2 = (char *) b + (n1 * p->s);

  msort_with_tmp (p, b1, n1, cmp);
  msort_with_tmp (p, b2, n2, cmp);

  char *tmp = p->t;
  const size_t s = p->s;
  assert(p->var == 1);
  while (n1 > 0 && n2 > 0)
  {
    if (cmp(b1, b2) <= 0)
    {
      *(uint64_t *) tmp = *(uint64_t *) b1;
      b1 += sizeof (uint64_t);
      --n1;
    }
    else
    {
      *(uint64_t *) tmp = *(uint64_t *) b2;
      b2 += sizeof (uint64_t);
      --n2;
    }
    tmp += sizeof (uint64_t);
  }

  if (n1 > 0)
    memcpy (tmp, b1, n1 * s);
  memcpy (b, p->t, (n - n2) * s);
}


template <typename C>
static inline void __qsort_r_copy (void *b, size_t n, size_t s, C cmp)
{
  size_t size = n * s;
  char *tmp = NULL;
  struct msort_param p;

  
    /* It's somewhat large, so malloc it.  */
  tmp = (char *)malloc(size);
  if (tmp == NULL)
  {
      /* Couldn't get space, so use the slower algorithm
          that doesn't need a temporary array.  */
      assert(0);
  }
  assert(s == sizeof(uint64_t));
  p.t = tmp;
  p.var = 1;
  p.s = s;  
  
  msort_with_tmp (&p, b, n, cmp);
    
  free (tmp);
}

void qsort_copy(void *b, size_t n, size_t s, __compar_fn_t cmp) {
  return __qsort_r_copy (b, n, s, cmp);
}

struct CompareU64 {
  int operator()(const void *l_, const void *r_) {
    if ( (*(uint64_t *) l_) < (*(uint64_t *) r_)) return -1;
    if ( (*(uint64_t *) l_) > (*(uint64_t *) r_)) return 1;
    return 0;
  }
};

void qsort_inlined(uint64_t* input, size_t size) {
  return __qsort_r_copy (input, size, sizeof(uint64_t), CompareU64{});
}
