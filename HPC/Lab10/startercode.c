/*
 * heat transfer simulation using Jacobi iteration
 */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define max(a,b) ((a) >= (b) ? (a) : (b))

/*
 * printMap - spit out the values as CSV
 *
 * prints m elements per row, n-many rows
 */
void
printMap(double **A, int n, int m)
{
    int i,j;

    for(j=0; j < n; j++) {
	for(i=0; i< m; i++) {
	    printf("%8.5f, ", A[j][i]);
	}
	printf("\n");
    }
}

/*
 * heat transfer simulation using Jacobi iteration
 *
 * memory is allocated for two lattices, to keep new values separate from current values
 *
 * example usage:  a.out 500 800
 */
int
main(int argc, char **argv)
{
    int i, j;
    int n, m;
    double localerr;
    double **A, **Anew;
    double tol = 0.001;
    double err = 2*tol;
    int iter=0;
    int iter_max=25000;

    /* size of the area nxm; default 2000x2000 */
    if (argc < 3) {
	n = m = 2000;
    } else {
	n=atoi(argv[1]);
	m=atoi(argv[2]);
    }

    // allocate memory for the lattice to hold the current values
    A = (double **) calloc(n, sizeof (double *));
    for (j=0; j < n; j++) {
	A[j] = (double *) calloc(m, sizeof(double));
    }

    // allocate memory for the lattice to hold the new values
    Anew = (double **) calloc(n, sizeof (double *));
    for (j=0; j < n; j++) {
	Anew[j] = (double *) calloc(m, sizeof(double));
    }

    /*
     * initialize it to something
     */
    for(j=1; j < n-1; j++) {
	for(i=1; i< m-1; i++) {
	    A[j][i] = 20;
	}
    }

    /*
     * jacobi iteration - until we're within tolerance
     */
    while ( err > tol && iter < iter_max) {
	err = 0.0;

	for (j=1; j < n-1; j++) {
	    for(i=1; i< m-1; i++) {

		Anew[j][i] = 0.25 * (A[j][i+1] + A[j][i-1] +
				     A[j-1][i] + A[j+1][i]);
	      
		err = max(localerr, fabs(Anew[j][i] - A[j][i]));

	    } // next i
	} // next j


	/* copy new map to old */
	for(j=1; j < n-1; j++) {
	    for(i=1; i< m-1; i++) {
		A[j][i] = Anew[j][i];
	    }
	}

      iter++;
      // DEBUG
      // if (iter%100==0) {printf("%d %f\n", iter, err);
      //                   printMap(Anew, n, m);}

    } // endwhile

    /*
     * Write this to stderr to avoid mixing in with the values;
     * it can serve as a "checksum" to see if a new version still
     * terminates at the same iteration count, implying the same
     * values were computed.
     */
    fprintf(stderr, "after %d iterations.\n", iter);
    /*
     * display final results
     */
    printMap(Anew, n, m);

} // main
