#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// "只有字母和数字[0-9a-zA-Z]、一些特殊符号"$-_.+!*'(),"[不包括双引号]、以及某些保留字，才可以不经过编码直接用于URL。"
static int
isURIChar(const char ch) {
	if (isalnum(ch))
		return 1;
	if (ch == ' ')
		return 0;
	switch (ch) {
		case '$':
		case '-':
		case '_':
		case '.':
		case '+':
		case '!':
		case '*':
		case '\'':
		case '(':
		case ')':
			return 1;
		default:
			return 0;
	}
}

char *
URIEncode(const char *uri) {
	int n = strlen(uri);
	char *cp = (char *)malloc(n * 3);
	memset(cp, 0, n * 3);
	int pn = 0;
	// scan uri
	for (int i = 0; i < n; i ++ ) {
		if (isURIChar(uri[i])) {
			cp[pn++] = uri[i];
			continue;
		}
		// UTF8
		char it[4] = { 0 };
		if ((unsigned char)uri[i] < 0x10)
			sprintf(it, "%%0%X", (unsigned char)uri[i]);
		else
			sprintf(it, "%%%X", (unsigned char)uri[i]);
		strcat(cp, it);
		pn += 3;
	}
	return cp;
}

int
main(int argc, char *argv[]) {
#define	BUFLE	1024L
	long long cur = 0;
	long long length = BUFLE * 2;
	char *buf = (void *)malloc(length);
	while (1) {
		long long le = read(0, &buf[cur], length - cur - 1);
		cur += le;
		if (le < 1) {
			break;
		}
		if (length - cur < BUFLE) {
			length *= 2;
			buf = realloc(buf, length);
		}
	}
	buf[cur] = '\0';
	printf("%s\n", URIEncode(buf));
	return 0;
}
