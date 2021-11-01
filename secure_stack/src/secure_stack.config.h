/*! \file
 *  This is config file for secure_stack.c
 */


#include "../../config_create/config_create.h"

#define VALIDATION ON // Checking the stack for integrity in each operation on it.
#define CANARIES   ON // Protective barriers at the edges of the structure and data of the stack.
#define HASH       ON // A unique value for each state of the stack, which is constantly recalculated.

#define POISON     145
#define POISON_PTR (void *)300
#define CANARY     0xC0DAB1ED11D0BEFULL
