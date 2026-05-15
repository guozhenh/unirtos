#ifndef __UNIRTOS_APP_INIT_REGISTRY_H__
#define __UNIRTOS_APP_INIT_REGISTRY_H__

#include <stddef.h>

typedef void (*unirtos_app_init_fn_t)(void);

typedef struct
{
    unsigned short        order;
    const char           *name;
    unirtos_app_init_fn_t fn;
} unirtos_app_init_entry_t;

#if defined(__GNUC__)
/* App-layer first version uses a dedicated linker section and keeps the old macro-based build gating unchanged. */
#define UNIRTOS_APP_INIT_USED __attribute__((used))
#define UNIRTOS_APP_INIT_SECTION __attribute__((section(".unirtos_app_init")))
#else
#error "UniRTOS app init registry currently requires GCC-compatible section support"
#endif

/* Registration only records init metadata; built-in demos and external qos_applications projects use the same dispatch path in apps_init(). */
#define UNIRTOS_APP_EXPORT(order_value, entry_name, entry_fn) \
    static const unirtos_app_init_entry_t __unirtos_app_init_##entry_fn \
    UNIRTOS_APP_INIT_USED UNIRTOS_APP_INIT_SECTION = { \
        order_value, \
        entry_name, \
        entry_fn \
    }

extern const unirtos_app_init_entry_t __unirtos_app_init_start[];
extern const unirtos_app_init_entry_t __unirtos_app_init_end[];

void unirtos_app_run_registered_init(void);

#endif /* __UNIRTOS_APP_INIT_REGISTRY_H__ */
