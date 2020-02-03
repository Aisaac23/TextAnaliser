#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <float.h>
#include <wchar.h>
#include <locale.h>
#include <wctype.h>
#include <stdbool.h>
#include "chkops.h"
#define N_OPTIONS 24
#define ONE_MEGABYTE 1048576

unsigned int textAnalizer_minParagraphLenth = 2;
unsigned int textAnalizer_minLengthByWord = 4;
wchar_t *textAnalizer_mainString = NULL;
FILE *textAnalizer_stream = NULL;

void setMinWordLength(int len);
void setParagraphLength(int len);
void setOutputStream(char *fileName);
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
char *wideStringToString(wchar_t *wstring);
wchar_t *StringToWideString(char *stringc);

wchar_t *getPuntuationCharacters(wchar_t *mainString);
wchar_t **getWords( wchar_t mainString[] );
wchar_t **getWideWords( wchar_t mainString[] );
wchar_t **getNumbers( wchar_t mainString[] );

int menu(int selected);
int setGlobalSettings(int selected, char *arg);

int main(int argc, char * argv[])
{
	setlocale(LC_ALL,"");
	textAnalizer_stream = stdout;

	if(argc < 3)
	{
		printf("Arguments missing or with the wrong format in: %s\n", argv[0]);
		exit(EXIT_SUCCESS);
	}

	int selected;
	char **options, *fileName;
	char optionsString[] = "-file -locale -paragraph -wordLength -output -all -length -lines -alpha -digits -punct -white -spaces -Wchars"\
				" -paragraphs -numbers -words -sWords -print -Wwords -gWords -gWwords -gPunct -gNumbers";
	options = (char**) calloc( N_OPTIONS, sizeof(char*) );
	setAvailableOptions(options, optionsString);
	
	for(int index = 0; index < argc; index++)
	{
		if(argv[index][0] == '-')
		{
			selected = findMainOptions(N_OPTIONS, options, argv[index]);
			if(selected < 5)
			{
				if( setGlobalSettings(selected, argv[index+1]) && selected == 0)
					fileName = argv[index+1];
			}
			else
			{
				if(textAnalizer_mainString == NULL)
					textAnalizer_mainString = loadData(fileName);
				menu(selected);
			}
		}
	}
			
	return EXIT_SUCCESS;
}

int setGlobalSettings(int selected, char *arg)
{
	switch(selected)
	{
		case 0:
			if(!fileExists(arg))
				return -1;
		break;

		case 1:
			if( setlocale(LC_ALL, arg) == NULL )
				return -2;
		break;

		case 2:
			if( isUnsignedInteger( arg ) )
				setParagraphLength( atoi( arg ) );
			else
				printf("Couldn't set %s to a paragraph length.\n", arg );
		break;

		case 3:
			if( isUnsignedInteger( arg ) )
				setMinWordLength( atoi(arg) );
			else
				printf("Couldn't set %s to a word length.\n", arg );
		break;
		
		case 4:
			if( !isFileName( arg ) )
				return -3;
			else
				setOutputStream( arg );
		break;

		default:
			return -1;
	}

	return 1;
}

int menu(int selected)
{
	wchar_t **words = NULL, *punctChars = NULL;
	unsigned long long index = 0;
	switch(selected)
	{
		case 5:
			for(int index = selected+1; index < N_OPTIONS; index++ )
				menu(index);
		break;

		case 6:
			fwprintf(textAnalizer_stream, L"-Total text length (characters): %llu\n", totalTextLength(textAnalizer_mainString) );
		break;

		case 7:
			fwprintf(textAnalizer_stream, L"-Lines: %llu\n", countLines(textAnalizer_mainString) );
		break;

		case 8:
			fwprintf(textAnalizer_stream, L"-Alphabetic characters: %llu\n", countAlphabeticCharacters(textAnalizer_mainString) );
		break;

		case 9:
			fwprintf(textAnalizer_stream, L"-Digits: %llu\n", countDigits(textAnalizer_mainString) );
		break;

		case 10:
			fwprintf(textAnalizer_stream, L"-Punctuation characters: %llu\n", countPunctuationCharacters(textAnalizer_mainString) );
		break;

		case 11:
			fwprintf(textAnalizer_stream, L"-White characters: %llu\n", countNonPrintableCharacters(textAnalizer_mainString) );
		break;

		case 12:
			fwprintf(textAnalizer_stream, L"-Spaces: %llu\n", countSpaces(textAnalizer_mainString) );
		break;

		case 13:
			fwprintf(textAnalizer_stream, L"-Wide Characters: %llu\n", countWideCharacters(textAnalizer_mainString) );
		break;

		case 14:
			fwprintf(textAnalizer_stream, L"-Paragraphs (%u characters or more): %llu\n", textAnalizer_minParagraphLenth, countParagraphs(textAnalizer_mainString) );
		break;

		case 15:
			fwprintf(textAnalizer_stream, L"-Whole numbers: %llu\n", countNumbers(textAnalizer_mainString) );
		break;

		case 16:
			fwprintf(textAnalizer_stream, L"-Words: %llu\n", countWords(textAnalizer_mainString) );
		break;

		case 17:
			fwprintf(textAnalizer_stream, L"-Small words (less than %u characters): %llu\n", textAnalizer_minLengthByWord, countSmallWords(textAnalizer_mainString));
		break;

		case 18:
			fwprintf(textAnalizer_stream, L"-Printable characters: %llu\n", countPrintableCharacters(textAnalizer_mainString) );
		break;

		case 19:
			fwprintf(textAnalizer_stream, L"-Words with wide characters: %llu\n", countWideWords(textAnalizer_mainString) );
		break;

		case 20:
			fwprintf(textAnalizer_stream, L"-Words:\n");
			words = getWords(textAnalizer_mainString);
			while( words[index] != NULL )
			//while(index < 100)
			{
				fwprintf(textAnalizer_stream, L"%ls,", words[index] );
				index++;
			}
			fwprintf(textAnalizer_stream, L"\n");
		break;

		case 21:
			fwprintf(textAnalizer_stream, L"\n-Words with wide characters:\n");
			words = getWideWords(textAnalizer_mainString);
			while( words[index] != NULL )
			//while(index < 100)
			{
				fwprintf(textAnalizer_stream, L"%ls,", words[index] );
				index++;
			}
			fwprintf(textAnalizer_stream, L"\n");
		break;
		case 22:
			fwprintf(textAnalizer_stream, L"\n-Punctuation characters:\n");
			punctChars = getPuntuationCharacters(textAnalizer_mainString);
			while( words[index] != NULL )
			//while(index < 100)
			{
				fwprintf(textAnalizer_stream, L"%ls,", punctChars );
				index++;
			}
			fwprintf(textAnalizer_stream, L"\n");
		break;
		case 23:
			fwprintf(textAnalizer_stream, L"\n-Whole Numbers:\n");
			words = getNumbers(textAnalizer_mainString);
			while( words[index] != NULL )
			//while(index < 100)
			{
				fwprintf(textAnalizer_stream, L"%ls,", words[index] );
				index++;
			}
			fwprintf(textAnalizer_stream, L"\n");
		break;
	}

	return 1;
}

wchar_t* loadData(char *fileName)
{
	FILE *bigText;

	bigText = fopen(fileName, "r");
	wchar_t ch[2], *fileString = NULL;
	wint_t wc = 0;
	unsigned long long size = 0;
	
	if( bigText != NULL)
	{
		while( (wc = fgetwc(bigText)) != EOF )
			size++;

		fileString = calloc( size+1, sizeof(wchar_t) );
	
		ch[1] = '\0';
		rewind(bigText);
		
		size = 0;
		while( (ch[0] = (wchar_t)fgetwc(bigText)) != EOF )
			fileString[size++] = ch[0];
		fileString[size] = L'\0';
	}
	else
		printf("null!");

	return fileString;
}

void setOutputStream(char *fileName)
{
	textAnalizer_stream = fopen(fileName, "a");
}

void setParagraphLength(int len)
{
	textAnalizer_minParagraphLenth = len;
}

void setMinWordLength(int len)
{
	textAnalizer_minLengthByWord = len;
}

unsigned long long totalTextLength(wchar_t *mainString)
{
	return wcslen(mainString);
}

unsigned long long countPrintableCharacters(wchar_t *mainString)
{
	unsigned long long count = 0, length;
	
	length = wcslen(mainString);
	for(unsigned long long i = 0; i < length; i++)
		if( iswprint( mainString[i] ) || isprint( mainString[i]) )
			count++;
	return count;
}

unsigned long long countNonPrintableCharacters(wchar_t *mainString)
{
	unsigned long long count = 0, length;
	
	length = wcslen(mainString);
	for(unsigned long long i = 0; i < length; i++)
		if( iswspace( mainString[i]  ) || isspace( mainString[i]  ) )
			count++;
	return count;
}

unsigned long long countDigits(wchar_t *mainString)
{
	unsigned long long count = 0, length;
	
	length = wcslen(mainString);
	for(unsigned long long i = 0; i < length; i++)
		if( iswdigit( mainString[i] ) )
			count++;
	return count;
}

unsigned long long countAlphabeticCharacters(wchar_t *mainString)
{
	unsigned long long count = 0, length;

	length = wcslen(mainString);

	for(unsigned long long i = 0; i < length; i++)
		if( iswalpha( mainString[i] ) )
			count++;

	return count;
}

unsigned long long countPunctuationCharacters(wchar_t *mainString)
{
	unsigned long long count = 0, length;
	
	length = wcslen(mainString);
	for(unsigned long long i = 0; i < length; i++)
		if( iswpunct( mainString[i] ) )
			count++;
	return count;
}

wchar_t * getPuntuationCharacters(wchar_t *mainString)
{
	unsigned long long count = 0, length;
	wchar_t *chars;
	
	count = countPunctuationCharacters(mainString);
	chars = (wchar_t*)calloc( count+1, sizeof(wchar_t) );
	chars[count] = L'\0';	
	
	length = wcslen(mainString);
	for(unsigned long long i = 0; i < length; i++)
		if( iswpunct( mainString[i] ) )
			chars[count] = mainString[i];
	return chars;
}

unsigned long long countSpaces(wchar_t *mainString)
{
	unsigned long long count = 0, length;
	
	length = wcslen(mainString);
	for(unsigned long long i = 0; i < length; i++)
		if( mainString[i] == L' ' )
			count++;
	return count;
}

unsigned long long countLines(wchar_t *mainString)
{
	unsigned long long count = 0, length;
	
	length = wcslen(mainString);
	for(unsigned long long i = 0; i < length; i++)
		if( mainString[i] == L'\n' )
			count++;
	return count;
}

unsigned long long countWideCharacters(wchar_t *mainString)
{
	unsigned long long count = 0, length;
	
	length = wcslen(mainString);
	for(unsigned long long i = 0; i < length; i++)
		if( ((wint_t)mainString[i]) > ASCII_MAX )
			count++;
	return count;
}

unsigned long long countWords( wchar_t *mainString )
{
	wchar_t *word, **tokBuff, *toMakeTokens;
	unsigned long long count = 0, length;
	bool printable = false;
	
	length = wcslen(mainString);
	tokBuff = (wchar_t**)calloc(ONE_MEGABYTE, sizeof(wchar_t*));
	toMakeTokens = (wchar_t*)calloc(length, sizeof(wchar_t) );	
	wcscpy(toMakeTokens, mainString);
	toMakeTokens[length] = '\0';

	word = wcstok(toMakeTokens, L" ", tokBuff);
	while( word != NULL )
	{
		length = 0;
		while( *(word+length) != '\0' && printable == false)
		{
			if( !iswspace( *(word+length) ) && !isspace( *(word+length) ) )
				printable = true;
			length++;
		}
		if(printable)
			count++;
		word = wcstok (NULL, L" ", tokBuff);
		printable = false;
	}
	return count;	
}

unsigned long long countSmallWords( wchar_t *mainString )
{
	wchar_t *word, **tokBuff, *toMakeTokens;
	unsigned long long count = 0, length;
	bool printable = false;

	length = wcslen(mainString);
	tokBuff = (wchar_t**)calloc(ONE_MEGABYTE, sizeof(wchar_t*));
	toMakeTokens = (wchar_t*)calloc(length, sizeof(wchar_t) );
	wcscpy(toMakeTokens, mainString);
	toMakeTokens[length] = L'\0';

	word = wcstok(toMakeTokens, L" ", tokBuff);
	while( word != NULL )
	{
		length = 0;
		while( *(word+length) != '\0' && printable == false)
		{
			if( !iswspace( *(word+length) ) && !isspace( *(word+length) ) )
				printable = true;
			length++;
		}

		if( wcslen(word) < textAnalizer_minLengthByWord && printable)
			count++;
		word = wcstok (NULL, L" ", tokBuff);
		printable = false;
	}
	
	return count;	
}

unsigned long long countWideWords( wchar_t *mainString )
{
	wchar_t *word, **tokBuff, *toMakeTokens;
	unsigned long long count = 0, length;
	bool printable = false;
	
	length = wcslen(mainString);
	tokBuff = (wchar_t**)calloc(ONE_MEGABYTE, sizeof(wchar_t*));
	toMakeTokens = (wchar_t*)calloc(length, sizeof(wchar_t) );
	wcscpy(toMakeTokens, mainString);
	toMakeTokens[length] = '\0';

	word = wcstok(toMakeTokens, L" ", tokBuff);
	while( word != NULL )
	{
		length = 0;
		while( *(word+length) != '\0' && printable == false)
		{
			if( !iswspace( *(word+length) ) && !isspace( *(word+length) ) )
				printable = true;
			length++;
		}

		if( hasWideChars(word) && printable)
			count++;
		word = wcstok (NULL, L" ", tokBuff);
		printable = false;
	}
	
	return count;	
}


/*NOTICE: getWords() IS dependent of countWords () */
wchar_t **getWords( wchar_t *mainString )
{	
	wchar_t *word, **allWords, **tokBuff, *toMakeTokens;
	unsigned long long count = 0, length, len;
	bool printable = false;	

	length = wcslen(mainString);
	tokBuff = (wchar_t**)calloc(ONE_MEGABYTE, sizeof(wchar_t*));
	toMakeTokens = (wchar_t*)calloc(length, sizeof(wchar_t) );
	wcscpy(toMakeTokens, mainString);
	toMakeTokens[length] = '\0';

	count  = countWords(mainString);
	allWords = (wchar_t **)calloc( count+1, sizeof(wchar_t*) );	
	

	word = wcstok(toMakeTokens, L" ", tokBuff);
	count = 0;
	while( word != NULL )
	{
		length = 0;
		while( *(word+length) != '\0' && printable == false)
		{
			if( !iswspace( *(word+length) ) && !isspace( *(word+length) ) )
				printable = true;
			length++;
		}

		if(printable)
		{
			len = wcslen(word);
			allWords[count] = (wchar_t*)calloc(len+1, sizeof(wchar_t));
			wcscpy( allWords[count], word );
			count++;
		}
		printable = false;
		word = wcstok (NULL, L" ", tokBuff);
	}
	allWords[count] = NULL;

	return allWords;
}

/*NOTICE: getWideWords() IS dependent of countWords () */
wchar_t **getWideWords( wchar_t *mainString )
{	
	wchar_t *word, **allWideWords, **tokBuff, *toMakeTokens;
	unsigned long long count = 0, length, len;
	bool printable = false;
	
	length = wcslen(mainString);
	tokBuff = (wchar_t**)calloc(ONE_MEGABYTE, sizeof(wchar_t*));
	toMakeTokens = (wchar_t*)calloc(length, sizeof(wchar_t) );
	wcscpy(toMakeTokens, mainString);
	toMakeTokens[length] = '\0';
	
	count  = countWideWords(mainString);
	allWideWords = (wchar_t **)calloc( count+1, sizeof(wchar_t*) );	
	
	word = wcstok(toMakeTokens, L" ", tokBuff);
	count = 0;
	while( word != NULL )
	{
		length = 0;
		while( *(word+length) != '\0' && printable == false)
		{
			if( !iswspace( *(word+length) ) && !isspace( *(word+length) ) )
				printable = true;
			length++;
		}

		if(printable && hasWideChars(word))
		{
			len = wcslen(word);
			allWideWords[count] = (wchar_t*)calloc(len+1, sizeof(wchar_t));
			wcscpy( allWideWords[count], word );
			count++;
		}

		printable = false;
		word = wcstok (NULL, L" ", tokBuff);
	}
	allWideWords[count] = NULL;

	return allWideWords;
}

wchar_t **getNumbers( wchar_t *mainString )
{	
	wchar_t *word, **allNumbers, **tokBuff, *toMakeTokens;
	unsigned long long count = 0, length, len;
	
	length = wcslen(mainString);
	tokBuff = (wchar_t**)calloc(ONE_MEGABYTE, sizeof(wchar_t*));
	toMakeTokens = (wchar_t*)calloc(length, sizeof(wchar_t) );
	wcscpy(toMakeTokens, mainString);
	toMakeTokens[length] = '\0';
	
	count  = countWords(mainString);
	allNumbers = (wchar_t **)calloc( count+1, sizeof(wchar_t*) );	
	
	word = wcstok(toMakeTokens, L" ", tokBuff);
	count = 0;
	while( word != NULL )
	{
		if( (isUnsignedNumber( wideStringToString(word) ) || isSignedNumber( wideStringToString(word) )) )
		{
			len = wcslen(word);
			allNumbers[count] = (wchar_t*)calloc(len+1, sizeof(wchar_t));
			wcscpy( allNumbers[count], word );
			count++;
		}

		word = wcstok (NULL, L" ", tokBuff);
	}
	allNumbers[count] = NULL;

	return allNumbers;
}

unsigned long long countNumbers( wchar_t *mainString )
{
	wchar_t *word, **tokBuff, *toMakeTokens;
	unsigned long long count = 0, length;
	
	length = wcslen(mainString);
	tokBuff = (wchar_t**)calloc(ONE_MEGABYTE, sizeof(wchar_t*));
	toMakeTokens = (wchar_t*)calloc(length, sizeof(wchar_t) );
	wcscpy(toMakeTokens, mainString);
	toMakeTokens[length] = '\0';

	word = wcstok(toMakeTokens, L" ", tokBuff);
	while( word != NULL )
	{
		if( isUnsignedNumber( wideStringToString(word) ) || isSignedNumber( wideStringToString(word) ) )
			count++;
		word = wcstok (NULL, L" ", tokBuff);
	}
	
	return count;	
}

unsigned long long countParagraphs( wchar_t *mainString )
{
	wchar_t *paragraph, **tokBuff, *toMakeTokens;
	unsigned long long count = 0, length, plen;
	
	length = wcslen(mainString);
	tokBuff = (wchar_t**)calloc(ONE_MEGABYTE, sizeof(wchar_t*));
	toMakeTokens = (wchar_t*)calloc(length, sizeof(wchar_t) );
	wcscpy(toMakeTokens, mainString);
	toMakeTokens[length] = L'\0';

	paragraph = wcstok(toMakeTokens, L"\n",  tokBuff);
	while (paragraph != NULL)
	{
		plen = wcslen(paragraph);	
		if( plen >= textAnalizer_minParagraphLenth && paragraph[plen-2] == L'.' )
			count++;
		paragraph = wcstok (NULL, L"\n", tokBuff);
	}
	return count;
}

/*NOTICE: this function is not safe if you there are non-ASCII characters in the passed string.*/
char *wideStringToString(wchar_t *wstring)
{
	unsigned long long length = wcslen( wstring );
	char *buffer;
	
	buffer = calloc( length, sizeof(char) );
	for(unsigned long long index = 0; index < length; index++)
		buffer[index] = (char)wctob(wstring[index]);	
	return buffer;		
}

wchar_t *StringToWideString(char *stringc)
{
	unsigned long long length = strlen( stringc );
	wchar_t *buffer;
	
	buffer = calloc( length, sizeof(wchar_t) );
	for(unsigned long long index = 0; index < length; index++)
		buffer[index] = (char)btowc(stringc[index]);	
	return buffer;		
}

