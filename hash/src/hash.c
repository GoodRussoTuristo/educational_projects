/*! \file
 *  This file includes some hashing algorithms implementations.
 */




/*================= Connectiong headers ==================*/


#include "hash.h"




/*=================== Global functions ===================*/


uint64_t pearson_hash64 (const void *data, size_t len)
{
	uint64_t hash64 = len;
	const unsigned char *uchar_data =
		(const unsigned char *) data;

	for (int j = 0; j < sizeof hash64; j++)
	{
		unsigned char hash8 = ((uchar_data[0] + j) % 256) ^ 89;

		for (int i = 1; i < len; i++)
			hash8 = (hash8 ^ uchar_data[i]) ^ 89;

		hash64 = (hash64 << 8) | hash8;
	}

	return hash64;
}
