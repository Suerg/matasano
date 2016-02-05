void xortwo(unsigned char *xored, unsigned char *b1,
		unsigned char *b2, int len)
{
	int i = 0;

	for (i = 0; i < len; i++)
		xored[i] = b1[i] ^ b2[i];
}

unsigned char xorbyte(unsigned char byte, unsigned char xorwith)
{
	return byte ^ xorwith;
}

void xorbytes(unsigned char *xored, unsigned char *bytes,
		int len, unsigned char xorwith)
{
	int i = 0;

	for (i = 0; i < len; i++)
		xored[i] = xorbyte(bytes[i], xorwith);
}
