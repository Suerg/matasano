#ifndef SCORE_H
#define SCORE_H

#define SEPARATORS (" ,.-:;")

void score_bytes(double *scores, unsigned char *bytes, int len);
double calc_bytescore(unsigned char *bytes, int len);

#endif
