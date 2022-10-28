/* See LICENSE file for copyright and license details. */
#include <sys/stat.h>
#include <stdarg.h>
#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include "util.h"

void
die(const char *fmt, ...)
{
	va_list ap;
	FILE *f = fopen("/home/emil/dwm-customisation/bin/log.txt", "a");

	va_start(ap, fmt);
	vfprintf(stderr, fmt, ap);
	fprintf(f, fmt, ap);
	va_end(ap);

	if (fmt[0] && fmt[strlen(fmt)-1] == ':') {
		fputc(' ', stderr);
		fputs(" ", f);
		perror(NULL);
	} else {
		fputc('\n', stderr);
		fputs("\n", f);
	}
	fclose(f);

	exit(1);
}
bool
checkFile(const char * path){
    struct stat buffer;
    return (stat(path, &buffer) == 0);
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
void echo(const char *format, ...){
	FILE *f ;
	f = fopen("/home/emil/dwm-customisation/bin/log.txt", "a");
	va_list args;
	va_start(args, format);
	vfprintf(f, format, args);
	va_end(args);
	fclose(f);
}