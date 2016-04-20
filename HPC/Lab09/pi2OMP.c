#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <omp.h>
/*
 * approximate pi by monte carlo methods
 */

long
throwdarts(long iters)
{
    long l;
    long insiders=0;
    double x, y;
    double h;		// hypotenuse
    long count = 0;
    int temp =0;

    #pragma omp parallel num_threads(8) reduction (+:temp)
    {
	    for(l=0; l < iters; l++) {
		x = ((double) rand()) / RAND_MAX;
		y = ((double) rand()) / RAND_MAX;
		// pythagorean theorem for hypotenuse
		// c^2 = a^2 + b^2
		// c=sqrt(a^2+b^2)
		h = sqrt(x*x+y*y);
		if (h > 1.0) { continue; }
		temp++;
	    }

	}

    return temp;
}

main(int argc, char ** argv)
{
    int count =0;
    int src;
    int tag;
    int dest;
    long ncount;
    long sumIn;
    long ans;
    double mypi;
    int i;
    ncount = atol(argv[1]); // get number of throws
    count =throwdarts(ncount);
    int myworld = omp_get_num_threads(); //if compiling with openmp
    //nt myworld = 1;

    mypi = ((double)count/(double)ncount) * 4.0 / (double) myworld;
	printf("my pi is: %20.18f\n", mypi);


} // main
