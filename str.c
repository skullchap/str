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
void*	memchr(void*, int, ulong);
int	memcmp(void*, void*, ulong);
ulong	strlen(char*);
int	strcmp(char*, char*);

static	void*(*allocf)(ulong)=  	malloc;
static	void(*deallocf)(void*)= 	free;
static	void*(*realocf)(void*, ulong)=	realloc;

struct str
{
	rune	*rs;
	long	runen, n;
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
	s->rs=0;
	s->runen=0;
	s->n=n;
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
	s->rs=0;
	s->runen=0;
	s->n=n;
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
void	freestr(str *s)		{nilrunes(s); free(STRSTART(s));}
void	nilrunes(str *s)	{s=(str*)STRSTART(s); free(s->rs); s->rs=0; s->runen=0;}
char*	cstr(str *s)		{return (char*)s;}
long	strn(str *s)		{return ((str*)STRSTART(s))->n;}

void	strallocfn(void *(*fn)(unsigned long))		{allocf=fn;}
void	strdeallocfn(void (*fn)(void*))			{deallocf=fn;}
void	strreallocfn(void *(*fn)(void *, ulong))	{realocf = fn;}

str*	sdup(str *s)		{return newstrn(cstr(s), strn(s));}
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

int
runew(rune r)
{
	uchar b = *(uchar*)&r;
	if((b & 0x80) == 0){
		return 1;
	}else if((b & 0xE0) == 0xC0){
		return 2;
	}else if((b & 0xF0) == 0xE0){
		return 3;
	}else if((b & 0xF8) == 0xF0){
		return 4;
	}
	return -1;
}

rune*
runes(str *s)
{
	s = (str*)STRSTART(s);

	if(s->rs)
		return s->rs;
	runesn((str*)STREND(s));
	return s->rs;
}

long
runesn(str *s)
{
	long runen, rscap;
	uchar *p = (uchar *)s;
	s = (str*)STRSTART(s);
	runen = s->runen;

	if(runen > 0)
		return runen;

	runen=0, rscap=2;
	s->rs = allocf(sizeof(rune)*rscap);
	if(!s->rs)
		goto bad;

	while(*p != '\0'){
		rune r = *(rune*)p;
		int w = runew(r);
		if(w < 0)
			goto bad;
		p+=w;

		if(runen == rscap){
			void *nrs;
			rscap*=2;
			nrs = realocf(s->rs, sizeof(rune)*rscap);
			if(!nrs)
				goto bad;
			s->rs=nrs;
		}
		s->rs[runen++] = r;
	}
	s->runen = runen;
	return runen;
bad:
	deallocf(s->rs);
	s->runen=0;
	return -1;
}
