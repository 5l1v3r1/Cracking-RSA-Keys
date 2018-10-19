/*The code is written based on sequential RSA cracking code https://github.com/mitchellwrosen/rsa-crack-cuda/tree/master/sequential
*/

#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <mpi.h>

#include "rsa.h"
#include "mygmp.h"

int size, rank;

int main(int argc, char** argv) {
    // When invoking the program, you need to type in the filename and keyNum
    if (argc < 3){
        printf("The user needs to input the filename and number of keys in this file\n");
        return 0;
    }

     //Initialize communicator
	MPI_Init( &argc, &argv );
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    mpz_t q1, q2, d1, d2;
    mpz_inits(q1, q2, d1, d2, NULL);

    int keyNum = atoi(argv[2]);
    mpz_t* keys = mpz_reads(argv[1], keyNum);

    FILE *stream = argc == 4 ? fopen(argv[3], "w") : stdout;

    int i;
    //This code block calculates the first half of key pairs as demonstrated in the report.
    #pragma omp parallel for schedule(dynamic) private(i)
    for (i = rank; i < keyNum-1; i+=size*2) {
        int j;
        for (j = i+1; j < keyNum; j++) {
        //Initialze gcd within each thread.
         mpz_t gcd;
         mpz_init(gcd);
         mpz_gcd(gcd, keys[i], keys[j]);
         //If gcd is larger than 1, the following block will calculate and print out the cracked keys
         //Since q1, d1, q2, d2 are defined globally, we need to sequentially implement the following code.
         if (mpz_cmp_ui(gcd, 1) != 0) {
                #pragma omp critical
         {

                mpz_divexact(q1, keys[i], gcd);
                rsa_compute_d(d1, keys[i], gcd, q1);
                mpz_out_str(stream, 10, keys[i]);
                fputc(':', stream);
                mpz_out_str(stream, 10, d1);
                fputc('\n', stream);

                mpz_divexact(q2, keys[j], gcd);
                rsa_compute_d(d2, keys[j], gcd, q2);
                mpz_out_str(stream, 10, keys[j]);
                fputc(':', stream);
                mpz_out_str(stream, 10, d2);
                fputc('\n', stream);

            }

         }

         }
      }

    // Once again, this code calculates the second half of key pairs.
    #pragma omp parallel for schedule(dynamic) private(i)
    for (i = 2*size-rank-1; i < keyNum-1; i+=size*2) {
        int j;
        for (j = i+1; j < keyNum; j++) {
         mpz_t gcd;
         mpz_init(gcd);

          mpz_gcd(gcd, keys[i], keys[j]);
          if (mpz_cmp_ui(gcd, 1) != 0) {
                #pragma omp critical
         {

                mpz_divexact(q1, keys[i], gcd);
                rsa_compute_d(d1, keys[i], gcd, q1);
                mpz_out_str(stream, 10, keys[i]);
                fputc(':', stream);
                mpz_out_str(stream, 10, d1);
                fputc('\n', stream);

                mpz_divexact(q2, keys[j], gcd);
                rsa_compute_d(d2, keys[j], gcd, q2);
                mpz_out_str(stream, 10, keys[j]);
                fputc(':', stream);
                mpz_out_str(stream, 10, d2);
                fputc('\n', stream);

            }

         }

         }
      }

    free(keys);

    if (argc == 4)
        fclose(stream);

    MPI_Finalize();
    return EXIT_SUCCESS;
}
