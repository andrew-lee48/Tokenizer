/*
 * tokenizer.c
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// enumerations for each state of the tokenizer
enum state_ID_t {	ENTRY_STATE, SCANNING_STATE, ALPHANUM_STATE, ALPHANUM_TOKEN_STATE,
					DECIMAL_STATE, DECIMAL_TOKEN_STATE,
					OCTAL_STATE, OCTAL_TOKEN_STATE,
					HEXADECIMAL_STATE, HEXADECIMAL_TOKEN_STATE,
					FLOATING_POINT_STATE, FLOATING_POINT_EXPONENT_STATE, FLOATING_POINT_TOKEN_STATE,
					OPERATOR_STATE, OPERATOR_TOKEN_STATE,
					MALFORMED_TOKEN_STATE,
					EXIT_STATE	};

/*
 * Tokenizer type.  You need to fill in the type as part of your implementation.
 */

struct TokenizerT_
{
	char* tokenStream;	// pointer to char array (i.e. input)
	int tokenStreamSize;	// size of the char array
	int index;	// counter for parsing char array
	int tokenSize;	// counter for measuring size of tokens
};

typedef struct TokenizerT_ TokenizerT;

TokenizerT* tokenizer;

/*
 * TKCreate creates a new TokenizerT object for a given token stream
 * (given as a string).
 * 
 * TKCreate should copy the arguments so that it is not dependent on
 * them staying immutable after returning.  (In the future, this may change
 * to increase efficiency.)
 *
 * If the function succeeds, it returns a non-NULL TokenizerT.
 * Else it returns NULL.
 *
 * You need to fill in this function as part of your implementation.
 */

TokenizerT *TKCreate( char * ts )
{
	TokenizerT *tokenizer;

	tokenizer = (TokenizerT *) malloc( sizeof(TokenizerT) );	// dynamic allocation for TokenizerT

	tokenizer->tokenStream = ts;
	tokenizer->tokenStreamSize = strlen(ts);
	tokenizer->index = 0;
	tokenizer->tokenSize = 0;

	if (tokenizer != NULL) {
		return tokenizer;
	}

	return NULL;
}

/*
 * TKDestroy destroys a TokenizerT object.  It should free all dynamically
 * allocated memory that is part of the object being destroyed.
 *
 * You need to fill in this function as part of your implementation.
 */

void TKDestroy( TokenizerT * tk )
{
	free(tk);
}

/*
 * TKGetNextToken returns the next token from the token stream as a
 * character string.  Space for the returned token should be dynamically
 * allocated.  The caller is responsible for freeing the space once it is
 * no longer needed.
 *
 * If the function succeeds, it returns a C string (delimited by '\0')
 * containing the token.  Else it returns 0.
 *
 * You need to fill in this function as part of your implementation.
 */

char *TKGetNextToken( TokenizerT * tk )
{
	char* returnToken;

	returnToken = (char * )malloc(sizeof(char) * tokenizer->tokenSize + 1);	// returnToken is tokenSize + 1 to allow for '\0'

	if (returnToken == NULL) {
		printf("I returned a null token!\n");
		return 0;	// this is a fail
	}

	// construct the token in the new string
	int count;
	/*
	 * Run a for loop for as many chars the return token char string will contain. The input stream is
	 * scanned from indices before the current position to get chars from the beginning of the token.
	 */
	for (count = tokenizer->tokenSize; count > 0; count--)
	{
		// Place character from former index in the input stream into token char array
		returnToken[tokenizer->tokenSize - count] = tokenizer->tokenStream[tokenizer->index - count];
	}
	returnToken[tokenizer->tokenSize] = '\0';

	return returnToken;
}

/*
 * Returns 1 if input char is octal digit, 0 otherwise.
 */

int isOctal(char input)
{
	if (isdigit(input) && input != '8' && input != '9') {
		return 1;
	}
	else {
		return 0;
	}
}

/*
 * Returns 1 if input char is a C operator symbol, 0 otherwise.
 */

int isOperator(char input)
{
	if (	   input == '+' || input == '-' || input == '*' || input == '/' || input == '%'
			|| input == '[' || input == ']' || input == '(' || input == ')' || input == '.'
			|| input == '!' || input == '&' || input == '~' || input == '='
			|| input == '^' || input == '|' || input == '<' || input == '>') {
		return 1;
	}
	else {
		return 0;
	}
}

/*
 * Returns 1 if input char sequence matches a C operator sequence, 0 otherwise.
 */

int isCOperator(char* input)
{
	// There are 40 C operators in this giant if-statement
	if (	   strcmp(input, "+") == 0 || strcmp(input, "-") == 0 || strcmp(input, "*") == 0
			|| strcmp(input, "/") == 0 || strcmp(input, "%") == 0 || strcmp(input, "[") == 0
			|| strcmp(input, "]") == 0 || strcmp(input, "(") == 0 || strcmp(input, ")") == 0
			|| strcmp(input, "!") == 0 || strcmp(input, "&") == 0 || strcmp(input, "~") == 0
			|| strcmp(input, "=") == 0 || strcmp(input, "^") == 0 || strcmp(input, "|") == 0
			|| strcmp(input, "<") == 0 || strcmp(input, ">") == 0 || strcmp(input, ".") == 0
			|| strcmp(input, "++") == 0 || strcmp(input, "--") == 0 || strcmp(input, "==") == 0
			|| strcmp(input, "!=") == 0 || strcmp(input, "<=") == 0 || strcmp(input, ">=") == 0
			|| strcmp(input, "&&") == 0 || strcmp(input, "||") == 0 || strcmp(input, ">>") == 0
			|| strcmp(input, "<<") == 0 || strcmp(input, "+-") == 0 || strcmp(input, "+=") == 0
			|| strcmp(input, "-=") == 0 || strcmp(input, "*=") == 0 || strcmp(input, "/=") == 0
			|| strcmp(input, "%/") == 0 || strcmp(input, "&=") == 0 || strcmp(input, "^=") == 0
			|| strcmp(input, "|=") == 0 || strcmp(input, "->") == 0
			|| strcmp(input, ">>=") == 0 || strcmp(input, "<<=") == 0) {
		return 1;
	}
	return 0;
}
/*
 * Returns a pointer to a char array representing the operator the input character represents.
 * This function returns NULL if the argument doesn't match a valid C operator.
 */

char* getOperator(char* input)
{
	char* returnString;

	if ( strcmp(input, "+") == 0 ) {
		returnString = "addition";
	}
	else if ( strcmp(input, "-") == 0 ) {
		returnString = "subtraction";
	}
	else if ( strcmp(input, "*") == 0 ) {
		returnString = "multiplication";
	}
	else if ( strcmp(input, "/") == 0 ) {
		returnString = "division";
	}
	else if ( strcmp(input, "%") == 0 ) {
		returnString = "modulo";
	}	// 5 operators
	else if ( strcmp(input, "[") == 0) {
		returnString = "left brace";
	}
	else if ( strcmp(input, "]") == 0) {
		returnString = "right brace";
	}
	else if ( strcmp(input, "(") == 0) {
		returnString = "left parenthesis";
	}
	else if ( strcmp(input, ")") == 0) {
		returnString = "right parenthesis";
	}
	else if ( strcmp(input, "!") == 0) {
		returnString = "negation";
	}	// 10 operators
	else if ( strcmp(input, "&") == 0) {
		returnString = "address";
	}
	else if ( strcmp(input, "~") == 0) {
		returnString = "1's complement";
	}
	else if ( strcmp(input, "=") == 0) {
		returnString = "assignment";
	}
	else if ( strcmp(input, "^") == 0) {
		returnString = "bitwise exclusive or";
	}
	else if ( strcmp(input, "|") == 0) {
		returnString = "bitwise or";
	}	// 15 operators
	else if ( strcmp(input, "<") == 0) {
		returnString = "less than";
	}
	else if ( strcmp(input, ">") == 0) {
		returnString = "greater than";
	}
	else if ( strcmp(input, ".") == 0) {
		returnString = "structure member";
	}
	else if ( strcmp(input, "++") == 0) {
		returnString = "increment";
	}
	else if ( strcmp(input, "--") == 0) {
		returnString = "decrement";
	}	// 20 operators
	else if ( strcmp(input, "==") == 0) {
		returnString = "equals";
	}
	else if ( strcmp(input, "!=") == 0) {
		returnString = "not equals";
	}
	else if ( strcmp(input, "<=") == 0) {
		returnString = "less than or equals";
	}
	else if ( strcmp(input, ">=") == 0) {
		returnString = "greater than or equals";
	}
	else if ( strcmp(input, "&&") == 0) {
		returnString = "logical and";
	}	// 25 operators
	else if ( strcmp(input, "||") == 0) {
		returnString = "logical or";
	}
	else if ( strcmp(input, ">>") == 0) {
		returnString = "shift right";
	}
	else if ( strcmp(input, "<<") == 0) {
		returnString = "shift left";
	}
	else if ( strcmp(input, "+-") == 0) {
		returnString = "plus minus";
	}
	else if ( strcmp(input, "+=") == 0) {
		returnString = "increment assignment";
	}	// 30 operators
	else if ( strcmp(input, "-=") == 0) {
		returnString = "decrement assignment";
	}
	else if ( strcmp(input, "*=") == 0) {
		returnString = "multiplication assignment";
	}
	else if ( strcmp(input, "/=") == 0) {
		returnString = "division assignment";
	}
	else if ( strcmp(input, "%=") == 0) {
		returnString = "modulo assignment";
	}
	else if ( strcmp(input, "&=") == 0) {
		returnString = "bitwise and assignment";
	}	// 35 operators
	else if ( strcmp(input, "^=") == 0) {
		returnString = "bitwise xor assignment";
	}
	else if ( strcmp(input, "|=") == 0) {
		returnString = "bitwise or assignment";
	}
	else if ( strcmp(input, "->") == 0) {
		returnString = "structure pointer";
	}
	else if ( strcmp(input, ">>=") == 0) {
		returnString = "bitwise right shift assignment";
	}
	else if ( strcmp(input, "<<=") == 0) {
		returnString = "bitwise left shift assignment";
	}	// 40 operators
	return returnString;
}

/*
 * Returns 1 if the char is an escape character, 0 otherwise.
 */

int isEscapeCharacter(char input)
{
	if (	   input == '\a' || input == '\b' || input == '\t'
			|| input == '\n' || input == '\v' || input == '\f'
			|| input == '\r' || input == '\"' || input == '\''
			|| input == '\?' || input == '\\') {
		return 1;
	}
	return 0;
}

/*
 * Return 1 if the char sequence is an escape character, 0 otherwise.
 */
/*
int isEscapeCharacterSequence(char* input)
{
	if (	   strcmp(input, "\a") || strcmp(input, "\b") || strcmp(input, "\t")
			|| strcmp(input, "\n") || strcmp(input, "\v") || strcmp(input, "\f")
			|| strcmp(input, "\r") || strcmp(input, "\"") || strcmp(input, "\'")
			|| strcmp(input, "\?") || strcmp(input, "\\") ) {
		return 1;
	}
	return 0;
}
*/

/*
 * Returns 1 if char sequence is a C keyword, 0 otherwise.
 */
int isCKeyword(char* input)
{
	if (	   strcmp(input, "auto") == 0 || strcmp(input, "break") == 0 || strcmp(input, "case") == 0
			|| strcmp(input, "char") == 0 || strcmp(input, "const") == 0 || strcmp(input, "continue") == 0
			|| strcmp(input, "default") == 0 || strcmp(input, "do") == 0 || strcmp(input, "double") == 0
			|| strcmp(input, "else") == 0 || strcmp(input, "enum") == 0 || strcmp(input, "extern") == 0
			|| strcmp(input, "float") == 0 || strcmp(input, "for") == 0 || strcmp(input, "goto") == 0
			|| strcmp(input, "if") == 0 || strcmp(input, "int") == 0 || strcmp(input, "long") == 0
			|| strcmp(input, "register") == 0 || strcmp(input, "return") == 0 || strcmp(input, "short") == 0
			|| strcmp(input, "signed") == 0 || strcmp(input, "sizeof") == 0 || strcmp(input, "static") == 0
			|| strcmp(input, "struct") == 0 || strcmp(input, "switch") == 0 || strcmp(input, "typedef") == 0
			|| strcmp(input, "union") == 0 || strcmp(input, "unsigned") == 0 || strcmp(input, "void") == 0
			|| strcmp(input, "volatile") == 0 || strcmp(input, "while") == 0) {
		return 1;
		}
	return 0;
}

/*
 * Tokenizer state for initializing the tokenizer.
 * If malloc cannot allocate memory for the structure, it will return EXIT_STATE to end the program.
 */
enum state_ID_t entryState0(char* inputStream)
{
	tokenizer = TKCreate(inputStream);
	if (tokenizer == NULL) {
		fprintf(stderr, "malloc returned null for TKCreate.\n");
		return EXIT_STATE;
	}
	printf("\"Input argument is %s length %zu.\"\n", inputStream, strlen(inputStream));

	return SCANNING_STATE;
}

/*
 * Tokenizer state for parsing a character and determining which token it correlates to.
 * No token is currently being processed in this state; the token size is 0.
 * A whitespace character is ignored, the tokenizer increments its index, and SCANNING_STATE is returned.
 * An escape character is printed in brackets and continues.
 * A C operator symbol returns OPERATOR_STATE.
 * An alphabetic character returns ALPHANUM_STATE.
 * A digit character is further analyzed to differentiate between decimal, octal, and hexadecimal numbers.
 *   If the digit is a 0, a x or X character immediately after returns HEXADECIMAL_STATE.
 *   Otherwise the function returns OCTAL_STATE.
 *   If the digit is not a 0, DECIMAL_STATE is returned.
 * If the End of File character is reached, EXIT_STATE is returned to begin ending the program.
 *
 */
enum state_ID_t nullTokenScanningState1()
{
	char input = tokenizer->tokenStream[tokenizer->index];

	// Determine what to do with input
	if (isspace(input)) {
		tokenizer->index++;
		return SCANNING_STATE;
	}

	if (isEscapeCharacter(input)) {
		printf("[%x]\n", input);
		tokenizer->index++;
		return SCANNING_STATE;
	}

	if (isOperator(input)) {
		return OPERATOR_STATE;
	}

	if (isalpha(input)) {
		return ALPHANUM_STATE;
	}

	if (isdigit(input)) {
		if (input == '0') {	// either octal or hexadecimal
			char nextInput = tokenizer->tokenStream[tokenizer->index+1];

			if (nextInput == 'x' || nextInput == 'X') {	// hexadecimal integer constant
				nextInput = tokenizer->tokenStream[tokenizer->index+2];	// look at the char after the X
				tokenizer->index++, tokenizer->tokenSize++;	// add extra increment to pass over x/X character
				if (isxdigit(nextInput)) {	// proper hexadecimal structure
					return HEXADECIMAL_STATE;
				}
				else if (tokenizer->index + 1 >= tokenizer->tokenStreamSize) {	// reached end of stream
					tokenizer->index++, tokenizer->tokenSize++;	// add extra increments to encompass whole malformed token
					return MALFORMED_TOKEN_STATE;
				}
				else if ( isspace( tokenizer->tokenStream[tokenizer->index + 1]) ) {	// white space after token
					tokenizer->index++, tokenizer->tokenSize++;	// add extra increments to encompass whole malformed token
					return MALFORMED_TOKEN_STATE;
				}
				else {	// since there is no hex digit after the X, this hex value is malformed
					tokenizer->index+=2, tokenizer->tokenSize+=2;	// add extra increments to encompass whole malformed token
					return MALFORMED_TOKEN_STATE;
				}
			}	// end of if (nextInput == 'x' || nextInput == 'X')
			else {	// octal integer constant
				if (isOctal(nextInput)) {	// proper octal structure
					return OCTAL_STATE;
				}
				else {	// defaulted to decimal constant "0"
					return DECIMAL_STATE;
				}
			}
		}	// end of if (input == '0')
		else {	// decimal integer constant
			return DECIMAL_STATE;
		}
	}	// end of isdigit(input))

	// this will end the function
	if (tokenizer->index >= tokenizer->tokenStreamSize) {
		return EXIT_STATE;
	}

	tokenizer->index++;
	return SCANNING_STATE;
}

/*
 * Tokenizer state for parsing a word token.
 * It may return ALPHANUM_STATE in the case of a larger word token possibility.
 * Otherwise it will return ALPHANUM_TOKEN_STATE to print the finalized token.
 */
enum state_ID_t alphanumState2()
{
	tokenizer->index++, tokenizer->tokenSize++;
	char input = tokenizer->tokenStream[tokenizer->index];
	if (isalpha(input) || isdigit(input)) {
		return ALPHANUM_STATE;
	}
	else {	// end of word token
		return ALPHANUM_TOKEN_STATE;
	}
}

/*
 * Tokenizer state for printing a finalized word token.
 * It will always return a SCANNING_STATE value to scan a new token.
 */
enum state_ID_t alphanumTokenState3()
{
	char* alphanumToken = TKGetNextToken(tokenizer);
	if ( isCKeyword(alphanumToken) ) {
		printf("C keyword \"%s\"\n", alphanumToken);
	}
	else {
		printf("word \"%s\"\n", alphanumToken);
	}
	free(alphanumToken);	// free allocated memory to token

	tokenizer->tokenSize = 0;	// reset token size counter for new token

	return SCANNING_STATE;
}

/*
 * Tokenizer state for parsing a decimal constant token.
 * It may return DECIMAL_STATE in the case of a larger decimal constant token possibility.
 * If the tokenizer encounters a floating-point symbol, it will return FLOATING_POINT_STATE to handle the identified floating-point constant.
 * Otherwise it will return DECIMAL_TOKEN_STATE to print the finalized token.
 */
enum state_ID_t decimalState5()
{
	tokenizer->index++, tokenizer->tokenSize++;
	char input = tokenizer->tokenStream[tokenizer->index];

	// NOTE: the presence of a decimal point indicates a floating-point number, and it should change accordingly
	if (isdigit(input)) {
		return DECIMAL_STATE;
	}
	else if (input == '.') {	// presence of floating-point number, change states
		tokenizer->index++, tokenizer->tokenSize++;	// extra increment to pass over floating-point
		return FLOATING_POINT_STATE;
	}
	else {	// end of decimal token
		return DECIMAL_TOKEN_STATE;
	}
}

/*
 * Tokenizer state for printing a finalized decimal constant token.
 * It will always return a SCANNING_STATE value to scan a new token.
 */
enum state_ID_t decimalTokenState6()
{
	char* decimalToken = TKGetNextToken(tokenizer);
	printf("decimal constant \"%s\"\n", decimalToken);

	free(decimalToken);	// free allocated memory to token

	tokenizer->tokenSize = 0;	// reset token size counter for new token

	return SCANNING_STATE;
}

/*
 * Tokenizer state for parsing an octal constant token.
 * It may return OCTAL_STATE in the case of a larger octal constant token possibility.
 * Otherwise it will return OCTAL_TOKEN_STATE to print the finalized token.
 */
enum state_ID_t octalState7()
{
	tokenizer->index++, tokenizer->tokenSize++;
	char input = tokenizer->tokenStream[tokenizer->index];
	if (isOctal(input)) {
		return OCTAL_STATE;
	}
	else {	// end of octal token
		return OCTAL_TOKEN_STATE;
	}
}

/*
 * Tokenizer state for printing a finalized octal constant token.
 * It will always return a SCANNING_STATE value to scan a new token.
 */
enum state_ID_t octalTokenState8()
{
	char* octalToken = TKGetNextToken(tokenizer);
	printf("octal constant \"%s\"\n", octalToken);

	free(octalToken);	// free alocated memory to token

	tokenizer->tokenSize = 0;	// reset token size counter for new token

	return SCANNING_STATE;
}

/*
 * Tokenizer state for parsing a hexadecimal constant token.
 * It may return HEXADECIMAL_STATE in the case of a larger hexadecimal constant token possibility.
 * Otherwise it will return HEXADECIMAL_TOKEN_STATE to print the finalized token.
 */
enum state_ID_t hexadecimalState9()
{
	tokenizer->index++, tokenizer->tokenSize++;
	char input = tokenizer->tokenStream[tokenizer->index];
	if (isxdigit(input)) {
		return HEXADECIMAL_STATE;
	}
	else {	// end of hexadecimal token
		return HEXADECIMAL_TOKEN_STATE;
	}
}

/*
 * Tokenizer state for printing a finalized hexadecimal constant token.
 * It will always return a SCANNING_STATE value to scan a new token.
 */
enum state_ID_t hexadecimalTokenState10()
{
	char* hexadecimalToken = TKGetNextToken(tokenizer);
	printf("hex constant \"%s\"\n", hexadecimalToken);

	free(hexadecimalToken);	// free alocated memory to token

	tokenizer->tokenSize = 0;	// reset token size counter for new token

	return SCANNING_STATE;
}

/*
 * Tokenizer state for parsing the mantiassa of a floating-point constant token.
 * It may return FLOATING_POINT_STATE in the case of a larger floating-point constant token possibility.
 * If it detects a 'e', it will return FLOATING_POINT_EXPONENT_STATE to handle the exponent.
 * Otherwise it will return FLOATING_POINT_TOKEN_STATE to print the finalized token.
 */
enum state_ID_t floatingPointState11()
{
	tokenizer->index++, tokenizer->tokenSize++;
	char input = tokenizer->tokenStream[tokenizer->index];

	// handle various input here
	if (isdigit(input)) {
		return FLOATING_POINT_STATE;
	}
	else if (input == 'e' || input == 'E') {	// presence of exponent
		tokenizer->index++, tokenizer->tokenSize++;	// pass over e/E
		// must check for malformed input here
		input = tokenizer->tokenStream[tokenizer->index];	// inspect next char after e/E
		if (input == '+' || input == '-') {	// all valid input
			if (isdigit(tokenizer->tokenStream[tokenizer->index+1])) {	// check for a number after a + or -
				return FLOATING_POINT_EXPONENT_STATE;
			}
			else {	// invalid input, malformed token
				tokenizer->index++, tokenizer->tokenSize++;	// include offending char
				return MALFORMED_TOKEN_STATE;
			}
		}
		else if (isdigit(input)) {	// positive exponent
			return FLOATING_POINT_EXPONENT_STATE;
		}
		else {	// invalid input after e, malformed token
			return MALFORMED_TOKEN_STATE;
		}
	}
	else {	// end of floating-point token
		return FLOATING_POINT_TOKEN_STATE;
	}
}

/*
 * Tokenizer state for pasing the exponent of a floating-point constant token.
 * It may return FLOATING_POINT_EXPONENT_STATE in the case of a larger floating-point token.
 * Otherwise it will return FLOATING_POINT_TOKEN_STATE to print the finalized token.
 */

enum state_ID_t floatingPointExponentState()
{
	tokenizer->index++, tokenizer->tokenSize++;
	char input = tokenizer->tokenStream[tokenizer->index];
	if (isdigit(input)) {
		return FLOATING_POINT_EXPONENT_STATE;
	}
	else {	// end of floating-point token
		return FLOATING_POINT_TOKEN_STATE;
	}
}

/*
 * Tokenizer state for printing a finalized floating-point constant token.
 * It will always return a SCANNING_STATE value to scan a new token.
 */
enum state_ID_t floatingPointTokenState12()
{
	char* floatingPointToken = TKGetNextToken(tokenizer);
	printf("float \"%s\"\n", floatingPointToken);

	free(floatingPointToken);	// free allocated memory to token

	tokenizer->tokenSize = 0;	// reset token size counter for new token

	return SCANNING_STATE;
}

/*
 * Tokenizer state for parsing a C operator token.
 * It may return OPERATOR_STATE in the case of a larger C operator token possibility.
 * Otherwise it will return OPERATOR_TOKEN_STATE to print the finalized token.
 */
enum state_ID_t operatorState13()
{
	tokenizer->index++, tokenizer->tokenSize++;
	char input = tokenizer->tokenStream[tokenizer->index];

	/*
	 * Due to the possibility of variable size operators, I have to look at the next
	 * character in the sequence to see if it's part of a larger operator or another
	 * individual operator.
	 */
	// if it's an operator, try to see if this char appended to the end of the current token makes a valid operator
	if ( isOperator(input) ) {
		tokenizer->index++, tokenizer->tokenSize++;
		char* tempOperatorToken = TKGetNextToken(tokenizer);
		tokenizer->index--, tokenizer->tokenSize--;	// undo the extra increment to prepare for a repeat call
		// if this token is still valid, then we keep going in the off-chance there's an even bigger valid token
		if ( isCOperator(tempOperatorToken) ) {
			free(tempOperatorToken);	// no longer needed, also may not be final token
			return OPERATOR_STATE;
		}
		else {	// is no longer a valid token, so we had a final token with the original string
			free(tempOperatorToken);
			return OPERATOR_TOKEN_STATE;
		}
	}	// end of isOperator(nextChar)
	else {	// next char isn't an operator, so now we have a definite valid token
		return OPERATOR_TOKEN_STATE;
	}
}

/*
 * Tokenizer state for printing a finalized C operator token.
 * It will always return a SCANNING_STATE value to scan a new token.
 */
enum state_ID_t operatorTokenState14()
{
	char* operatorToken = TKGetNextToken(tokenizer);
	char* operatorName = getOperator(operatorToken);	// this method shouldn't ever fail at this point, otherwise there is a discrepanc
	printf("%s \"%s\"\n", operatorName, operatorToken);
	free(operatorToken);

	tokenizer->tokenSize = 0;

	return SCANNING_STATE;
}

/*
 * Tokenizer state for printing a malformed token.
 * After printing the formatted malformed token, it will return SCANNING_STATE to scan a new token.
 */
enum state_ID_t malformedTokenState4()
{
	char* malformedToken = TKGetNextToken(tokenizer);
	printf("malformed token \"[%s]\"\n", malformedToken);
	free(malformedToken);

	tokenizer->tokenSize = 0;

	return SCANNING_STATE;
}


/*
 * main will have a string argument (in argv[1]).
 * The string argument contains the tokens.
 * Print out the tokens in the second string in left-to-right order.
 * Each token should be printed on a separate line.
 */

int main(int argc, char **argv)
{
	// Erroneous input handling
	if (argc != 2) {
		fprintf(stderr, "Please input one string argument.\n");
		return -1;
	}

	int endOfFunction = 0;
	enum state_ID_t state = ENTRY_STATE;

	/*
	 * Tokenizer states are handled in a switch-case format.
	 * The state of the tokenizer determines which function to call, and each function will
	 * return a value depending on the output of the character sequence. When the tokenizer
	 * reaches the end of the input stream, it will return EXIT_STATE which frees any remaning
	 * dynamically allocated memory to tokenizer and exits.
	 */
	do
	{
		switch(state) {
		case ENTRY_STATE:	state = entryState0(argv[1]); break;
		case SCANNING_STATE: state = nullTokenScanningState1(); break;
		case ALPHANUM_STATE: state = alphanumState2(); break;
		case ALPHANUM_TOKEN_STATE: state = alphanumTokenState3(); break;
		case DECIMAL_STATE: state = decimalState5(); break;
		case DECIMAL_TOKEN_STATE: state = decimalTokenState6(); break;
		case OCTAL_STATE: state = octalState7(); break;
		case OCTAL_TOKEN_STATE: state = octalTokenState8(); break;
		case HEXADECIMAL_STATE: state = hexadecimalState9(); break;
		case HEXADECIMAL_TOKEN_STATE: state = hexadecimalTokenState10(); break;
		case FLOATING_POINT_STATE: state = floatingPointState11(); break;
		case FLOATING_POINT_EXPONENT_STATE: state = floatingPointExponentState(); break;
		case FLOATING_POINT_TOKEN_STATE: state = floatingPointTokenState12(); break;
		case OPERATOR_STATE: state = operatorState13(); break;
		case OPERATOR_TOKEN_STATE: state = operatorTokenState14(); break;
		case MALFORMED_TOKEN_STATE: state = malformedTokenState4(); break;
		case EXIT_STATE: endOfFunction = 1; break;
		}
	}
	while(endOfFunction != 1);

	return 0;
}
