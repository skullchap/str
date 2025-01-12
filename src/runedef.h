typedef unsigned int Rune; 

enum
{
	UTFmax=		4,		/* maximum bytes per rune */
	Runesync=	0x80,		/* cannot represent part of a UTF sequence (<) */
	Runeself=	0x80,		/* rune and UTF sequences are the same (<) */
	Runeerror=	0xFFFD,		/* decoding error in UTF */
	Runemax=	0x10FFFF	/* 21-bit rune */
};

Rune	tolowerrune(Rune);
Rune	totitlerune(Rune);
Rune	toupperrune(Rune);
int	isalpharune(Rune);
int	islowerrune(Rune);
int	isspacerune(Rune);
int	istitlerune(Rune);
int	isupperrune(Rune);
int	runetochar(char*, Rune*);
int	chartorune(Rune*, char*);
int	runelen(long);
int	runenlen(Rune*, int);
int	fullrune(char*, int);
