#include <mpi.h>
#include <stdbool.h>
#include "message.h"
#include "manager.h"

int find_process_with_max_available_tasks_count(const size_t* tasks_count, int workers_size) {
    size_t max_available_tasks_count = tasks_count[0];
    int ret = 0;

    for (int i = 1; i < workers_size; i++) {
        if (max_available_tasks_count < tasks_count[i]) {
            max_available_tasks_count = tasks_count[i];
            ret = i;
        }
    }

    return ret;
}

void manage(int workers_size) {
    size_t tasks_count[workers_size];

    MPI_Datatype message_data_type = create_message_data_type();

    while (true) {
        message_t in;
        MPI_Status status;

        MPI_Recv(&in,
                 1,
                 message_data_type,
                 MPI_ANY_SOURCE,
                 MPI_ANY_TAG,
                 MPI_COMM_WORLD,
                 &status);

        const int source = status.MPI_SOURCE;

        if (in.type == TASKS_UPDATE_TYPE) {
            tasks_count[source] = (size_t) in.value;
        }
        if (in.type == PROCESS_RANK_REQUEST_TYPE) {
            const int response = find_process_with_max_available_tasks_count(tasks_count, workers_size);
            MPI_Send(&response, 1, MPI_INT, source, DEFAULT_MANAGER_RESPONSE_TAG, MPI_COMM_WORLD);
        }
        if (in.type == TERMINATION_TYPE) {
            break;
        }
    }

    MPI_Type_free(&message_data_type);
}
