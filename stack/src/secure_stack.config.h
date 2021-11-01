#include "../../config_create/config_create.h"

#define VALIDATION ON
#define CANARIES   ON
#define HASH       ON

#define POISON     145
#define POISON_PTR (void *)300
#define CANARY     0xC0DAB1ED11D0BEFULL
