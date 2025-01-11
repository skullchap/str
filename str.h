/* started 01.07.2025 - skullchap - MIT */

typedef struct str str;

str*	newstrn(char* cstr, long n);
str*	newstr(char* cstr);
void	freestr(str *s);
char*	cstr(str *s);
long	strn(str *s);
str*    scopy(str *s);
str*	sresize(str *s, long n);
str*    sreadfile(char *fpath);

void	strallocfn(void*(*)(unsigned long));
void	strdeallocfn(void(*)(void*));
void	strreallocfn(void*(*)(void*, unsigned long));

#define str(S) newstrn((S), sizeof(S)-1)

/* todo: add all string.h funcs below */
str*	scat(str *l, str *r);           /* :D */
str*	scatn(str *sa[], int n);
int	scmp(str *l, str *r);

/* todo: add anything utf8 related below */
long	runen(str *s);
