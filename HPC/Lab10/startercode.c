/*
 * heat transfer simulation using Jacobi iteration
 */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <omp.h>
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
    printf("here1\n");
    //initialize parallel threads
    //pragma omp parallel
    //{
        // allocate memory for the lattice to hold the current values
        A = (double **) calloc(n, sizeof (double *));
        //pragma omp for
        for (j=0; j < n; j++) {
        A[j] = (double *) calloc(m, sizeof(double));
        }
        printf("here2\n");
        // allocate memory for the lattice to hold the new values
        Anew = (double **) calloc(n, sizeof (double *));
        //pragma omp for
        for (j=0; j < n; j++) {
        Anew[j] = (double *) calloc(m, sizeof(double));
        }
        printf("here3\n");
        /*
        * initialize it to something
        */
        //pragma omp for private(i)
        for(j=1; j < n-1; j++) {
            for(i=1; i< m-1; i++) {
            //printf("j:%d i:%d\n",j,i);
            A[j][i] = 20;
            }
        }
        printf("here4\n");
       // pragma omp for
        for(i=0;i<m;i++){
            A[0][i] = 20;
            A[m-1][i] = 20;
            Anew[0][i] = 20;
            Anew[m-1][i] = 20;
        }
        printf("here5\n");
        //pragma omp for
        for(i=1;i<n-1;i++){
            A[i][0] = 90;
            A[i][n-1] = 90;
            Anew[i][0] = 90;
            Anew[i][n-1] = 90;
        }
        //pragma omp for private(j,i)
        for(i=0;i<10;i++){
            for(j=0;j<10;j++){
                A[n/3+i][m/3+j] = 10;
                A[2*n/3+i][m/3+j] = 10;
                A[n/3+i][2*m/3+j] = 10;
                A[2*n/3+i][2*m/3+j] = 10;
            }
        }
    //}
    /*
     * jacobi iteration - until we're within tolerance
     */
    int localError= err;

    #pragma omp parallel firstprivate(localError,i,j)
    {    
        while ( err > tol && iter < iter_max) {
        #pragma omp barrier   
   
        err = 0;
        localError = 0;    
        #pragma omp for
        for (j=1; j < n-1; j++) {
            for(i=1; i< m-1; i++) {

            Anew[j][i] = 0.25 * (A[j][i+1] + A[j][i-1] +
                        A[j-1][i] + A[j+1][i]);
            
            err = max(localError, fabs(Anew[j][i] - A[j][i]));
            } // next i
        } // next j 
        #pragma omp critical
        {
            if ( err < localError ){
                err = localError;
            }
        }
        //pragma omp barrier
        //printf("here %d difference: %f\n", iter, err);
        //pragma omp barrier

        /* copy new map to old */
        for(j=1; j < n-1; j++) {
            for(i=1; i< m-1; i++) {
            A[j][i] = Anew[j][i];
            }
        }

        //copy the cooling blocks in to prevent them from changing
        for(i=0;i<10;i++){
            for(j=0;j<10;j++){
                A[n/3+i][m/3+j] = 10;
                A[2*n/3+i][m/3+j] = 10;
                A[n/3+i][2*m/3+j] = 10;
                A[2*n/3+i][2*m/3+j] = 10;
            }
        }
        iter++;
 
        // DEBUG
        // if (iter%100==0) {printf("%d %f\n", iter, err);
        //                   printMap(Anew, n, m);}
        
        } // endwhile
    }
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
    //printMap(Anew, n, m);
    
} // main
