//////////////////////////////////////////////////////////////////////////////
// Licensed to Qualys, Inc. (QUALYS) under one or more
// contributor license agreements.  See the NOTICE file distributed with
// this work for additional information regarding copyright ownership.
// QUALYS licenses this file to You under the Apache License, Version 2.0
// (the "License"); you may not use this file except in compliance with
// the License.  You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////
/// @file
/// @brief IronBee - Hash Test Functions
/// 
/// @author Pablo Rincon <pablo.rincon.crespo@gmail.com>
//////////////////////////////////////////////////////////////////////////////

#include "ironbee_config_auto.h"

#include "gtest/gtest.h"
#include "gtest/gtest-spi.h"

#define TESTING

#include "util/util.c"
#include "util/hash.c"
#include "util/list.c"
#include "util/mpool.c"
#include "util/debug.c"


/* -- Tests -- */

/// @test Test util hash library - ib_hash_create()
TEST(TestIBUtilHash, test_hash_create)
{
    ib_mpool_t *mp = NULL;
    ib_hash_t *ht = NULL;
    ib_status_t rc;

    atexit(ib_shutdown);
    rc = ib_initialize();
    ASSERT_TRUE(rc == IB_OK) << "ib_initialize() failed - rc != IB_OK";
    rc = ib_mpool_create(&mp, NULL);
    ASSERT_TRUE(rc == IB_OK) << "ib_mpool_create() failed - rc != IB_OK";
    rc = ib_hash_create(&ht, mp);
    ASSERT_TRUE(rc == IB_OK) << "ib_hash_create_ex() failed - rc != IB_OK";
    ib_hash_clear(ht);
    
    ib_mpool_destroy(mp);
}

/// @test Test util hash library - ib_hash_create_ex(), ib_hash_set/get()
TEST(TestIBUtilHash, test_hash_set_and_get)
{
    ib_mpool_t *mp = NULL;
    ib_hash_t *ht = NULL;
    ib_status_t rc;

    atexit(ib_shutdown);

    rc = ib_initialize();
    ASSERT_TRUE(rc == IB_OK) << "ib_initialize() failed - rc != IB_OK";
    rc = ib_mpool_create(&mp, NULL);
    ASSERT_TRUE(rc == IB_OK) << "ib_mpool_create() failed - rc != IB_OK";

    rc = ib_hash_create_ex(&ht, mp, IB_HASH_INITIAL_SIZE, IB_HASH_FLAG_CASE);
    ASSERT_TRUE(rc == IB_OK) << "ib_hash_create_ex() failed - rc != IB_OK";
    rc = ib_hash_set(ht, "Key", (void*)"value");
    ASSERT_TRUE(rc == IB_OK) << "ib_hash_set() failed - rc != IB_OK";

    char *val = NULL;
    rc = ib_hash_get(ht, "Key", (void **)&val);
    ASSERT_TRUE(rc == IB_OK) << "ib_hash_get() failed - rc != IB_OK";
    ASSERT_TRUE(strncmp("value", val, 5) == 0) << "ib_hash_get() failed -"
                                             " expected \"value\", got " << val;
    rc = ib_hash_set(ht, "Key2", (void*)"value2");
    ASSERT_TRUE(rc == IB_OK) << "ib_hash_set() failed - rc != IB_OK";
    val = NULL;
    rc = ib_hash_get(ht, "Key", (void **)&val);
    ASSERT_TRUE(rc == IB_OK) << "ib_hash_get() failed - rc != IB_OK";
    ASSERT_TRUE(strncmp("value", val, 5) == 0) << "ib_hash_get() failed -"
                                             " expected \"value\", got " << val;
    val = NULL;
    rc = ib_hash_get(ht, "Key2", (void **)&val);
    ASSERT_TRUE(rc == IB_OK) << "ib_hash_get() failed - rc != IB_OK";
    ASSERT_TRUE(strncmp("value2", val, 5) == 0) << "ib_hash_get() failed -"
                                             " expected \"value\", got " << val;

    val = NULL;
    rc = ib_hash_get(ht, "noKey", (void **)&val);
    ASSERT_TRUE(rc == IB_ENOENT) << "ib_hash_get() failed - rc != IB_ENOENT";
    
    ib_mpool_destroy(mp);
}

/// @test Test util hash library - ib_hash_set/get() with nocase
TEST(TestIBUtilHash, test_hash_nocase)
{
    ib_mpool_t *mp = NULL;
    ib_hash_t *ht = NULL;
    ib_status_t rc;

    atexit(ib_shutdown);
    rc = ib_initialize();
    ASSERT_TRUE(rc == IB_OK) << "ib_initialize() failed - rc != IB_OK";

    rc = ib_mpool_create(&mp, NULL);
    ASSERT_TRUE(rc == IB_OK) << "ib_mpool_create() failed - rc != IB_OK";

    rc = ib_hash_create_ex(&ht, mp, IB_HASH_INITIAL_SIZE, IB_HASH_FLAG_NOCASE);
    ASSERT_TRUE(rc == IB_OK) << "ib_hash_create_ex() failed - rc != IB_OK";

    rc = ib_hash_set(ht, "Key", (void*)"value");
    ASSERT_TRUE(rc == IB_OK) << "ib_hash_set() failed - rc != IB_OK";

    char *val = NULL;
    rc = ib_hash_get_nocase(ht, "kEY", (void **)&val);
    ASSERT_TRUE(rc == IB_OK) << "ib_hash_get_nocase() failed - rc != IB_OK";

    ASSERT_TRUE(strncmp("value", val, 5) == 0) << "ib_hash_get_nocase() "
                                                  "failed - expected "
                                                  "\"value\", got " << val;
    rc = ib_hash_set(ht, "KeY2", (void*)"value2");
    ASSERT_TRUE(rc == IB_OK) << "ib_hash_set() failed - rc != IB_OK";

    val = NULL;
    rc = ib_hash_get_nocase(ht, "KeY", (void **)&val);
    ASSERT_TRUE(rc == IB_OK) << "ib_hash_get_nocase() failed - rc != IB_OK";

    ASSERT_TRUE(strncmp("value", val, 5) == 0) << "ib_hash_get_nocase() failed "
                                                  "- expected \"value\", "
                                                  "got " << val;
    val = NULL;
    rc = ib_hash_get_nocase(ht, "KEY2", (void **)&val);
    ASSERT_TRUE(rc == IB_OK) << "ib_hash_get_nocase() failed - rc != IB_OK";

    ASSERT_TRUE(strncmp("value2", val, 6) == 0) << "ib_hash_get_nocase() failed"
                                           " - expected \"value\", got " << val;

    val = NULL;
    rc = ib_hash_get_nocase(ht, "noKey", (void **)&val);
    ASSERT_TRUE(rc == IB_ENOENT) << "ib_hash_get_nocase() failed - "
                                    "rc != IB_ENOENT";

    ib_mpool_destroy(mp);
}

/// @test Test util hash library - ib_hash_create_ex(), ib_hash_set/get_ex()
TEST(TestIBUtilHash, test_hash_set_and_get_ex)
{
    ib_mpool_t *mp = NULL;
    ib_hash_t *ht = NULL;
    ib_status_t rc;
    char key1[] = "Key1";
    char key2[] = "Key2";
    char key3[] = "kEY1";
    char key4[] = "kEY2";

    atexit(ib_shutdown);

    rc = ib_initialize();
    ASSERT_TRUE(rc == IB_OK) << "ib_initialize() failed - rc != IB_OK";
    rc = ib_mpool_create(&mp, NULL);
    ASSERT_TRUE(rc == IB_OK) << "ib_mpool_create() failed - rc != IB_OK";

    rc = ib_hash_create_ex(&ht, mp, IB_HASH_INITIAL_SIZE, IB_HASH_FLAG_CASE);
    ASSERT_TRUE(rc == IB_OK) << "ib_hash_create_ex() failed - rc != IB_OK";

    rc = ib_hash_set_ex(ht, key1, 2, (void*)"value");
    ASSERT_TRUE(rc == IB_OK) << "ib_hash_set() failed - rc != IB_OK";

    char *val = NULL;
    rc = ib_hash_get_ex(ht, key1, 2, (void **) &val, IB_HASH_FLAG_CASE);
    ASSERT_TRUE(rc == IB_OK) << "ib_hash_get_ex() failed - rc != IB_OK";
    ASSERT_TRUE(strncmp("value", val, 5) == 0) << "ib_hash_get_ex() failed -";

    rc = ib_hash_set_ex(ht, key2, 2, (void*)"other");
    ASSERT_TRUE(rc == IB_OK) << "ib_hash_set() failed - rc != IB_OK";

    val = NULL;
    rc = ib_hash_get_ex(ht, key2, 2, (void **) &val, IB_HASH_FLAG_CASE);
    ASSERT_TRUE(rc == IB_OK) << "ib_hash_get_ex() failed - rc != IB_OK";
    ASSERT_TRUE(strncmp("other", val, 5) == 0) << "ib_hash_get_ex() failed -";

    val = NULL;
    rc = ib_hash_get_ex(ht, key1, 2, (void **) &val, IB_HASH_FLAG_CASE);
    ASSERT_TRUE(rc == IB_OK) << "ib_hash_get_ex() failed - rc != IB_OK";
    ASSERT_TRUE(strncmp("other", val, 5) == 0) << "ib_hash_get_ex() failed -";

    val = NULL;
    rc = ib_hash_get_ex(ht, key3, 2, (void **) &val, IB_HASH_FLAG_CASE);
    ASSERT_TRUE(rc == IB_ENOENT) << "ib_hash_get_ex() failed - rc != IB_ENOENT";

    val = NULL;
    rc = ib_hash_get_ex(ht, key4, 2, (void **) &val, IB_HASH_FLAG_CASE);
    ASSERT_TRUE(rc == IB_ENOENT) << "ib_hash_get_ex() failed - rc != IB_ENOENT";

    ib_mpool_destroy(mp);
}

/// @test Test util hash library - ib_hash_set/get_ex() with nocase
TEST(TestIBUtilHash, test_hash_get_ex_nocase)
{
    ib_mpool_t *mp = NULL;
    ib_hash_t *ht = NULL;
    ib_status_t rc;
    char key1[] = "Key1";
    char key2[] = "Key2";
    char key3[] = "kEY1";
    char key4[] = "kEY2";

    atexit(ib_shutdown);

    rc = ib_initialize();
    ASSERT_TRUE(rc == IB_OK) << "ib_initialize() failed - rc != IB_OK";
    rc = ib_mpool_create(&mp, NULL);
    ASSERT_TRUE(rc == IB_OK) << "ib_mpool_create() failed - rc != IB_OK";

    rc = ib_hash_create_ex(&ht, mp, IB_HASH_INITIAL_SIZE, IB_HASH_FLAG_NOCASE);
    ASSERT_TRUE(rc == IB_OK) << "ib_hash_create_ex() failed - rc != IB_OK";

    rc = ib_hash_set_ex(ht, key1, 2, (void*)"value");
    ASSERT_TRUE(rc == IB_OK) << "ib_hash_set() failed - rc != IB_OK";

    char *val = NULL;
    rc = ib_hash_get_ex(ht, key1, 2, (void **) &val, IB_HASH_FLAG_CASE);
    ASSERT_TRUE(rc == IB_OK) << "ib_hash_get_ex() failed - rc != IB_OK";
    ASSERT_TRUE(strncmp("value", val, 5) == 0) << "ib_hash_get_ex() failed -";

    rc = ib_hash_set_ex(ht, key2, 2, (void*)"other");
    ASSERT_TRUE(rc == IB_OK) << "ib_hash_set() failed - rc != IB_OK";

    val = NULL;
    rc = ib_hash_get_ex(ht, key2, 2, (void **) &val, IB_HASH_FLAG_CASE);
    ASSERT_TRUE(rc == IB_OK) << "ib_hash_get_ex() failed - rc != IB_OK";
    ASSERT_TRUE(strncmp("other", val, 5) == 0) << "ib_hash_get_ex() failed -";

    val = NULL;
    rc = ib_hash_get_ex(ht, key1, 2, (void **) &val, IB_HASH_FLAG_CASE);
    ASSERT_TRUE(rc == IB_OK) << "ib_hash_get_ex() failed - rc != IB_OK";
    ASSERT_TRUE(strncmp("other", val, 5) == 0) << "ib_hash_get_ex() failed -";

    val = NULL;
    rc = ib_hash_get_ex(ht, key3, 2, (void **) &val, IB_HASH_FLAG_CASE);
    ASSERT_TRUE(rc == IB_ENOENT) << "ib_hash_get_ex() failed - rc != IB_ENOENT";

    val = NULL;
    rc = ib_hash_get_ex(ht, key4, 2, (void **) &val, IB_HASH_FLAG_CASE);
    ASSERT_TRUE(rc == IB_ENOENT) << "ib_hash_get_ex() failed - rc != IB_ENOENT";

    val = NULL;
    rc = ib_hash_get_ex(ht, key3, 2, (void **) &val, IB_HASH_FLAG_NOCASE);
    ASSERT_TRUE(rc == IB_OK) << "ib_hash_get_ex() failed - rc != IB_OK";
    ASSERT_TRUE(strncmp("other", val, 5) == 0) << "ib_hash_get_ex() failed -";

    val = NULL;
    rc = ib_hash_get_ex(ht, key4, 2, (void **) &val, IB_HASH_FLAG_NOCASE);
    ASSERT_TRUE(rc == IB_OK) << "ib_hash_get_ex() failed - rc != IB_OK";
    ASSERT_TRUE(strncmp("other", val, 5) == 0) << "ib_hash_get_ex() failed -";

    ib_mpool_destroy(mp);
}

/// @test Test util hash library - ib_hash_default with flags
TEST(TestIBUtilHash, test_hashfunc_default)
{
    ib_mpool_t *mp = NULL;
    ib_status_t rc;
    unsigned int hash1 = 0;
    unsigned int hash2 = 0;

    atexit(ib_shutdown);
    rc = ib_initialize();

    // Test with no case sensitive
    hash1 = ib_hashfunc_default("Key", 3,
                                IB_HASH_FLAG_NOCASE);
    hash2 = ib_hashfunc_default("kEY", 3,
                                IB_HASH_FLAG_NOCASE);
    ASSERT_TRUE(hash1 == hash2) << "ib_hashfunc_default() failed - hash1:"
                                    << hash1 << " != Hash2:" << hash2
                                    << " but should be equal";
    // Test with case sensitive
    hash1 = hash2 = 0;
    hash1 = ib_hashfunc_default("Key", 3, 0);
    hash2 = ib_hashfunc_default("kEY", 3, 0);
    ASSERT_TRUE(hash1 != hash2) << "ib_hashfunc_default() failed - hash1:"
                                    << hash1 << " == Hash2:" << hash2
                                    << " but should not be equal";
    
}

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    ib_trace_init(NULL);
    return RUN_ALL_TESTS();
}
