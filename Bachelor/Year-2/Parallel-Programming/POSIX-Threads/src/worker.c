#include <mpi.h>
#include <pthread.h>
#include <stdio.h>
#include <malloc.h>
#include <math.h>
#include <unistd.h>
#include "manager.h"
#include "worker.h"
#include "task.h"
#include "tasks_list.h"
#include "config.h"
#include "message.h"
#include "tasks_response.h"

#define OUTPUT_FILE_NAME "output.txt"
#define OUTPUT_BUFFER_SIZE 1024
#define DEFAULT_LISTENER_RESPONSE_TAG 1
#define DEFAULT_NOTIFY_TAG 2
#define DEFAULT_RANK_REQUEST_TAG 3
#define DEFAULT_TASK_REQUEST_TAG 4
#define SUCCESS_CODE 0

#define MAX(A, B) ((A) > (B) ? (A) : (B))
#define MIN(A, B) ((A) > (B) ? (B) : (A))

typedef struct {
    double min_imbalance_part;
    double max_imbalance_part;
    double average_imbalance_part;
} final_stats_t;

double seconds_to_millis(double seconds) {
    return seconds * 1000;
}

void print_iteration_stats(MPI_File out,
                           char* buffer,
                           size_t tasks_executed,
                           double global_result,
                           int iter_counter,
                           double current_iteration_time_ms,
                           int size,
                           int rank) {

    const int length1 = rank == 0
                        ? sprintf(buffer, "ITERATION %d\n\n", iter_counter)
                        : 0;

    const int length2 = sprintf(buffer + length1,
                                "Process %d\n%-40s%zu\n%-40s%.0lf ms\n%-40s%.2lf\n\n",
                                rank,
                                "Tasks executed",
                                tasks_executed,
                                "Time for iteration",
                                current_iteration_time_ms,
                                "Global result",
                                global_result);

    const int length3 = rank == size - 1
                        ? sprintf(buffer + length1 + length2, "-----------------------------------------------\n\n")
                        : 0;

    MPI_File_write_shared(out, buffer, length1 + length2 + length3, MPI_CHAR, MPI_STATUS_IGNORE);
}

final_stats_t calculate_final_stats(const double* total_iterations_time_ms, int size) {
    double min_imbalance_part = 1;
    double average_imbalance_part = 0;
    double max_imbalance_part = 0;

    for (int i = 0; i < LISTS_NUMBER; i++) {
        double max_time_ms = total_iterations_time_ms[i * size + size - 1];
        double imbalance_time_ms = 0;

        for (int j = 0; j < size - 1; j++) {
            const double t1 = total_iterations_time_ms[i * size + j];
            max_time_ms = MAX(max_time_ms, t1);

            for (int k = j + 1; k < size; k++) {
                const double t2 = total_iterations_time_ms[i * size + k];
                const double current_delta = fabs(t2 - t1);
                imbalance_time_ms = MAX(imbalance_time_ms, current_delta);
            }
        }

        const double current_imbalance_part = imbalance_time_ms / max_time_ms;
        min_imbalance_part = MIN(min_imbalance_part, current_imbalance_part);
        average_imbalance_part += current_imbalance_part;
        max_imbalance_part = MAX(max_imbalance_part, current_imbalance_part);
    }

    average_imbalance_part /= LISTS_NUMBER;

    final_stats_t ret = {
            .min_imbalance_part = min_imbalance_part,
            .max_imbalance_part = max_imbalance_part,
            .average_imbalance_part = average_imbalance_part
    };

    return ret;
}

pthread_mutex_t tasks_list_mutex;
pthread_mutex_t reached_critical_tasks_count_mutex;
pthread_mutex_t started_new_list_mutex;
pthread_cond_t reached_critical_tasks_count_cond;
pthread_cond_t started_new_list_cond;
tasks_list_t tasks_list;
MPI_Datatype task_data_type;
MPI_Datatype message_date_type;
MPI_Datatype tasks_response_data_type;
MPI_Comm workers_comm;
int manager_rank;
bool finished_all_tasks_flag = false;
bool reached_critical_tasks_count_flag = false;
bool started_new_list_flag = false;

void* requester_routine(__attribute__((unused)) void* arg) {
    int rank;
    MPI_Comm_rank(workers_comm, &rank);

    while (!finished_all_tasks_flag) {
        pthread_mutex_lock(&reached_critical_tasks_count_mutex);
        while (!reached_critical_tasks_count_flag) {
            pthread_cond_wait(&reached_critical_tasks_count_cond, &reached_critical_tasks_count_mutex);
        }
        pthread_mutex_unlock(&reached_critical_tasks_count_mutex);

        const message_t request = {
                .type = PROCESS_RANK_REQUEST_TYPE,
                .value = 0
        };

        MPI_Send(&request,
                 1,
                 message_date_type,
                 manager_rank,
                 DEFAULT_RANK_REQUEST_TAG,
                 MPI_COMM_WORLD);

        int rank_of_process_with_available_tasks;
        MPI_Recv(&rank_of_process_with_available_tasks,
                 1,
                 MPI_INT,
                 manager_rank,
                 DEFAULT_MANAGER_RESPONSE_TAG,
                 MPI_COMM_WORLD,
                 MPI_STATUS_IGNORE);

        if (rank_of_process_with_available_tasks == rank) {
            pthread_mutex_lock(&started_new_list_mutex);
            while (!started_new_list_flag) {
                pthread_cond_wait(&started_new_list_cond, &started_new_list_mutex);
            }
            pthread_mutex_unlock(&started_new_list_mutex);
            continue;
        }

        int fictive = 0;
        MPI_Send(&fictive,
                 1,
                 MPI_INT,
                 rank_of_process_with_available_tasks,
                 DEFAULT_TASK_REQUEST_TAG,
                 workers_comm);

        tasks_response_t tasks_response;
        MPI_Recv(&tasks_response,
                 1,
                 tasks_response_data_type,
                 rank_of_process_with_available_tasks,
                 DEFAULT_LISTENER_RESPONSE_TAG,
                 workers_comm,
                 MPI_STATUS_IGNORE);

        if (tasks_response.tasks_count) {
            pthread_mutex_lock(&tasks_list_mutex);
            for (long i = 0; i < tasks_response.tasks_count; i++) {
                add_task(&tasks_list, tasks_response.tasks[i]);
            }
            pthread_mutex_unlock(&tasks_list_mutex);
        }
        else {
            pthread_mutex_lock(&started_new_list_mutex);
            while (!started_new_list_flag) {
                pthread_cond_wait(&started_new_list_cond, &started_new_list_mutex);
            }
            pthread_mutex_unlock(&started_new_list_mutex);
        }
    }

    return SUCCESS_CODE;
}

void* listener_routine(__attribute__((unused)) void* arg) {
    int rank;
    MPI_Comm_rank(workers_comm, &rank);

    int received = 1;
    while (!finished_all_tasks_flag) {
        MPI_Request request;

        if (received) {
            int fictive;
            MPI_Irecv(&fictive,
                      1,
                      MPI_INT,
                      MPI_ANY_SOURCE,
                      DEFAULT_TASK_REQUEST_TAG,
                      workers_comm,
                      &request);
            received = 0;
        }

        MPI_Status status;
        MPI_Test(&request, &received, &status);
        if (received) {
            pthread_mutex_lock(&tasks_list_mutex);

            tasks_response_t tasks_response = {.tasks_count = 0};
            for (int i = 0; i < TASKS_RESPONSE_LENGTH; i++) {
                if (get_remaining_tasks_count(&tasks_list) == 0) {
                    break;
                }
                tasks_response.tasks[tasks_response.tasks_count++] = get_task(&tasks_list);
            }

            pthread_mutex_unlock(&tasks_list_mutex);

            MPI_Send(&tasks_response,
                     1,
                     tasks_response_data_type,
                     status.MPI_SOURCE,
                     DEFAULT_LISTENER_RESPONSE_TAG,
                     workers_comm);
        }
    }

    return SUCCESS_CODE;
}

void work(MPI_Comm _workers_comm, int _manager_rank) {
    manager_rank = _manager_rank;
    workers_comm = _workers_comm;

    int rank, size;

    MPI_Comm_rank(workers_comm, &rank);
    MPI_Comm_size(workers_comm, &size);

    const int ROOT_RANK = 0;
    task_data_type = create_task_data_type();
    message_date_type = create_message_data_type();
    tasks_response_data_type = create_tasks_response_data_type(task_data_type);

    pthread_mutex_init(&tasks_list_mutex, NULL);
    pthread_mutex_init(&reached_critical_tasks_count_mutex, NULL);
    pthread_mutex_init(&started_new_list_mutex, NULL);
    pthread_cond_init(&reached_critical_tasks_count_cond, NULL);
    pthread_cond_init(&started_new_list_cond, NULL);

    pthread_attr_t attrs;
    pthread_attr_init(&attrs);

    pthread_t requester, listener;
    pthread_create(&requester, &attrs, requester_routine, NULL);
    pthread_create(&listener, &attrs, listener_routine, NULL);

    remove(OUTPUT_FILE_NAME);

    MPI_File out;
    MPI_File_open(workers_comm,
                  OUTPUT_FILE_NAME,
                  MPI_MODE_WRONLY | MPI_MODE_CREATE,
                  MPI_INFO_NULL,
                  &out);

    char* buffer = calloc(OUTPUT_BUFFER_SIZE, sizeof(char));

    double* iterations_time_ms = calloc(LISTS_NUMBER, sizeof(double));
    double* total_iterations_time_ms = rank == ROOT_RANK
                                       ? calloc((unsigned long) (LISTS_NUMBER * size), sizeof(double))
                                       : NULL;

    double global_result = 0;
    for (int iter_counter = 0; iter_counter < LISTS_NUMBER; iter_counter++) {
        pthread_mutex_lock(&tasks_list_mutex);
        init_tasks_list(&tasks_list);
        for (int task_index = 0; task_index < TASKS_IN_LIST_PER_PROCESS; task_index++) {
            const task_t new_task = create_task(iter_counter, task_index, rank, size);
            add_task(&tasks_list, new_task);
        }

        const double start_time_s = MPI_Wtime();

        size_t tasks_executed = 0;
        started_new_list_flag = true;
        pthread_cond_signal(&started_new_list_cond);
        while (!reached_end(&tasks_list)) {
            const task_t current_task = get_task(&tasks_list);
            pthread_mutex_unlock(&tasks_list_mutex);

            global_result += execute_task(&current_task);

            pthread_mutex_lock(&tasks_list_mutex);
            tasks_executed++;
            const message_t message = {
                    .type = TASKS_UPDATE_TYPE,
                    .value = (int)get_remaining_tasks_count(&tasks_list)
            };
            MPI_Send(&message,
                     1,
                     message_date_type,
                     manager_rank,
                     DEFAULT_NOTIFY_TAG,
                     MPI_COMM_WORLD);

            reached_critical_tasks_count_flag = get_remaining_tasks_count(&tasks_list) < CRITICAL_TASKS_COUNT;

            if (reached_critical_tasks_count_flag) {
                pthread_cond_signal(&reached_critical_tasks_count_cond);
            }
        }
        pthread_mutex_unlock(&tasks_list_mutex);

        const double end_time_s = MPI_Wtime();

        const double current_iteration_time_ms = seconds_to_millis(end_time_s - start_time_s);
        iterations_time_ms[iter_counter] = current_iteration_time_ms;

        for (int i = 0; i < size; i++) {
            if (i == rank) {
                print_iteration_stats(out,
                                      buffer,
                                      tasks_executed,
                                      global_result,
                                      iter_counter,
                                      current_iteration_time_ms,
                                      size,
                                      rank);
            }
            MPI_Barrier(workers_comm);
        }

        pthread_mutex_lock(&tasks_list_mutex);
        free_tasks_list(&tasks_list);
        pthread_mutex_unlock(&tasks_list_mutex);
    }
    finished_all_tasks_flag = true;

    MPI_Datatype recv_type;
    MPI_Datatype resized_recv_type;
    MPI_Type_vector(LISTS_NUMBER, 1, size, MPI_DOUBLE, &recv_type);

    MPI_Type_create_resized(recv_type, 0, sizeof(double), &resized_recv_type);
    MPI_Type_commit(&resized_recv_type);

    MPI_Gather(iterations_time_ms,
               LISTS_NUMBER,
               MPI_DOUBLE,
               total_iterations_time_ms,
               1,
               resized_recv_type,
               ROOT_RANK,
               workers_comm);

    if (rank == ROOT_RANK) {
        const message_t termination_message = {
                .type = TERMINATION_TYPE,
                .value = 0
        };
        MPI_Send(&termination_message,
                 1,
                 message_date_type,
                 manager_rank,
                 0,
                 MPI_COMM_WORLD);

        const final_stats_t final_stats = calculate_final_stats(total_iterations_time_ms, size);
        char* tmp = buffer;
        tmp += sprintf(tmp, "%-40s%.0lf %%\n%-40s%.0lf %%\n%-40s%.0lf %%\n\n",
                       "Min imbalance percentage",
                       final_stats.min_imbalance_part * 100,
                       "Max imbalance percentage",
                       final_stats.max_imbalance_part * 100,
                       "Average imbalance percentage",
                       final_stats.average_imbalance_part * 100);

        MPI_File_write_shared(out,
                              buffer,
                              (int)(tmp - buffer),
                              MPI_CHAR,
                              MPI_STATUS_IGNORE);
    }

    pthread_join(requester, NULL);
    pthread_join(listener, NULL);

    MPI_File_close(&out);

    pthread_attr_destroy(&attrs);

    pthread_mutex_destroy(&tasks_list_mutex);
    pthread_mutex_destroy(&reached_critical_tasks_count_mutex);
    pthread_mutex_destroy(&started_new_list_mutex);

    pthread_cond_destroy(&reached_critical_tasks_count_cond);
    pthread_cond_destroy(&started_new_list_cond);

    free(iterations_time_ms);
    free(total_iterations_time_ms);
    free(buffer);

    MPI_Type_free(&task_data_type);
    MPI_Type_free(&message_date_type);
    MPI_Type_free(&tasks_response_data_type);
    MPI_Type_free(&recv_type);
    MPI_Type_free(&resized_recv_type);
}
