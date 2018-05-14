#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <mpi.h>

#define MASTER_ID 0

const int id_tag = 1;
const int name_tag = 2;
const int name_len_tag = 3;

/*
	This program works like a ring, each processor sends name and id to the following processor.
	The last processors sends it back to the MASTER.
*/
int main(){
    MPI_Status status;

    int ierr, size, pid;

    ierr = MPI_Init(NULL, NULL);
    ierr = MPI_Comm_rank(MPI_COMM_WORLD, &pid);
    ierr = MPI_Comm_size(MPI_COMM_WORLD, &size);

	if(size == 1){
		printf("Ok, just one processor.\n");
	}else{
		if(pid == MASTER_ID){

            char name[MPI_MAX_PROCESSOR_NAME];
            int len;

            MPI_Get_processor_name(name, &len);
			ierr = MPI_Send(&pid, 1, MPI_INT, (pid + 1) % size, id_tag, MPI_COMM_WORLD);
            ierr = MPI_Send(&len, 1, MPI_INT, (pid + 1) % size, name_len_tag, MPI_COMM_WORLD);
            ierr = MPI_Send(&name, len, MPI_CHAR, (pid + 1) % size, name_tag, MPI_COMM_WORLD);

			int final_id, final_len;
            char final_name[MPI_MAX_PROCESSOR_NAME];
			ierr = MPI_Recv(&final_id, 1, MPI_INT, size - 1, id_tag, MPI_COMM_WORLD, &status);
            ierr = MPI_Recv(&final_len, 1, MPI_INT, size - 1, name_len_tag, MPI_COMM_WORLD, &status);
            ierr = MPI_Recv(&final_name[0], final_len, MPI_CHAR, size - 1, name_tag, MPI_COMM_WORLD, &status);

            printf("Currend id = %d, received from %d the name = %s\n", pid, size - 1, final_name);
		}else{

			int before_id, before_len;
            char before_name[MPI_MAX_PROCESSOR_NAME];

            ierr = MPI_Recv(&before_id, 1, MPI_INT, pid - 1, id_tag, MPI_COMM_WORLD, &status);
            ierr = MPI_Recv(&before_len, 1, MPI_INT, pid - 1, name_len_tag, MPI_COMM_WORLD, &status);
            ierr = MPI_Recv(&before_name[0], before_len, MPI_CHAR, pid - 1, name_tag, MPI_COMM_WORLD, &status);

            char name[MPI_MAX_PROCESSOR_NAME];
            int len;
            MPI_Get_processor_name(name, &len);
            ierr = MPI_Send(&pid, 1, MPI_INT, (pid + 1) % size, id_tag, MPI_COMM_WORLD);
            ierr = MPI_Send(&len, 1, MPI_INT, (pid + 1) % size, name_len_tag, MPI_COMM_WORLD);
            ierr = MPI_Send(&name, len, MPI_CHAR, (pid + 1) % size, name_tag, MPI_COMM_WORLD);

            printf("Currend id = %d, received from %d the name = %s\n", pid, before_id, before_name);
		}
	}
    MPI_Finalize();
	return 0;
}
