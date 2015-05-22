/*
 * This is a basic test that checks ftrace can trace functions
 * properly within a multi-thread environment.
 */
#include <stdlib.h>
#include <pthread.h>

#define NUM_THREAD  4

static int __attribute__((noinline)) a(void *);
static int __attribute__((noinline)) b(void *);
static int __attribute__((noinline)) c(void *);

static int a(void *arg)
{
	return b(arg) - 1;
}

static int b(void *arg)
{
	return c(arg) + 1;
}

static int c(void *arg)
{
	return *(int *)arg;
}

static void *foo(void *arg)
{
	return (void *)(long) a(arg);
}

int main(int argc, char *argv[])
{
	int i;
	int n = 10;
	int ret = 0;
	void *v;
	pthread_t t[NUM_THREAD];

	if (argc > 1)
		n = atoi(argv[1]);

	for (i = 0; i < NUM_THREAD; i++)
		pthread_create(&t[i], NULL, foo, &n);
	for (i = 0; i < NUM_THREAD; i++) {
		pthread_join(t[i], &v);
		ret += (long)v;
	}

	if (n * NUM_THREAD != ret)
		return ret;
	return 0;
}
