#include <stdio.h>
#include <stdlib.h>

#define key 53;

float *multiply_scalar(float *matrix, int scalar, int rows, int columns) {
  int i, j;
  for (i = 0; i < rows; i++) {
    for (j = 0; j < columns; j++) {
      matrix[i * columns + j] *= scalar;
    }
  }
  return matrix;
}

float *multiply_matrix(float *matrix1, float *matrix2, int rows1, int columns1,
                       int rows2, int columns2) {
  int i, j, k;
  float *matrix3 = (float *)malloc(rows1 * columns2 * sizeof(float));
  for (i = 0; i < rows1; i++) {
    for (j = 0; j < columns2; j++) {
      matrix3[i * columns2 + j] = 0;
      for (k = 0; k < columns1; k++) {
        matrix3[i * columns2 + j] +=
            matrix1[i * columns1 + k] * matrix2[k * columns2 + j];
      }
    }
  }
  return matrix3;
}

float determinant2x2(float *matrix) {
  float det = matrix[0] * matrix[3] - matrix[1] * matrix[2];
  return det;
}

float determinant3x3(float *matrix) {
  float det = matrix[0] * (matrix[4] * matrix[8] - matrix[5] * matrix[7]) -
              matrix[1] * (matrix[3] * matrix[8] - matrix[5] * matrix[6]) +
              matrix[2] * (matrix[3] * matrix[7] - matrix[4] * matrix[6]);
  return det;
}

float determinant4x4(float *matrix) {
  float det =
      matrix[0] *
          (matrix[5] * (matrix[10] * matrix[15] - matrix[11] * matrix[14]) -
           matrix[6] * (matrix[9] * matrix[15] - matrix[11] * matrix[13]) +
           matrix[7] * (matrix[9] * matrix[14] - matrix[10] * matrix[13])) -
      matrix[1] *
          (matrix[4] * (matrix[10] * matrix[15] - matrix[11] * matrix[14]) -
           matrix[6] * (matrix[8] * matrix[15] - matrix[11] * matrix[12]) +
           matrix[7] * (matrix[8] * matrix[14] - matrix[10] * matrix[12])) +
      matrix[2] *
          (matrix[4] * (matrix[9] * matrix[15] - matrix[11] * matrix[13]) -
           matrix[5] * (matrix[8] * matrix[15] - matrix[11] * matrix[12]) +
           matrix[7] * (matrix[8] * matrix[13] - matrix[9] * matrix[12])) -
      matrix[3] *
          (matrix[4] * (matrix[9] * matrix[14] - matrix[10] * matrix[13]) -
           matrix[5] * (matrix[8] * matrix[14] - matrix[10] * matrix[12]) +
           matrix[6] * (matrix[8] * matrix[13] - matrix[9] * matrix[12]));
  return det;
}

float *find_reverse_matrix(float *matrix, int rows, int columns) {
  int i, j, k;

  float *matrix2 = (float *)malloc(rows * columns * sizeof(float));
  for (i = 0; i < rows; i++) {
    for (j = 0; j < columns; j++) {
      matrix2[i * columns + j] = matrix[i * columns + j];
    }
  }

  float *matrix3 = (float *)malloc(rows * columns * sizeof(float));
  for (i = 0; i < rows; i++) {
    for (j = 0; j < columns; j++) {
      if (i == j) {
        matrix3[i * columns + j] = 1;
      } else {
        matrix3[i * columns + j] = 0;
      }
    }
  }

  for (i = 0; i < rows; i++) {
    if (matrix2[i * columns + i] == 0) {
      for (k = i + 1; k < rows; k++) {
        if (matrix2[k * columns + i] != 0) {
          for (j = 0; j < columns; j++) {
            int temp = matrix2[i * columns + j];
            matrix2[i * columns + j] = matrix2[k * columns + j];
            matrix2[k * columns + j] = temp;

            temp = matrix3[i * columns + j];
            matrix3[i * columns + j] = matrix3[k * columns + j];
            matrix3[k * columns + j] = temp;
          }
          break;
        }
      }
    }

    int pivot = matrix2[i * columns + i];
    if (pivot == 0) {
      printf("Matrix is not invertible.\n");
      free(matrix2);
      free(matrix3);
      return NULL;
    }

    for (j = 0; j < columns; j++) {
      matrix2[i * columns + j] /= pivot;
      matrix3[i * columns + j] /= pivot;
    }

    for (k = 0; k < rows; k++) {
      if (k != i) {
        int factor = matrix2[k * columns + i];
        for (j = 0; j < columns; j++) {
          matrix2[k * columns + j] -= factor * matrix2[i * columns + j];
          matrix3[k * columns + j] -= factor * matrix3[i * columns + j];
        }
      }
    }
  }

  free(matrix2);
  return matrix3;
}

float *divide_matrix(float *matrix, int rows, int columns, float number) {
  int i, j;
  for (i = 0; i < rows; i++) {
    for (j = 0; j < columns; j++) {
      matrix[i * columns + j] /= number;
    }
  }
  return matrix;
}

float *transport_matrix(float *matrix, int rows, int columns) {
  int i, j;
  float *matrix2 = (float *)malloc(rows * columns * sizeof(float));
  for (i = 0; i < rows; i++) {
    for (j = 0; j < columns; j++) {
      matrix2[j * rows + i] = matrix[i * columns + j];
    }
  }
  return matrix2;
}

void encryptMatrix(float *matrix, int size) {
  for (int i = 0; i < size; i++) {
    matrix[i] = matrix[i] * key;
  }
}

void decryptMatrix(float *matrix, int size) {
  for (int i = 0; i < size; i++) {
    matrix[i] = matrix[i] / key;
  }
}

void writeMatrixToFile(const char *filename, float *matrix, int rows,
                       int columns) {
  FILE *file = fopen(filename, "w");
  if (file == NULL) {
    printf("Failed to open the file.\n");
    return;
  }

  encryptMatrix(matrix, rows * columns);

  fprintf(file, "%d %d\n", rows, columns);

  for (int i = 0; i < rows; i++) {
    for (int j = 0; j < columns; j++) {
      fprintf(file, "%.2f ", matrix[i * columns + j]);
    }
    fprintf(file, "\n");
  }

  printf("Matrix written to file.\n");

  fclose(file);
}

float *readMatrixFromFile(const char *filename, int *rows, int *columns) {
  FILE *file = fopen(filename, "r");
  if (file == NULL) {
    printf("Failed to open the file.\n");
    return NULL;
  }

  fscanf(file, "%d %d", rows, columns);

  int size = (*rows) * (*columns);
  float *matrix = (float *)malloc(size * sizeof(float));

  for (int i = 0; i < size; i++) {
    fscanf(file, "%f", &matrix[i]);
  }

  decryptMatrix(matrix, size);

  fclose(file);
  return matrix;
}

int main() {

  int rows1, columns1, rows2, columns2;
  float determined;

  printf("Enter the number of rows and columns of the first matrix: ");
  scanf("%d %d", &rows1, &columns1);
  printf("Enter the number of rows and columns of the second matrix: ");
  scanf("%d %d", &rows2, &columns2);
  if (columns1 != rows2) {
    printf("The number of columns of the first matrix must be equal to the "
           "number of rows of the second matrix.\n");
    return 0;
  }
  float *matrix1 = (float *)malloc(rows1 * columns1 * sizeof(float));
  float *matrix2 = (float *)malloc(rows2 * columns2 * sizeof(float));
  int i, j;
  printf("Enter the elements of the first matrix:\n");
  for (i = 0; i < rows1; i++) {
    for (j = 0; j < columns1; j++) {
      scanf("%f", &matrix1[i * columns1 + j]);
    }
  }
  printf("Enter the elements of the second matrix:\n");
  for (i = 0; i < rows2; i++) {
    for (j = 0; j < columns2; j++) {
      scanf("%f", &matrix2[i * columns2 + j]);
    }
  }
  printf("The first matrix is:\n");
  for (i = 0; i < rows1; i++) {
    for (j = 0; j < columns1; j++) {
      printf("%f ", matrix1[i * columns1 + j]);
    }
    printf("\n");
  }
  printf("The second matrix is:\n");
  for (i = 0; i < rows2; i++) {
    for (j = 0; j < columns2; j++) {
      printf("%f ", matrix2[i * columns2 + j]);
    }
    printf("\n");
  }
  float *matrix3 =
      multiply_matrix(matrix1, matrix2, rows1, columns1, rows2, columns2);
  printf("The multiplication of the two matrices is:\n");
  for (i = 0; i < rows1; i++) {
    for (j = 0; j < columns2; j++) {
      printf("%f ", matrix3[i * columns2 + j]);
    }
    printf("\n");
  }

  float scalar;
  printf("Enter the scalar: ");
  scanf("%f", &scalar);
  float *matrix4 = multiply_scalar(matrix1, scalar, rows1, columns1);
  printf("The multiplication of the first matrix by the scalar is:\n");
  for (i = 0; i < rows1; i++) {
    for (j = 0; j < columns1; j++) {
      printf("%f ", matrix4[i * columns1 + j]);
    }
    printf("\n");
  }
  if (rows1 != columns1) {
    printf(
        "Matrix1 is not a square matrix and we cant calculate the determined");
  }

  else {
    if (rows1 == 2) {
      determined = determinant2x2(matrix1);
    } else if (rows1 == 3) {
      determined = determinant3x3(matrix1);
    } else if (rows1 == 4) {
      determined = determinant4x4(matrix1);
    }
    printf("The determined of the first matrix is: %f\n", determined);
  }
  if (determined == 0) {
    printf("The inverse of the first matrix does not exist.\n");
  } else {
    float *matrix5 = find_reverse_matrix(matrix1, rows1, columns1);
    if (matrix5 != NULL) {
      printf("The inverse of the first matrix is:\n");
      for (i = 0; i < rows1; i++) {
        for (j = 0; j < columns1; j++) {
          printf("%f ", matrix5[i * columns1 + j]);
        }
        printf("\n");
      }
    }
  }
  float *matrix6 = divide_matrix(matrix1, rows1, columns1, scalar);
  printf("The division of the first matrix by the scalar is:\n");
  for (i = 0; i < rows1; i++) {
    for (j = 0; j < columns1; j++) {
      printf("%f ", matrix6[i * columns1 + j]);
    }
    printf("\n");
  }

  float *matrix7 = transport_matrix(matrix1, rows1, columns1);
  printf("The transport of the first matrix is:\n");
  for (i = 0; i < columns1; i++) {
    for (j = 0; j < rows1; j++) {
      printf("%f ", matrix7[i * rows1 + j]);
    }
    printf("\n");
  }

  writeMatrixToFile("matrici.txt", matrix1, rows1, columns1);

  matrix2 = readMatrixFromFile("matrici.txt", &rows2, &columns2);
  if (matrix2 == NULL) {
    printf("Failed to read matrix from file.\n");
    return 0;
  }

  printf("Matrix read from file:\n");
  for (int i = 0; i < rows2; i++) {
    for (int j = 0; j < columns2; j++) {
      printf("%.2f ", matrix2[i * columns1 + j]);
    }
    printf("\n");
  }

  return 0;
}
