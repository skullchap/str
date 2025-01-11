/* started 01.07.2025 - skullchap - MIT */

#include "str.h"

typedef unsigned char uchar;
typedef unsigned long ulong;
typedef struct FILE FILE;
enum	{SEEK_END=	2};

FILE*	fopen(char*, char*);
ulong	fread(void*, ulong, ulong, FILE*);
int	fseek(FILE*, long, int);
long	ftell(FILE*);
void	rewind(FILE*);
int	fclose(FILE*);
void*	malloc(ulong);
void	free(void*);
void*	realloc(void*, ulong);
void*	memcpy(void*, void*, ulong);
ulong	strlen(char *);
int	strcmp(char*, char*);

void*(*allocf)(ulong) = malloc;
void(*deallocf)(void*) = free;
void*(*realocf)(void*, ulong) = realloc;

struct str
{
	long n, runen;
};

#define cast(T,V)	((T)(V))
#define STREND(S)	(((uchar*)(S))+sizeof(str))
#define STRSTART(S)	(((uchar*)(S))-sizeof(str))

static void term(str *s){STREND(s)[strn((str*)STREND(s))]= '\0';}

static
str*
allocstr(long n)		/* not reusing reallocstr, cause custom global realocfn can act differently when passing nil to it */
{
	str *s = allocf(sizeof(str)+ n + 1);
	if(!s)
		return 0;
	s->n=n;
	s->runen=0;
	term(s);
	return (str*)STREND(s);
}

static
str*
reallocstr(str *s, long n)
{
	void *p = realocf(STRSTART(s), sizeof(str)+ n + 1);
	if(!p)
		return 0;
	s=p;
	s->n=n;
	s->runen=0;
	term(s);
	return (str*)STREND(s);
}

str*
newstrn(char* cstr, long n)
{
	str *s = allocstr(n);
	if(!s)
		return 0;
	memcpy(s, cstr, n);
	return s;
}

str*	newstr(char *cstr)	{return newstrn(cstr, strlen(cstr)+1);}
void	freestr(str *s)		{free(STRSTART(s));}
char*	cstr(str *s)		{return (char*)s;}
long	strn(str *s)		{return ((str*)STRSTART(s))->n;}

void	strallocfn(void *(*fn)(unsigned long))		{allocf=fn;}
void	strdeallocfn(void (*fn)(void*))			{deallocf=fn;}
void	strreallocfn(void *(*fn)(void *, ulong))	{realocf = fn;}

str*	scopy(str *s)		{return newstrn(cstr(s), strn(s));}
str*	sresize(str *s, long n)	{return reallocstr(s, n);}

str*
sreadfile(char *fpath)
{
	FILE *f;
	long n;
	str *s;

	f = fopen(fpath, "rb");
	if(!f)
		return 0;
	fseek(f, 0, SEEK_END);
	n = ftell(f);
	rewind(f);

	s = allocstr(n);
	if(!s)
		goto cleanup;

	n = fread(s, 1, n, f);
	if(n < 0)
		goto cleanup;

	fclose(f);
	return s;
cleanup:
	fclose(f);
	freestr(s);
	return 0;
}

str*
scat(str *l, str *r)
{
/* 	str *s = allocstr(strn(l) + strn(r));
	if(!s)
		return 0;
	memcpy(s, l, strn(l));
	memcpy(cast(uchar*, s)+ strn(l), r, strn(r));
	return s; */

	str *sa[2];
	sa[0]= l, sa[1]= r;
	return scatn(sa, 2);
}

str*
scatn(str *sa[], int n)
{
        str *s, sp;
	int i; 
	long slen;

	for(i=0,slen=0; i<n; ++i)
		slen += strn(sa[i]); 

	s = allocstr(slen);
	if(!s)
		return 0;

	for(i=0,slen=0; i<n; ++i){
		memcpy(cast(uchar*, s)+ slen, sa[i], strn(sa[i]));
		slen += strn(sa[i]);
	}
	return s;
}

int	scmp(str *l, str *r)	{return strcmp((char*)l, (char*)r);}

long
runen(str *s)
{
	long runen = cast(str*, STRSTART(s))->runen;
	uchar *p = (uchar *)s;

	if(runen > 0)
		return runen;

	while(*p != '\0'){
		uchar b = *p;
		if((b & 0x80) == 0){
			p += 1;
		}else if((b & 0xE0) == 0xC0){
			p += 2;
		}else if((b & 0xF0) == 0xE0){
			p += 3;
		}else if((b & 0xF8) == 0xF0){
			p += 4;
		}
		runen++;
	}
	cast(str*, STRSTART(s))->runen = runen;
	return runen;
}
