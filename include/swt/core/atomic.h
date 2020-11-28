/*
 * atomic.h
 *
 *  Created on: 21 sept. 2020
 *      Author: azeddine El Bassbasi
 */

#ifndef SWT_SWT_CORE_ATOMIC_H_
#define SWT_SWT_CORE_ATOMIC_H_
#include "defs.h"
#ifdef __cplusplus
extern "C" {
#endif
typedef enum {
	wmemory_order_relaxed = __ATOMIC_RELAXED,
	wmemory_order_consume = __ATOMIC_CONSUME,
	wmemory_order_acquire = __ATOMIC_ACQUIRE,
	wmemory_order_release = __ATOMIC_RELEASE,
	wmemory_order_acq_rel = __ATOMIC_ACQ_REL,
	wmemory_order_seq_cst = __ATOMIC_SEQ_CST
} wmemory_order;
typedef volatile wintptr watomic_intptr;
typedef volatile char watomic_char;
typedef volatile signed char watomic_schar;
typedef volatile unsigned char watomic_uchar;
typedef volatile short watomic_short;
typedef volatile unsigned short watomic_ushort;
typedef volatile int watomic_int;
typedef volatile unsigned int watomic_uint;
typedef volatile long watomic_long;
typedef volatile unsigned long watomic_ulong;
typedef volatile long long watomic_llong;
typedef volatile unsigned long long watomic_ullong;

#define watomic_store_explicit(PTR, VAL, MO)				\
  __extension__								\
  ({									\
    __auto_type __atomic_store_ptr = (PTR);				\
    __typeof__ (*__atomic_store_ptr) __atomic_store_tmp = (VAL);	\
    __atomic_store (__atomic_store_ptr, &__atomic_store_tmp, (MO));	\
  })

#define watomic_store(PTR, VAL)				\
  atomic_store_explicit (PTR, VAL, __ATOMIC_SEQ_CST)


#define watomic_load_explicit(PTR, MO)					\
  __extension__								\
  ({									\
    __auto_type __atomic_load_ptr = (PTR);				\
    __typeof__ (*__atomic_load_ptr) __atomic_load_tmp;			\
    __atomic_load (__atomic_load_ptr, &__atomic_load_tmp, (MO));	\
    __atomic_load_tmp;							\
  })

#define watomic_load(PTR)  atomic_load_explicit (PTR, __ATOMIC_SEQ_CST)


#define watomic_exchange_explicit(PTR, VAL, MO)				\
  __extension__								\
  ({									\
    __auto_type __atomic_exchange_ptr = (PTR);				\
    __typeof__ (*__atomic_exchange_ptr) __atomic_exchange_val = (VAL);	\
    __typeof__ (*__atomic_exchange_ptr) __atomic_exchange_tmp;		\
    __atomic_exchange (__atomic_exchange_ptr, &__atomic_exchange_val,	\
		       &__atomic_exchange_tmp, (MO));			\
    __atomic_exchange_tmp;						\
  })

#define watomic_exchange(PTR, VAL) 			\
  atomic_exchange_explicit (PTR, VAL, __ATOMIC_SEQ_CST)


#define watomic_compare_exchange_strong_explicit(PTR, VAL, DES, SUC, FAIL) \
  __extension__								\
  ({									\
    __auto_type __atomic_compare_exchange_ptr = (PTR);			\
    __typeof__ (*__atomic_compare_exchange_ptr) __atomic_compare_exchange_tmp \
      = (DES);								\
    __atomic_compare_exchange (__atomic_compare_exchange_ptr, (VAL),	\
			       &__atomic_compare_exchange_tmp, 0,	\
			       (SUC), (FAIL));				\
  })

#define watomic_compare_exchange_strong(PTR, VAL, DES) 			   \
  atomic_compare_exchange_strong_explicit (PTR, VAL, DES, __ATOMIC_SEQ_CST, \
					   __ATOMIC_SEQ_CST)

#define watomic_compare_exchange_weak_explicit(PTR, VAL, DES, SUC, FAIL) \
  __extension__								\
  ({									\
    __auto_type __atomic_compare_exchange_ptr = (PTR);			\
    __typeof__ (*__atomic_compare_exchange_ptr) __atomic_compare_exchange_tmp \
      = (DES);								\
    __atomic_compare_exchange (__atomic_compare_exchange_ptr, (VAL),	\
			       &__atomic_compare_exchange_tmp, 1,	\
			       (SUC), (FAIL));				\
  })

#define watomic_compare_exchange_weak(PTR, VAL, DES)			\
  atomic_compare_exchange_weak_explicit (PTR, VAL, DES, __ATOMIC_SEQ_CST, \
					 __ATOMIC_SEQ_CST)



#define watomic_fetch_add(PTR, VAL) __atomic_fetch_add ((PTR), (VAL), 	\
						       __ATOMIC_SEQ_CST)
#define watomic_fetch_add_explicit(PTR, VAL, MO) 			\
			  __atomic_fetch_add ((PTR), (VAL), (MO))

#define watomic_fetch_sub(PTR, VAL) __atomic_fetch_sub ((PTR), (VAL), 	\
						       __ATOMIC_SEQ_CST)
#define watomic_fetch_sub_explicit(PTR, VAL, MO) 			\
			  __atomic_fetch_sub ((PTR), (VAL), (MO))

#define watomic_fetch_or(PTR, VAL) __atomic_fetch_or ((PTR), (VAL), 	\
						       __ATOMIC_SEQ_CST)
#define watomic_fetch_or_explicit(PTR, VAL, MO) 			\
			  __atomic_fetch_or ((PTR), (VAL), (MO))

#define watomic_fetch_xor(PTR, VAL) __atomic_fetch_xor ((PTR), (VAL), 	\
						       __ATOMIC_SEQ_CST)
#define watomic_fetch_xor_explicit(PTR, VAL, MO) 			\
			  __atomic_fetch_xor ((PTR), (VAL), (MO))

#define watomic_fetch_and(PTR, VAL) __atomic_fetch_and ((PTR), (VAL), 	\
						       __ATOMIC_SEQ_CST)
#define watomic_fetch_and_explicit(PTR, VAL, MO) 			\
			  __atomic_fetch_and ((PTR), (VAL), (MO))


#ifdef __cplusplus
}
#endif
#endif /* SWT_SWT_CORE_ATOMIC_H_ */
