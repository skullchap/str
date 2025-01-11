/* started 01.07.2025 - skullchap - MIT */

#include "str.h"

typedef unsigned char uchar;
typedef unsigned long ulong;

void*	malloc(ulong);
void	free(void*);
void*	realloc(void*, ulong);
void*	memcpy(void*, const void*, ulong);
ulong	strlen(const char *);
int	strcmp(const char*, const char*);;

void*(*allocf)(ulong) = malloc;
void(*deallocf)(void*) = free;
void*(*realocf)(void*, ulong) = realloc;

struct str
{
	long	n;
};

#define STREND(S)	(((uchar*)(s))+sizeof(str))
#define STRSTART(S)	(((uchar*)(s))-sizeof(str))

static void term(str *s){STREND(s)[strn((str*)STREND(s))]= '\0';}

static
str*
allocstr(long n)
{
	str *s = allocf(sizeof(str)+ n + 1);
	if(!s)
		return 0;
	s->n=n;
	term(s);
	return s;
}

str*
newstrn(char* cstr, long n)
{
	str *s = allocstr(n);
	if(!s)
		return 0;
	memcpy(STREND(s), cstr, n);
	return (str*)STREND(s);
}

str*	newstr(char *cstr)	{return newstrn(cstr, strlen(cstr)+1);}
void	freestr(str *s)		{free(STRSTART(s));}
char*	cstr(str *s)		{return (char*)s;}
long	strn(str *s)		{return ((str*)STRSTART(s))->n;}

void	strallocfn(void *(*fn)(unsigned long))		{allocf=fn;}
void	strdeallocfn(void (*fn)(void*))			{deallocf=fn;}
void	strreallocfn(void *(*fn)(void *, ulong))	{realocf = fn;}

str*	strcopy(str *s)	{return newstrn(cstr(s), strn(s));}
str*
strize(str *s, long n)
{
	void *p = realocf(STRSTART(s), sizeof(str)+ n + 1);
	if(!p)
		return 0;
	s=p;
	s->n=n;
	term(s);
	return (str*)STREND(s);
}

str*
sstrcat(str *l, str *r)
{
	str *s = allocstr(strn(l) + strn(r));
	if(!s)
		return 0;
	memcpy(STREND(s), l, strn(l));
	memcpy(&STREND(s)[strn(l)], r, strn(r));
	return (str*)STREND(s);
}

int	sstrcmp(str *l, str *r)	{return strcmp((char*)l, (char*)r);}

long
runen(str *s)
{
	long runen = 0;
	uchar *ptr = (uchar *)s;

	while(*ptr != '\0'){
		uchar b = *ptr;
		if((b & 0x80) == 0){
			ptr += 1;
		}else if((b & 0xE0) == 0xC0){
			ptr += 2;
		}else if((b & 0xF0) == 0xE0){
			ptr += 3;
		}else if((b & 0xF8) == 0xF0){
			ptr += 4;
		}
		runen++;
	}
	return runen;
}
