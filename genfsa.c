#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#ifndef __APPLE__
#include <time.h>
#endif

static _Bool bias_flip(int, int);
static void print_usage(_Bool);
static void print_version(void);
static void print_symbol(int);
static void set_rand_seed(void);

int
main(int argc, char ** argv)
{
	char * endptr;
	int states = 0;
	int alphabet = 0;
	int edge_weight = 50;
	int final_weight = 50;
	int i;
	int j;
	int k;
	_Bool use_eps = 0;
	char c;

	while ((c = getopt(argc, argv, "e:f:hvz?")) != -1)
	{
		switch (c)
		{
		case 'e':
			edge_weight  = (int)strtol(optarg, &endptr, 0);
			if ('\0' != *endptr)
			{
				return EXIT_FAILURE;
			}
			break;
		case 'f':
			final_weight = (int)strtol(optarg, &endptr, 0);
			if ('\0' != *endptr)
			{
				return EXIT_FAILURE;
			}
			break;
		case 'z':
			use_eps = 1;
			break;
		case 'v':
			print_version();
			return EXIT_SUCCESS;
		case 'h':
		case '?':
			print_usage(1);
			return EXIT_SUCCESS;
		default:
			print_usage(0);
			return EXIT_FAILURE;
		}
	}
	argc -= optind;
	argv += optind;

	if (2 != argc)
	{
		print_usage(0);
		return EXIT_FAILURE;
	}

	states = strtol(argv[0], &endptr, 0);
	if ('\0' != *endptr)
	{
		print_usage(0);
		return EXIT_FAILURE;
	}
	alphabet = strtol(argv[1], &endptr, 0);
	if ('\0' != *endptr)
	{
		print_usage(0);
		return EXIT_FAILURE;
	}
	set_rand_seed();

	/* Transitions */
	for (i = 1; i <= states; ++i)
	{
		for (j = 1; j <= states; ++j)
		{
			for (k = use_eps ? 0 : 1; k <= alphabet; ++k)
			{
				if (bias_flip(100, edge_weight))
				{
					printf("%d\t%d\t", i, j);
					print_symbol(k);
					printf("\n");
				}
			}
		}
	}

	/* Finals */
	for (i = 1; i <= states; ++i)
	{
		if (bias_flip(100, final_weight))
		{
			printf("%d\n", i);
		}
	}

	return EXIT_SUCCESS;
}

static _Bool
bias_flip(int n, int m)
{
	int x = 0;
	if (n == 0)
	{
		return 1;
	}
	x = rand() / (RAND_MAX / n + 1);
	return (x < m);
}

static void
print_usage(_Bool requested)
{
	FILE * out = requested ? stdout : stderr;
	fputs("usage: genfsa [-e weight] [-f weight] [-hvz?]", out);
	fputs(" states symbols\n", out);
}

static void
print_version(void)
{
	puts("version 1.0");
}

static void
print_symbol(int x)
{
	/* the most trivial solution: print in base-26-LE */
	if (x == 0)
	{
		printf("0");
		return;
	}
	while (x > 26)
	{
		printf("%c", 'a' + (x % 26) - 1);
		x /= 26;
	}
	printf("%c", 'A' + x - 1);
}

#ifdef __APPLE__
static void
set_rand_seed(void)
{
	sranddev();
}
#else /* ifndef __APPLE__ */
/* mix abstracted from the public domain Bob Jenkins lookup3.c
 * http://burtleburtle.net/bob/c/lookup3.c
 */
#define rot(x,r) ((x) << (r) | (x) >> (32 - (r)))
static int
mix(unsigned int a, unsigned int b, unsigned int c)
{
	a -= c; a ^= rot(c, 4); c += b;
	b -= a; b ^= rot(a, 6); a += c;
	c -= b; c ^= rot(b, 8); b += a;
	a -= c; a ^= rot(c,16); c += b;
	b -= a; a ^= rot(a,19); a += c;
	c -= b; a ^= rot(b, 4); b += a;

	c ^= b; c -= rot(b, 14);
	a ^= c; a -= rot(c, 11);
	b ^= a; b -= rot(a, 25);
	c ^= b; c -= rot(b, 16);
	a ^= c; a -= rot(c,  4);
	b ^= a; b -= rot(a, 14);
	c ^= b; c -= rot(b, 24);
	return c;
}

static void
set_rand_seed(void)
{
	srand(mix(time(NULL), clock(), getpid()));
}
#endif /* ifndef __APPLE__ */
