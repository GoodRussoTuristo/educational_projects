/*! \file
 *  This file includes some prototypes of functions
 *  whick implement various hashing algorithms.
 */




#ifndef HASH_H_


#define HASH_H_




/*================= Connectiong headers ==================*/


#include <stdint.h>
#include <stddef.h>




/*================== Function prototypes =================*/


/*! This function implements the Pearson hashing algorithm
 *  for a 64-bit number.
 *
 *  \param[in] data - pointer to hashing memory.
 *  \param[in] len  - length of hashing memory.
 *
 *  \return hash value.
 */
uint64_t pearson_hash64 (const void *data, size_t len);


#endif
