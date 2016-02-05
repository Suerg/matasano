#ifndef DATAMANIP_H
#define DATAMANIP_H

void xortwo(unsigned char *xored, unsigned char *b1,
		unsigned char *b2, int len);
unsigned char xorbyte(unsigned char byte, unsigned char xorwith);
void xorbytes(unsigned char *xored, unsigned char *bytes,
		int len, unsigned char xorwith);

#endif
