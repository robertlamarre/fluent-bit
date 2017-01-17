/* -*- Mode: C; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*- */

/*  Fluent Bit
 *  ==========
 *  Copyright (C) 2015-2017 Treasure Data Inc.
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 */

#include <stdio.h>

#include <fluent-bit/flb_filter.h>
#include <fluent-bit/flb_utils.h>

#include <msgpack.h>

static int cb_stdout_init(struct flb_filter_instance *f_ins,
                          struct flb_config *config,
                          void *data)
{
    (void) f_ins;
    (void) config;
    (void) data;

    return 0;
}

static int cb_stdout_filter(void *data, size_t bytes,
                            char *tag, int tag_len,
                            struct flb_filter_instance *f_ins,
                            void *filter_context,
                            struct flb_config *config)
{
    msgpack_unpacked result;
    size_t off = 0, cnt = 0;
    (void) f_ins;
    (void) filter_context;
    (void) config;

    msgpack_unpacked_init(&result);
    while (msgpack_unpack_next(&result, data, bytes, &off)) {
        printf("[%zd] %s: ", cnt++, tag);
        msgpack_object_print(stdout, result.data);
        printf("\n");
    }
    msgpack_unpacked_destroy(&result);
    return 0;
}

struct flb_filter_plugin filter_stdout_plugin = {
    .name         = "stdout",
    .description  = "Filter events to STDOUT",
    .cb_init      = cb_stdout_init,
    .cb_filter    = cb_stdout_filter,
    .cb_exit      = NULL,
    .flags        = 0
};