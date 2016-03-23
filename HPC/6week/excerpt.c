#include <stdio.h>
#include <stdlib.h>

int
main(int argc, char**argv)
{
    int i;
    long *bufr;
    long value;
    FILE * fp;

    bufr=calloc(1200, sizeof(long));
    fp = fopen("1200.nums", "r");
    for(i=0; i<1200; i++) {
	if (fscanf(fp, "%ld\n", &value) != 1) {
	    fprintf(stderr, "error on input\n");
	}
	bufr[i] = value;
    }
    fclose(fp);

    for(i=0; i<100; i++) {
	printf("%ld\n", bufr[i]);
    }
}
