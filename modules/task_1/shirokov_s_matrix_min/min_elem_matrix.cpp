// Copyright 2020 Shirokov Sergey
#include <mpi.h>
#include <iostream>
#include <vector>
#include <random>
#include <algorithm>
#include "./min_elem_matrix.h"
#include "../../../modules/task_1/shirokov_s_matrix_min/min_elem_matrix.h"

int maxElementMatrix(int sizel_f, int sizec_f) {
    int ProcNum, ProcRank, min = 0;
    MPI_Status status;
    MPI_Comm_size(MPI_COMM_WORLD, &ProcNum);
    MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);
    int** matrix = new int* [sizel_f];
    int* array = new int[sizec_f];
    int* array1 = new int[sizec_f];
    for (int i = 0; i < sizel_f; i++) {
        matrix[i] = new int[sizec_f];
    }
    if (ProcRank == 0) {
        matrix_enter(sizel_f, sizec_f, matrix);
        matrix_print(sizel_f, sizec_f, matrix);
        std::cout << std::endl;
    }
    if (ProcNum == sizel_f || sizel_f < ProcNum) {
        for (int i = 0; i < sizel_f; i++) {
            if (i == 0) {
                if (ProcRank == 0) {
                    min = min_el(sizec_f, matrix[0]);
                    array1[0] = min;
                }
            }
            if (i != 0) {
                if (ProcRank == 0) {
                    MPI_Send(matrix[i], sizec_f, MPI_INT, i, i, MPI_COMM_WORLD);
                }
                if (ProcRank == i) {
                    MPI_Recv(array, sizec_f, MPI_INT, 0, i, MPI_COMM_WORLD, &status);
                    min = min_el(sizec_f, array);
                    MPI_Send(&min, 1, MPI_INT, 0, i + 1, MPI_COMM_WORLD);
                }
                if (ProcRank == 0) {
                    MPI_Recv(&array1[i], 1, MPI_INT, i, i + 1, MPI_COMM_WORLD, &status);
                }
            }
        }
    }
    if (ProcRank == 0) {
        min = min_el(sizel_f, array1);
        std::cout << " Min element of matrix: " << min;
    }
    return min;
}
