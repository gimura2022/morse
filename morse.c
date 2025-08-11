/* Permission to use, copy, modify, and/or distribute this software for
 * any purpose with or without fee is hereby granted.

 * THE SOFTWARE IS PROVIDED “AS IS” AND THE AUTHOR DISCLAIMS ALL
 * WARRANTIES WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE
 * FOR ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY
 * DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN
 * AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT
 * OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE. */

/* include headers */
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>
#include <ctype.h>

/* define morse codes */
#define morse_codes() \
	code('a', ".-")		\
	code('b', "-...")	\
	code('c', "-.-.")	\
	code('d', "-..")	\
	code('e', ".")		\
	code('f', "..-.")	\
	code('g', "--.")	\
	code('h', "....")	\
	code('i', "..")		\
	code('j', ".---")	\
	code('k', "-.-")	\
	code('l', ".-..")	\
	code('m', "--")		\
	code('n', "-.")		\
	code('o', "---")	\
	code('p', ".--.")	\
	code('q', "--.-")	\
	code('r', ".-.")	\
	code('s', "...")	\
	code('t', "-")		\
	code('u', "..-")	\
	code('v', "...-")	\
	code('w', ".--")	\
	code('x', "-..-")	\
	code('y', "-.--")	\
	code('z', "--..")	\
	code('1', ".----")	\
	code('2', "..---")	\
	code('3', "...--")	\
	code('4', "....-")	\
	code('5', ".....")	\
	code('6', "-....")	\
	code('7', "--...")	\
	code('8', "---..")	\
	code('9', "----.")	\
	code('0', "-----")	\
	code(' ', "/")		/* for creating separators for words */

/* current mode */
static enum {
	ENCODE = 0,	/* encoding mode */
	DECODE		/* decoging mode */
} mode = ENCODE;	/* use encoding by default */

/* encode text and print result into stdout */
static void encode(const char* text)
{
	/* for every charcter */
	for (const char* s = text; *s != '\0'; s++) {
		switch (tolower(*s)) {
		/* define code for generating cases for charcters */
#		define code(x, y) case x: printf("%s", y); break;

		/* generate cases */
		morse_codes();

		/* if charcter not found, just skip it */
		default: break;

#		undef code
		}

		/* put space after any charcter */
		putc(' ', stdout);
	}

	/* put new line */
	putc('\n', stdout);
}

/* decode text and print result to stdout */
static void decode(char* text)
{
	/* split text to segments separated using space */
	for (const char* s = strtok(text, " "); s != NULL; s = strtok(NULL, " ")) {
		/* define code for compare segment and encoded version of charcter */
#		define code(x, y) \
			if (strcmp(y, s) == 0) {	/* if match... */ 			\
				putc(x, stdout);	/* print charcter to stdout */		\
				continue;		/* and go to the next segment */	\
			}

		/* generate code */
		morse_codes();

#		undef code
	}
}

/* execute encoding or decoding using current mode */
static void execute(char* text)
{
	switch (mode) {
	case ENCODE:
		encode(text);	/* run encoding if we are in encoding mode */
		break;

	case DECODE:
		decode(text);	/* run decoging if we are in decoding mode */
		break;
	}
}

/* print morse codding table to the stdout */
static void table(void)
{
	/* define code for printing line of table */
#	define code(x, y) printf("| %c\t%s\t|\n", x, y);

	/* print header */
	printf("|---------------|\n");

	/* print body */
	morse_codes();
/* print footer */
	printf("|---------------|\n");

#	undef code
}

/* run encoding or decoding from stdin to stdout in infinity loop */
static void main_loop(void)
{
	/* variables for getline */
	size_t len;
	char* line;
	int read;

	/* main loop */
	while (true) {
		/* read line */
		if ((read = getline(&line, &len, stdin)) == -1)
			break;

		/* and execute */
		execute(line);
	}
}

/* usage text */
#define USAGE_SMALL	"usage: morse [-h][-e][-d][-t] string\n"
#define USAGE		"	-h	print help message\n" \
			"	-e	set encoding mode\n" \
			"	-d	set decoding mode\n" \
			"	-t	print morse encoding table\n" \
			"	string	string for encoding\n"

/* print usage */
static void usage(FILE* stream, bool small)
{
	fprintf(stream, small ? USAGE_SMALL : USAGE_SMALL USAGE);
}

/* main function */
int main(int argc, char* argv[])
{
	int c;

	/* process flags */
	opterr = 0;
	while ((c = getopt(argc, argv, "hedt")) != -1) switch (c) {
	/* help flag */
	case 'h':
		usage(stdout, false);
		exit(0);

	/* encode mode flag */
	case 'e':
		mode = ENCODE;
		break;

	/* decode mode flag */
	case 'd':
		mode = DECODE;
		break;

	/* print table flag */
	case 't':
		table();
		exit(0);

	/* undefined flag */
	case '?':
		usage(stderr, true);
		exit(1);
	}

	/* if program doesn't recive string to encode/decode start encoding/decoding from stdin */
	if (argv[optind] == NULL)
		main_loop();
	/* otherwise encode/decode string */
	else
		execute(argv[optind]);

	/* exit success */
	return 0;
}
