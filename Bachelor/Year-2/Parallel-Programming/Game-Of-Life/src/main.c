#include <stdlib.h>
#include <stdio.h>
#include <mpi/mpi.h>
#include <malloc.h>
#include "generation.h"

#define ROOT_RANK 0
#define START_CAPACITY 1
#define MAX_ITERATIONS_COUNT 10000

#ifdef DEBUG
void print_time(const char* name, double time_s) {
    printf("%-40s %-8.0f ms\n", name, time_s * 1000);
}
#endif

unsigned char get_alive_neighbours_of_internal_cell(const Cell* generation,
                                                    unsigned long width,
                                                    const unsigned long y,
                                                    const unsigned long x) {

    return GET_CELL(generation, width, y - 1, x) +
           GET_CELL(generation, width, y - 1, (x + 1) % width) +
           GET_CELL(generation, width, y, (x + 1) % width) +
           GET_CELL(generation, width, y + 1, (x + 1) % width) +
           GET_CELL(generation, width, y + 1, x) +
           GET_CELL(generation, width, y + 1, (width + x - 1) % width) +
           GET_CELL(generation, width, y, (width + x - 1) % width) +
           GET_CELL(generation, width, y - 1, (width + x - 1) % width);
}

unsigned char get_alive_neighbours_of_upper_cell(const Cell* generation,
                                                 const Cell* neighbour_bottom_row,
                                                 unsigned long width,
                                                 const unsigned long y,
                                                 const unsigned long x) {

    return neighbour_bottom_row[x] +
           neighbour_bottom_row[(x + 1) % width] +
           GET_CELL(generation, width, y, (x + 1) % width) +
           GET_CELL(generation, width, y + 1, (x + 1) % width) +
           GET_CELL(generation, width, y + 1, x) +
           GET_CELL(generation, width, y + 1, (width + x - 1) % width) +
           GET_CELL(generation, width, y, (width + x - 1) % width) +
           neighbour_bottom_row[(width + x - 1) % width];
}

unsigned char get_neighbours_count_of_bottom_cell(const Cell* generation,
                                                  const Cell* neighbour_upper_row,
                                                  unsigned long width,
                                                  const unsigned long y,
                                                  const unsigned long x) {

    return GET_CELL(generation, width, y - 1, x) +
           GET_CELL(generation, width, y - 1, (x + 1) % width) +
           GET_CELL(generation, width, y, (x + 1) % width) +
           neighbour_upper_row[(x + 1) % width] +
           neighbour_upper_row[x] +
           neighbour_upper_row[(width + x - 1) % width] +
           GET_CELL(generation, width, y, (width + x - 1) % width) +
           GET_CELL(generation, width, y - 1, (width + x - 1) % width);
}

void write_generation_to_file(const char* filename,
                              const Cell* generation,
                              unsigned long local_height,
                              unsigned long width,
                              int rank,
                              int size) {

    MPI_File out;
    MPI_File_open(MPI_COMM_WORLD,
                  filename,
                  MPI_MODE_WRONLY | MPI_MODE_CREATE,
                  MPI_INFO_NULL,
                  &out);

    char* buffer = calloc(local_height * (width + 1), sizeof(char));
    write_generation(generation, local_height, width, buffer);

    for (int i = 0; i < size; i++) {
        MPI_Barrier(MPI_COMM_WORLD);

        if (rank == i) {
            MPI_File_write_shared(out,
                                  buffer,
                                  (int)(local_height * (width + 1)),
                                  MPI_CHAR,
                                  MPI_STATUS_IGNORE);
        }
    }

    free(buffer);
    MPI_File_close(&out);
}

void evolve_cells_in_upper_row(const Cell* current_generation,
                               const Cell* upper_neighbour_bottom_row,
                               unsigned long width,
                               unsigned long* alive_cells_count_in_next_generation,
                               Cell* next_generation) {

    const unsigned long y1 = 0;
    for (unsigned long x = 0; x < width; x++) {
        const unsigned char alive_neighbours_count = get_alive_neighbours_of_upper_cell(current_generation,
                                                                                        upper_neighbour_bottom_row,
                                                                                        width,
                                                                                        y1,
                                                                                        x);

        *alive_cells_count_in_next_generation += evolve(current_generation,
                                                        next_generation,
                                                        width,
                                                        alive_neighbours_count,
                                                        y1,
                                                        x);
    }
}

void evolve_cells_in_bottom_row(const Cell* current_generation,
                                const Cell* bottom_neighbour_upper_row,
                                unsigned long local_height,
                                unsigned long width,
                                unsigned long* alive_cells_count_in_next_generation,
                                Cell* next_generation) {

    const unsigned long y2 = local_height - 1;
    for (unsigned long x = 0; x < width; x++) {
        const unsigned char alive_neighbours_count = get_neighbours_count_of_bottom_cell(current_generation,
                                                                                         bottom_neighbour_upper_row,
                                                                                         width,
                                                                                         y2,
                                                                                         x);

        *alive_cells_count_in_next_generation += evolve(current_generation,
                                                        next_generation,
                                                        width,
                                                        alive_neighbours_count,
                                                        y2,
                                                        x);
    }
}

bool check_repeat(const BitValue* reduced_stop_vector, size_t current_iteration) {
    for (size_t j = 0; j < current_iteration; j++) {
        if (reduced_stop_vector[j]) {
            return true;
        }
    }
    return false;
}

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);

    #ifdef DEBUG
    const double start_time = MPI_Wtime();
    #endif

    const unsigned long height = strtoul(argv[1], NULL, 10);
    const unsigned long width = strtoul(argv[2], NULL, 10);
    const char* in_name = argv[3];
    const char* out_name = argv[4];

    int rank;
    int size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    const unsigned long k1 = rank * height / size;
    const unsigned long k2 = (rank + 1) * height / size;
    const unsigned long local_height = k2 - k1;

    Cell* current_generation = CREATE_GENERATION(local_height, width);

    FILE* in = fopen(in_name, "r");
    fseek(in, (long) (k1 * (width + 1)), SEEK_SET);
    unsigned long alive_cells_in_current_generation = read_generation(current_generation,
                                                                      local_height,
                                                                      width,
                                                                      in);

    size_t capacity = START_CAPACITY;
    Cell** generations_history = calloc(capacity, sizeof(Cell*));
    unsigned long* alive_cells_history = calloc(capacity, sizeof(unsigned long));
    BitValue* local_stop_vector = calloc(capacity, sizeof(BitValue));
    BitValue* reduced_stop_vector = calloc(capacity, sizeof(BitValue));
    Cell* bottom_neighbour_upper_row = calloc(width, sizeof(Cell));
    Cell* upper_neighbour_bottom_row = calloc(width, sizeof(Cell));

    MPI_Request send_upper_row_req;
    MPI_Request send_bottom_row_req;
    MPI_Request recv_bottom_neighbour_upper_row_req;
    MPI_Request recv_upper_neighbour_bottom_row_req;
    MPI_Request reducing_stop_vectors_req;

    #ifdef DEBUG
    double calculating_stop_flags_vector_time = 0;
    double evolving_time = 0;
    double waiting_time_1 = 0;
    double waiting_time_2 = 0;
    double waiting_time_3 = 0;
    #endif

    size_t current_iteration = 0;
    while (current_iteration < MAX_ITERATIONS_COUNT) {
        // Initiate receiving upper-neighbour's bottom row
        MPI_Irecv(upper_neighbour_bottom_row,
                  (int)width,
                  MPI_UINT8_T,
                  (size + rank - 1) % size,
                  1,
                  MPI_COMM_WORLD,
                  &recv_upper_neighbour_bottom_row_req);

        // Initiate receiving bottom-neighbour's upper row
        MPI_Irecv(bottom_neighbour_upper_row,
                  (int)width,
                  MPI_UINT8_T,
                  (rank + 1) % size,
                  0,
                  MPI_COMM_WORLD,
                  &recv_bottom_neighbour_upper_row_req);

        // Initiate sending bottom row
        MPI_Isend(current_generation + width * (local_height - 1),
                  (int)width,
                  MPI_UINT8_T,
                  (rank + 1) % size,
                  1,
                  MPI_COMM_WORLD,
                  &send_bottom_row_req);

        // Initiate sending upper row
        MPI_Isend(current_generation,
                  (int)width,
                  MPI_UINT8_T,
                  (size + rank - 1) % size,
                  0,
                  MPI_COMM_WORLD,
                  &send_upper_row_req);

        #ifdef DEBUG
        const double t1 = MPI_Wtime();
        #endif

        // Calculate vector of stop-flags
        for (size_t j = 0; j < current_iteration; j++) {
            local_stop_vector[j] = alive_cells_in_current_generation != alive_cells_history[j]
                                   ? false
                                   : EQUAL(current_generation, generations_history[j], local_height, width);
        }

        // Initiate sending and "land"-ing stop-vector-flags from all to all
        MPI_Iallreduce(local_stop_vector,
                       reduced_stop_vector,
                       (int)current_iteration,
                       MPI_UINT8_T,
                       MPI_LAND,
                       MPI_COMM_WORLD,
                       &reducing_stop_vectors_req);

        #ifdef DEBUG
        const double t2 = MPI_Wtime();
        #endif

        Cell* next_generation = CREATE_GENERATION(local_height, width);

        unsigned long alive_cells_in_next_generation = 0;

        #ifdef DEBUG
        const double t3 = MPI_Wtime();
        #endif

        // Evolve internal cells
        const unsigned long stop = local_height - 1;
        for (unsigned long y = 1; y < stop; y++) {
            for (unsigned long x = 0; x < width; x++) {
                const unsigned char alive_neighbours_count = get_alive_neighbours_of_internal_cell(current_generation,
                                                                                                   width,
                                                                                                   y,
                                                                                                   x);

                alive_cells_in_next_generation += evolve(current_generation,
                                                         next_generation,
                                                         width,
                                                         alive_neighbours_count,
                                                         y,
                                                         x);
            }
        }

        #ifdef DEBUG
        const double t4 = MPI_Wtime();
        #endif

        bool checked_repeat = false;
        int flag;

        MPI_Test(&reducing_stop_vectors_req, &flag, MPI_STATUS_IGNORE);

        if (flag) {
            checked_repeat = true;

            if (check_repeat(reduced_stop_vector, current_iteration)) {
                break;
            }
        }

        #ifdef DEBUG
        const double t5 = MPI_Wtime();
        #endif

        MPI_Wait(&recv_upper_neighbour_bottom_row_req, MPI_STATUS_IGNORE);

        #ifdef DEBUG
        const double t6 = MPI_Wtime();
        #endif

        evolve_cells_in_upper_row(current_generation,
                                  upper_neighbour_bottom_row,
                                  width,
                                  &alive_cells_in_next_generation,
                                  next_generation);

        #ifdef DEBUG
        const double t7 = MPI_Wtime();
        #endif

        if (!checked_repeat) {
            MPI_Test(&reducing_stop_vectors_req, &flag, MPI_STATUS_IGNORE);

            if (flag) {
                checked_repeat = true;

                if (check_repeat(reduced_stop_vector, current_iteration)) {
                    break;
                }
            }
        }

        #ifdef DEBUG
        const double t8 = MPI_Wtime();
        #endif

        MPI_Wait(&recv_bottom_neighbour_upper_row_req, MPI_STATUS_IGNORE);

        #ifdef DEBUG
        const double t9 = MPI_Wtime();
        #endif

        evolve_cells_in_bottom_row(current_generation,
                                   bottom_neighbour_upper_row,
                                   local_height,
                                   width,
                                   &alive_cells_in_next_generation,
                                   next_generation);

        #ifdef DEBUG
        const double t10 = MPI_Wtime();
        double t11;
        double t12;
        #endif

        if (!checked_repeat) {
            #ifdef DEBUG
            t11 = MPI_Wtime();
            #endif

            MPI_Wait(&reducing_stop_vectors_req, MPI_STATUS_IGNORE);

            #ifdef DEBUG
            t12 = MPI_Wtime();
            #endif

            if (check_repeat(reduced_stop_vector, current_iteration)) {
                break;
            }
        }

        // Add current generation to container of previous generations
        if (capacity == current_iteration) {
            capacity *= 2;
            generations_history = reallocarray(generations_history, capacity, sizeof(Cell*));
            local_stop_vector = reallocarray(local_stop_vector, capacity, sizeof(BitValue));
            reduced_stop_vector = reallocarray(reduced_stop_vector, capacity, sizeof(BitValue));
            alive_cells_history = reallocarray(alive_cells_history, capacity, sizeof(unsigned long));
        }
        generations_history[current_iteration] = current_generation;
        alive_cells_history[current_iteration] = alive_cells_in_current_generation;

        alive_cells_in_current_generation = alive_cells_in_next_generation;

        current_iteration++;

        MPI_Wait(&send_bottom_row_req, MPI_STATUS_IGNORE);
        MPI_Wait(&send_upper_row_req, MPI_STATUS_IGNORE);

        current_generation = next_generation;

        #ifdef DEBUG
        evolving_time += (t4 - t3) + (t7 - t6) + (t10 - t9);
        calculating_stop_flags_vector_time += t2 - t1;
        waiting_time_1 += t6 - t5;
        waiting_time_2 += (t9 - t8);
        waiting_time_3 += (t12 - t11);
        #endif
    }

    write_generation_to_file(out_name,
                             current_generation,
                             local_height,
                             width,
                             rank,
                             size);

    for (size_t i = 0; i < current_iteration; i++) {
        free(generations_history[i]);
    }
    free(generations_history);
    free(current_generation);
    fclose(in);
    free(upper_neighbour_bottom_row);
    free(bottom_neighbour_upper_row);
    free(reduced_stop_vector);
    free(local_stop_vector);
    free(alive_cells_history);

    #ifdef DEBUG
    const double end_time = MPI_Wtime();

    if (rank == ROOT_RANK) {
        printf("%-40s %d\n", "Exceeded max iterations count", current_iteration == MAX_ITERATIONS_COUNT);
        printf("%-40s %zu\n", "Iterations", current_iteration);
        print_time("Evolving", evolving_time);
        print_time("Waiting 1", waiting_time_1);
        print_time("Waiting 2", waiting_time_2);
        print_time("Waiting 3", waiting_time_3);
        print_time("Calculating stop-flags-vector", calculating_stop_flags_vector_time);
        print_time("Total time", end_time - start_time);
    }
    #endif

    MPI_Finalize();
}
