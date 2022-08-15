/* See LICENSE file for copyright and license details. */
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "util.h"

void
die(const char *fmt, ...)
{
	va_list ap;

	va_start(ap, fmt);
	vfprintf(stderr, fmt, ap);
	va_end(ap);

	if (fmt[0] && fmt[strlen(fmt)-1] == ':') {
		fputc(' ', stderr);
		perror(NULL);
	} else {
		fputc('\n', stderr);
	}

	exit(1);
}

void *
ecalloc(size_t nmemb, size_t size)
{
	void *p;

	if (!(p = calloc(nmemb, size)))
		die("calloc:");
	return p;
}

void
printBits(size_t const size, void const * const ptr)
{
    unsigned char *b = (unsigned char*) ptr;
    unsigned char byte;
    int i, j;
    
    for (i = size-1; i >= 0; i--) {
        for (j = 7; j >= 0; j--) {
            byte = (b[i] >> j) & 1;
			if (byte) 
				system("/home/emil/xlog 1");
			else
				system("/home/emil/xlog 0");
        }
		system("/home/emil/xlog \\\\n");
    }
}
void
writeBitsToFile(size_t const size, void const * const ptr, const char *path)
{
    unsigned char *b = (unsigned char*) ptr;
    unsigned char byte;
    int i, j;
    FILE *f = fopen(path, "a");

    for (i = size-1; i >= 0; i--) {
        for (j = 7; j >= 0; j--) {
            byte = (b[i] >> j) & 1;
            fprintf(f, "%u", byte);
        }
		// fputs(" ", f);
    }
    fputs("\n", f);
    fclose(f);
}
void
writeToFile(const char *text, const char *path, int append){
	FILE *f ;
	if (append)
		f = fopen(path, "a");
	else
		f= fopen(path, "w");
	fputs(text, f);
	fclose(f);
}
void
writeIntToFile(const int number, const char *path, int append){
	FILE *f ;
	if (append)
		f = fopen(path, "a");
	else
		f= fopen(path, "w");
	fprintf(f, "%d", number);
	fclose(f);
}