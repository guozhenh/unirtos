/*
 * Copyright (c) Quectel Wireless Solution, Co., Ltd.All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "qosa_log.h"
#include "qosa_sys.h"

#define main_printf(...) QOSA_LOG_E(LOG_TAG, ##__VA_ARGS__)

extern void components_init(void);
extern void apps_init(void);

void unirtos_init(void)
{
    main_printf("unirtos_init init");

    components_init();
    apps_init();
}
