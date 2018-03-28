#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <mpi.h>

#define MASTER_ID 0
#define ARRAY_SIZE 40

const int tag_index = 1;


/*
	This program fills an array and divide the work into slaves.
	Each slave process a whole block and send this whole block to master.
*/
int main(int argc, char **argv){
    
    MPI_Status status;
    
    int ierr, size, pid, i, j;
    
    ierr = MPI_Init(&argc, &argv);
    ierr = MPI_Comm_rank(MPI_COMM_WORLD, &pid);
    ierr = MPI_Comm_size(MPI_COMM_WORLD, &size);

    int chunksize = ARRAY_SIZE / size; //size per processor
        
    //------------ MASTER STARTS HERE ------------
    if(pid == MASTER_ID) {    
        float arr[ARRAY_SIZE];
        
        //master fills the first parts [0, chunksize - 1]
        for(j = 0; j < chunksize; j++){
            arr[j] = sqrt(j * 1.0);
        }

        for(i = 1; i < size; i++){
            int pos = chunksize * i;
            ierr = MPI_Recv(&arr[pos], chunksize, MPI_FLOAT, i, tag_index, MPI_COMM_WORLD, &status);      
        }
		
        printf("\nMaster and slaves have finished the work, result:\n");
        for(i = 0; i < ARRAY_SIZE; i++){
            printf("%.1f ", arr[i]);
            if((1+i) % chunksize == 0) printf("\n");
        }
        printf("\n");
    }
    //------------ MASTER ENDS HERE ------------
    
    //------------ SLAVES START HERE ------------
    else{                    
        float arr[chunksize];
        
        for(j = 0; j < chunksize; j++){
            int real_pos = pid * chunksize + j;
            arr[j] = sqrt((pid * chunksize + j) * 1.0);
        }
        
        ierr = MPI_Send(&arr[0], chunksize, MPI_FLOAT, MASTER_ID, tag_index, MPI_COMM_WORLD);
    }
    //------------ SLAVES END HERE ------------
    ierr = MPI_Finalize();
    return 0;
}