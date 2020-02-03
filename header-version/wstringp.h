#ifndef WSTRINGP_H_
#define WSTRINGP_H_
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <float.h>
#include <wchar.h>
#include <locale.h>
#include <wctype.h>
#include <ctype.h>
#include <stdbool.h>

#define ONE_MEGABYTE 1048576
#define ASCII_MAX 127

unsigned int textAnalizer_minParagraphLenth = 2;
unsigned int textAnalizer_minLengthByWord = 4;

void setMinWordLength(int len);
void setParagraphLength(int len);

wchar_t* loadData(char *fileName);

unsigned long long totalTextLength(wchar_t *mainString);
unsigned long long countPrintableCharacters(wchar_t *mainString);
unsigned long long countNonPrintableCharacters(wchar_t *mainString);
unsigned long long countDigits(wchar_t *mainString);
unsigned long long countAlphabeticCharacters(wchar_t *mainString);
unsigned long long countPunctuationCharacters(wchar_t *mainString);
unsigned long long countSpaces(wchar_t *mainString);
unsigned long long countLines(wchar_t *mainString);
unsigned long long countWideCharacters(wchar_t *mainString);
unsigned long long countWords( wchar_t mainString[] );
unsigned long long countSmallWords( wchar_t mainString[] );
unsigned long long countWideWords( wchar_t mainString[] );
unsigned long long countNumbers( wchar_t mainString[] );
unsigned long long countParagraphs( wchar_t mainString[] );

bool isWUnsignedNumber(wchar_t* number);
bool isWSignedNumber(wchar_t* number);
bool hasWideChars(wchar_t *word);

char *wideStringToString(wchar_t *wstring);
wchar_t *StringToWideString(char *stringc);

wchar_t *getPuntuationCharacters(wchar_t *mainString);
wchar_t **getWords( wchar_t mainString[] );
wchar_t **getWideWords( wchar_t mainString[] );
wchar_t **getNumbers( wchar_t mainString[] );

#endif
