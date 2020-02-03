#include "wstringp.h"
#define ONE_MEGABYTE 1048576

//Gets a single wchar_t string form a file
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

//sets the minimum length in characters for a string to be considered a paragraph.
void setParagraphLength(int len)
{
	textAnalizer_minParagraphLenth = len;
}

//sets the minimum length in characters for a string to be considered a word.
void setMinWordLength(int len)
{
	textAnalizer_minLengthByWord = len;
}

//returns the total length of the string.
unsigned long long totalTextLength(wchar_t *mainString)
{
	return wcslen(mainString);
}

//Counts all the printable characters.
unsigned long long countPrintableCharacters(wchar_t *mainString)
{
	unsigned long long count = 0, length;
	
	length = wcslen(mainString);
	for(unsigned long long i = 0; i < length; i++)
		if( iswprint( mainString[i] ) || isprint( mainString[i]) )
			count++;
	return count;
}

//Counts all the whitespace characters.
unsigned long long countNonPrintableCharacters(wchar_t *mainString)
{
	unsigned long long count = 0, length;
	
	length = wcslen(mainString);
	for(unsigned long long i = 0; i < length; i++)
		if( iswspace( mainString[i]  ) || isspace( mainString[i]  ) )
			count++;
	return count;
}

//Counts all the inidividual digits (0-9) 
unsigned long long countDigits(wchar_t *mainString)
{
	unsigned long long count = 0, length;
	
	length = wcslen(mainString);
	for(unsigned long long i = 0; i < length; i++)
		if( iswdigit( mainString[i] ) )
			count++;
	return count;
}

//Counts all the inidividual alphabetic letters (a-z or A-Z) 
unsigned long long countAlphabeticCharacters(wchar_t *mainString)
{
	unsigned long long count = 0, length;

	length = wcslen(mainString);

	for(unsigned long long i = 0; i < length; i++)
		if( iswalpha( mainString[i] ) )
			count++;

	return count;
}

//Counts all the inidividual punctuation characters
unsigned long long countPunctuationCharacters(wchar_t *mainString)
{
	unsigned long long count = 0, length;
	
	length = wcslen(mainString);
	for(unsigned long long i = 0; i < length; i++)
		if( iswpunct( mainString[i] ) )
			count++;
	return count;
}

//Gets all the inidividual punctuation characters concatenated in a string (including repeated ones).
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

//Counts all the inidividual spaces
unsigned long long countSpaces(wchar_t *mainString)
{
	unsigned long long count = 0, length;
	
	length = wcslen(mainString);
	for(unsigned long long i = 0; i < length; i++)
		if( mainString[i] == L' ' )
			count++;
	return count;
}

//Counts all the inidividual lines (new lines)
unsigned long long countLines(wchar_t *mainString)
{
	unsigned long long count = 0, length;
	
	length = wcslen(mainString);
	for(unsigned long long i = 0; i < length; i++)
		if( mainString[i] == L'\n' )
			count++;
	return count;
}

//Counts all the inidividual wide characters characters
unsigned long long countWideCharacters(wchar_t *mainString)
{
	unsigned long long count = 0, length;
	
	length = wcslen(mainString);
	for(unsigned long long i = 0; i < length; i++)
		if( ((wint_t)mainString[i]) > ASCII_MAX )
			count++;
	return count;
}

//Counts all the inidividual words that have at least one printable character.
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
//Counts all the inidividual words that its length is less than the minimum for a string to be considered a word, default is 4.
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

//Counts all the inidividual words that includes at least one wide characters characters
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

//gets all the whole numbers like 1234 or 123.898
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
		if( (isWUnsignedNumber( word ) || isWSignedNumber( word )) )
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

//Counts all the whole numbers like 1234 or 123.898 or +7873
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
		if( isWUnsignedNumber( word ) || isWSignedNumber( word ) )
			count++;
		word = wcstok (NULL, L" ", tokBuff);
	}
	
	return count;	
}

//Counts all the inidividual paragraphs that its length is greater or equal than the minimum for a string to be considered a paragraph, default is 30.
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

//Checks whether the passed number (as string) is a valid UNSIGNED integer or float type of number.
bool isWUnsignedNumber(wchar_t* number)
{
	if(number == NULL)
		return false;

	int len = wcslen(number);
	bool point = false;

 	if(len == 0)
		return false;

	for( int index = 0; index < len; index++ )
		if( !iswdigit(number[index]) )
		{
			if( !point && number[index] == L'.' && index != 0 && index != len)
				point = true;
			else
				return false;
		}
	return true;		
}

//Checks whether the passed number (as string) is a valid SIGNED integer or float type of number.
bool isWSignedNumber(wchar_t* number)
{
	if(number == NULL)
		return false;

	int len = wcslen(number);
	bool sign, point = false;

 	if(len == 0)
		return false;
	
	sign = (number[0] == L'+' || number[0] == L'-') ? true : false;
	for( int index = 0; index < len; index++ )
		if( !iswdigit(number[index]) )
		{
			if( !point && number[index] == L'.' && index != 0 && index != len)
				point = true;
			else
				return false;
		}
	return sign;		
}

//Checks whether the passed word consists only of ASCII characters; returns false on any occurrence of any character with a value grater than 127
bool hasWideChars(wchar_t *word)
{
	if(word == NULL)
		return false;

 	if(wcslen(word) == 0)
		return false;

	for(int i = 0; i<wcslen(word); i++)		
		if( ( (wint_t)word[i] ) > ASCII_MAX )
			return true;
	return false;
}


