
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <sys/types.h>

#include <termios.h>

#include <unistd.h>

enum {
	BUFSIZE = 3,
	DEL_CHAR = 127,
	NEW_LINE = '\n',
};

char str[BUFSIZE];
int termflg;


unsigned int i = 0;


int blondy(char *str);
int get_char(char s[3]);
void term_set_options();

int
main(int argc, char *argv[])
{
	if (isatty(fileno(stdin))) {
		termflg = 1;
		term_set_options();
	}
	
	while (get_char(str) != EOF) {
		blondy(str);
		printf("%s", str);
	}

	return 0;
}

unsigned short
ext_tolower(unsigned short val)
{
	if (val == 53377)
		val = 53649;
	else if (val >= 53392 && val <= 53407)
		val += 32;
	else if (val >= 53407 && val <= 53423)
		val += 224;
		return val;
}

unsigned short
ext_toupper(unsigned short val)
{
	if (val == 53649)
		val = 53377;
	else if (val >= 53424 && val <= 53439)
		val -= 32;
	else if (val >= 53632 && val <= 53647)
		val -= 224;
	return val;
}

void
extended_blondy(char *str)
{
	unsigned short tmp;

	tmp = (unsigned char)*(str + 1) + (unsigned short)(*str << 8);

	if (i % 2 == 0)
		tmp = ext_toupper(tmp);
	else
		tmp = ext_tolower(tmp);

	*str = tmp >> 8;
	*(str + 1) = tmp & 0xFF;
}


int
blondy(char *str)
{
	if (*str == -48 || *str == -47) {
		extended_blondy(str);
		i++;
		return i;
	} else if (*str == ' ' || *str == '\t') {
		return i;
	} else if (*str == DEL_CHAR) {
		printf("\033[1D \033[1D");
		/* 
		 * We must care that blondy style must be saved
		 * after deletion.
		 */
		i--;
		return i;
	} else if (*str == NEW_LINE) {
		return i;
	}

	if (i % 2 == 0)
		*str = toupper(*str);
	else
		*str = tolower(*str);
	i++;

	return i;
}

int
get_char(char s[3])
{
	if ((s[0] = getchar()) != -48 && s[0] != -47) {
		s[1] = '\0';
		return s[0];
	} else {
		s[1] = getchar();
		s[2] = '\0';
		return s[1];
	}
}


void
term_set_options()
{
	int res;
	struct termios term;

	res = tcgetattr(fileno(stdin), &term);

	if (res != 0) {
		perror("GET_ATTR_ERR:");
		exit(1);
	}
	term.c_lflag = ((0xFFffFFff ^ (ICANON | ECHO)) & term.c_lflag);

        res = tcsetattr(fileno(stdin), TCSANOW, &term);
	if (res != 0) {
		perror("get_attr:");
		exit(1);
	}
}

