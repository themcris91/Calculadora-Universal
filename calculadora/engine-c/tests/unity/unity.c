#include <math.h>
#include <setjmp.h>
#include <stdio.h>
#include <string.h>
#include "unity.h"

static struct {
    const char *file;
    const char *cur;
    int tests;
    int failures;
    int cur_failed;
    jmp_buf env;
} U;

void UnityBegin(const char *file)
{
    memset(&U, 0, sizeof U);
    U.file = file;
}

int UnityEnd(void)
{
    printf("\n-----------------------\n%d Tests %d Failures 0 Ignored\n%s\n",
           U.tests, U.failures, U.failures ? "FAIL" : "OK");
    return U.failures;
}

static void fail(int line, const char *msg)
{
    printf("%s:%d:%s:FAIL: %s\n", U.file, line, U.cur, msg);
    U.cur_failed = 1;
    longjmp(U.env, 1);
}

void UnityDefaultTestRun(void (*func)(void), const char *name, int line)
{
    U.cur = name;
    U.tests++;
    U.cur_failed = 0;
    if (setjmp(U.env) == 0) {
        setUp();
        func();
    }
    tearDown();
    if (U.cur_failed) {
        U.failures++;
    } else {
        printf("%s:%d:%s:PASS\n", U.file, line, name);
    }
}

void UnityAssertTrue(int cond, const char *expr, int line)
{
    if (!cond) {
        char buf[256];
        snprintf(buf, sizeof buf, "Expression Evaluated To FALSE: %s", expr);
        fail(line, buf);
    }
}

void UnityAssertEqualInt(long expected, long actual, int line)
{
    if (expected != actual) {
        char buf[128];
        snprintf(buf, sizeof buf, "Expected %ld Was %ld", expected, actual);
        fail(line, buf);
    }
}

void UnityAssertDoubleWithin(double delta, double expected, double actual, int line)
{
    if (!(fabs(actual - expected) <= delta)) {
        char buf[160];
        snprintf(buf, sizeof buf, "Values Not Within Delta %.3g: Expected %.17g Was %.17g",
                 delta, expected, actual);
        fail(line, buf);
    }
}

void UnityAssertEqualString(const char *expected, const char *actual, int line)
{
    if (expected == NULL || actual == NULL || strcmp(expected, actual) != 0) {
        char buf[256];
        snprintf(buf, sizeof buf, "Expected '%s' Was '%s'",
                 expected ? expected : "(null)", actual ? actual : "(null)");
        fail(line, buf);
    }
}
