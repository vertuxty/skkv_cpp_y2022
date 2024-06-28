#include "return_codes.h"

#include <math.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct
{
	FILE* in;
	FILE* out;
	int size;
	float* matrix;
	float* Q;
	float* R;
	float* Q_h;
	float* R_h;
} Context;

int clear_memory(Context* context, int ERROR_CODE)
{
	if (context->in != NULL)
	{
		fclose(context->in);
	}
	if (context->out != NULL)
	{
		fclose(context->out);
	}
	free(context->matrix);
	free(context->Q);
	free(context->R);
	free(context->Q_h);
	free(context->R_h);
	return ERROR_CODE;
}

void fill_zero(float* R, float* Q, int size)
{
	for (int i = 0; i < size; i++)
	{
		for (int j = 0; j < size; j++)
		{
			R[i * size + j] = 0.0f;
			Q[i * size + j] = 0.0f;
		}
	}
}

void matrix_mul(float* res, const float* R, const float* Q, int size)
{
	for (int i = 0; i < size; i++)
	{
		for (int j = 0; j < size; j++)
		{
			res[i * size + j] = 0;
		}
	}
	for (int i = 0; i < size; i++)
	{
		for (int j = 0; j < size; j++)
		{
			for (int k = 0; k < size; k++)
				res[i * size + j] += R[i * size + k] * Q[k * size + j];
		}
	}
}

void copy_col(const float* matrix1, int j, float* matrix2, int k, int size)
{
	for (int i = 0; i < size; i++)
	{
		matrix2[i * size + k] = matrix1[i * size + j];
	}
}

// v_i = v_i - (a_i, e_j)*e_j
void column_sub(float* matrix1, int j, const float* matrix2, int k, int size)
{
	for (int i = 0; i < size; i++)
	{
		matrix1[i * size + j] -= matrix2[i * size + k];
	}
}

// Умножаем вектор e_j на (a_i, e_j)
void column_d_or_mul(float* matrix, int j, float scalar, int size)
{
	for (int i = 0; i < size; i++)
	{
		matrix[i * size + j] *= scalar;
	}
}

// Ищем длину вектора e_j
float len(const float* matrix, int j, int size)
{
	float length = 0;
	for (int i = 0; i < size; i++)
	{
		length += matrix[i * size + j] * matrix[i * size + j];
	}
	float res = sqrt(length);
	return res;
}

// Вспомогательная функция для вывода матрицы (использовалась для дебага Q и R)
void print_matrix(float* matrix, int size)
{
	for (int i = 0; i < size; i++)
	{
		for (int j = 0; j < size; j++)
		{
			if (fabsf(matrix[i * size + j]) < 1E-4)
			{
				printf("%d ", 0);
			}
			else
			{
				printf("%f ", matrix[i * size + j]);
			}
		}
		printf("\n");
	}
}

float scalar_product(Context* context)
{
	float r = 0.0f;
	for (int i = 0; i < context->size; i++)
	{
		r += context->Q_h[i * context->size] * context->R_h[i * context->size];
	}
	return r;
}

// QR - декомпозиция.
// Алгоритм Грэмма-Шмита.
void QR(Context* context, int size)
{
	fill_zero(context->R_h, context->Q_h, size);
	for (int i = 0; i < size; i++)
	{
		copy_col(context->matrix, i, context->Q, i, size);	  // Копируем i-й вектор-столбец из matrix в i столбец Q

		for (int j = 0; j < i; j++)
		{
			copy_col(context->Q, j, context->Q_h, 0, size);	   // Копируем вектор e_j из Q в Q_h (Q_h = e_j)
			copy_col(context->matrix, i, context->R_h, 0, size);	// Kопируем вектор a_i из matrix в R_h (R_h = a_i)

			float r = scalar_product(context);	  // Скалярное произведение векторов. (a_i, e_j)
			context->R[j * size + i] = r;

			column_d_or_mul(context->Q_h, 0, r, size);			 // (a_i, e_j)*e_j
			column_sub(context->Q, i, context->Q_h, 0, size);	 // v_i = v_i - (a_i, e_j)*e_j
		}

		context->R[i * context->size + i] = len(context->Q, i, size);
		column_d_or_mul(context->Q, i, 1 / context->R[i * size + i], size);	   // Hормируем: v_i/||v_i||
	}
}

void find_complex_eigenvalues(FILE* out, const float* matrix, int size, int i, int j)
{
	float Im, Re;
	float mult_1 = matrix[i * size + j] * matrix[(i + 1) * size + j + 1];
	float mult_2 = matrix[(i + 1) * size + j] * matrix[i * size + j + 1];
	float diff = mult_1 - mult_2;
	Re = matrix[i * size + j] + matrix[(i + 1) * size + j + 1];
	float determinant = pow(Re, 2) - 4 * diff;
	Im = sqrt(fabs(determinant)) / 2;
	Re /= 2;
	if (determinant > 0)
	{
		fprintf(out, "%g\n", Re - Im);
		fprintf(out, "%g\n", Re + Im);
	}
	else
	{
		fprintf(out, "%g +%gi\n", Re, Im);
		fprintf(out, "%g -%gi\n", Re, Im);
	}
}

void answer(FILE* out, float* matrix, int size)
{
	int check = 0;
	for (int i = 0; i < size; i++)
	{
		if (i + 1 < size && fabsf(matrix[(i + 1) * size + i]) <= 1E-1)
		{
			if (!check)
			{
				fprintf(out, "%g\n", matrix[i * size + i]);
			}
			check = 0;
		}
		else if (i + 1 < size)
		{
			if (!check)
			{
				find_complex_eigenvalues(out, matrix, size, i, i);
				check = 1;
			}
			else
			{
				check = 0;
			}
		}
		else
		{
			if (!check)
			{
				fprintf(out, "%g\n", matrix[i * size + i]);
			}
			check = 0;
		}
	}
}

int read_matrix(Context* context)
{
	if (fscanf(context->in, "%i", &context->size) != 1)
	{
		return clear_memory(context, ERROR_DATA_INVALID);
	}

	context->matrix = malloc((context->size) * (context->size) * sizeof(float));
	if (context->matrix == NULL)
	{
		return clear_memory(context, ERROR_OUT_OF_MEMORY);
	}

	for (int i = 0; i < context->size; i++)
	{
		for (int j = 0; j < context->size; j++)
		{
			if (fscanf(context->in, "%f", &context->matrix[i * context->size + j]) != 1)
			{
				return clear_memory(context, ERROR_DATA_INVALID);
			}
		}
	}
	return SUCCESS;
}

void end_free(Context* context)
{
	if (context->R_h != NULL)
	{
		free(context->R_h);
	}
	if (context->Q_h != NULL)
	{
		free(context->Q_h);
	}
	if (context->R != NULL)
	{
		free(context->R);
	}
	if (context->Q != NULL)
	{
		free(context->Q);
	}
	if (context->matrix != NULL)
	{
		free(context->matrix);
	}
}

int main(int const argc, char const ** argv)
{
	if (argc != 3)
	{
		return ERROR_PARAMETER_INVALID;
	}

	Context context[1] = { NULL };
	context->in = fopen(argv[1], "r");

	if (context->in == NULL)
	{
		return clear_memory(context, ERROR_CANNOT_OPEN_FILE);
	}

	int fl = read_matrix(context);

	if (fl != 0)
	{
		return fl;
	}

	fclose(context->in);

	context->Q = malloc((context->size) * (context->size) * sizeof(float));
	if (context->Q == NULL)
	{
		return clear_memory(context, ERROR_OUT_OF_MEMORY);
	}

	context->R = malloc((context->size) * (context->size) * sizeof(float));
	if (context->R == NULL)
	{
		return clear_memory(context, ERROR_OUT_OF_MEMORY);
	}

	context->Q_h = malloc((context->size) * (context->size) * sizeof(float));
	if (context->Q_h == NULL)
	{
		return clear_memory(context, ERROR_OUT_OF_MEMORY);
	}

	context->R_h = malloc((context->size) * (context->size) * sizeof(float));
	if (context->R_h == NULL)
	{
		return clear_memory(context, ERROR_OUT_OF_MEMORY);
	}

	fill_zero(context->R, context->Q, context->size);

	int iter = 1500;
	for (int p = 0; p < iter; p++)
	{
		QR(context, context->size);
		matrix_mul(context->matrix, context->R, context->Q, context->size);
	}

	context->out = fopen(argv[2], "w");

	if (context->out == NULL)
	{
		return clear_memory(context, ERROR_CANNOT_OPEN_FILE);
	}

	answer(context->out, context->matrix, context->size);
	end_free(context);
	fclose(context->out);
	return SUCCESS;
}
