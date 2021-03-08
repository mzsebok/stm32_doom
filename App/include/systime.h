/*
 * systime.h
 */

#ifndef INCLUDE_SYSTIME_H_
#define INCLUDE_SYSTIME_H_

#include <stdint.h>

void systime_overflow(void);
void systime_init(void);
uint64_t systime_get(void);

#endif /* INCLUDE_SYSTIME_H_ */
