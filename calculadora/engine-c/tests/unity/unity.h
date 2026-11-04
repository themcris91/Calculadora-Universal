#ifndef UNITY_H_COMPAT
#define UNITY_H_COMPAT

void setUp(void);
void tearDown(void);

void UnityBegin(const char *file);
int UnityEnd(void);
void UnityDefaultTestRun(void (*func)(void), const char *name, int line);
void UnityAssertTrue(int cond, const char *expr, int line);
void UnityAssertEqualInt(long expected, long actual, int line);
void UnityAssertDoubleWithin(double delta, double expected, double actual, int line);
void UnityAssertEqualString(const char *expected, const char *actual, int line);

#define UNITY_BEGIN() UnityBegin(__FILE__)
#define UNITY_END() UnityEnd()
#define RUN_TEST(f) UnityDefaultTestRun(f, #f, __LINE__)

#define TEST_ASSERT(c) UnityAssertTrue((c) != 0, #c, __LINE__)
#define TEST_ASSERT_TRUE(c) UnityAssertTrue((c) != 0, #c, __LINE__)
#define TEST_ASSERT_FALSE(c) UnityAssertTrue((c) == 0, "!(" #c ")", __LINE__)
#define TEST_ASSERT_NULL(p) UnityAssertTrue((p) == 0, #p " == NULL", __LINE__)
#define TEST_ASSERT_NOT_NULL(p) UnityAssertTrue((p) != 0, #p " != NULL", __LINE__)
#define TEST_ASSERT_EQUAL_INT(e, a) UnityAssertEqualInt((long)(e), (long)(a), __LINE__)
#define TEST_ASSERT_DOUBLE_WITHIN(d, e, a) UnityAssertDoubleWithin((d), (e), (a), __LINE__)
#define TEST_ASSERT_EQUAL_STRING(e, a) UnityAssertEqualString((e), (a), __LINE__)

#endif /* UNITY_H_COMPAT */
