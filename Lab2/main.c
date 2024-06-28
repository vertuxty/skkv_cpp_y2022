#include "criticalFragments.h"
#include "filterFile.h"
#include "return_codes.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#ifdef ZLIB
#include <zlib.h>
#else
#ifdef LIBDEFLATE
#include <libdeflate.h>
#else
#ifdef ISAL
#include <include/igzip_lib.h>
#else
#error "Such libary not defined"
#endif
#endif
#endif

typedef struct
{
	FILE* in;
	FILE* out;
	unsigned char* data;
} Context;

typedef struct
{
	size_t width;
	size_t height;
	size_t bit_depth;
	size_t colorType;
	size_t compression_method;
	size_t filter_method;
	size_t interlace_method;
} IHDR_part;

typedef struct
{
	unsigned char* de_data;
	size_t size;
} decode_png;

int release_memory(Context* context, int ERROR_CODE)
{
	if (context->in != NULL)
	{
		fclose(context->in);
	}
	if (context->out != NULL)
	{
		fclose(context->out);
	}
	free(context->data);
	return ERROR_CODE;
}

// Теперь считываем тут сразу 4 байта, а не один.
int get_height_or_width(Context* context, unsigned char* header)
{
	int params = 0;
	if (fread(&header[0], sizeof(unsigned char), 4, context->in) != 4)
	{
		return -1;	  // Возвращаю не ERROR_DATA_INVALID, а -1, что бы понимать, есть ли ошибка. Иначе, если мы подадим
		// картинку шириной равной ERROR_DATA_INVALID, то не сможем отличить верные данные от неверных.
	}
	for (size_t i = 0; i < 4; i++)
	{
		params = params * 256 + header[i];
	}
	return params;
}

int write_in_file(Context* context, IHDR_part* ihdrPart, decode_png* decodePng, char* name_out_file)
{
	//    context->out = fopen("C:\\Visual Studio Code C++\\res0.pnm", "wb");
	context->out = fopen(name_out_file, "wb");
	if (context->out == NULL)
	{
		fprintf(stderr, "Writing file not found.");
		fclose(context->out);
		free(decodePng->de_data);
		return ERROR_CANNOT_OPEN_FILE;
	}

	if (ihdrPart->colorType == 0 || ihdrPart->colorType == 3)
	{
		fprintf(context->out, "P5\n%zu %zu\n255\n", ihdrPart->width, ihdrPart->height);
	}
	else
	{
		fprintf(context->out, "P6\n%zu %zu\n255\n", ihdrPart->width, ihdrPart->height);
	}

	int width = (ihdrPart->colorType + 1) * ihdrPart->width + 1;
	for (size_t i = 0; i < ihdrPart->height; i++)
	{
		if (fwrite(&decodePng->de_data[i * (width) + 1], sizeof(unsigned char), width - 1, context->out) != width - 1)
		{
			fprintf(stderr, "Error while writing in file.");
			return release_memory(context, ERROR_PARAMETER_INVALID);
		}
	}
	fclose(context->out);
	free(decodePng->de_data);
	return SUCCESS;
}

int main(int argc, char** argv)
{
	if (argc != 3)
	{
		fprintf(stderr, "Wrong count of arguments!");
		return ERROR_DATA_INVALID;
	}
	Context context[1] = { NULL };
	//    context->in = fopen("C:\\Visual Studio Code C++\\space.png", "rb");
	context->in = fopen(argv[1], "rb");
	if (context->in == NULL)
	{
		fprintf(stderr, "File can't open a file!");
		return release_memory(context, ERROR_CANNOT_OPEN_FILE);
	}

	int sizeOfBite = 8;
	IHDR_part ihdrPart[1] = { 0 };

	unsigned char header[8] = { 0, 0, 0, 0, 0, 0, 0, 0 };

	if (fread(&header[0], sizeof(unsigned char), 8, context->in) != 8)
	{
		fprintf(stderr, "Invalid data in input file, when reading PNG header.");
		return release_memory(context, ERROR_DATA_INVALID);
	}

	if (!isPNG(header))
	{
		fprintf(stderr, "Data in context->data is invalid.");
		return release_memory(context, ERROR_DATA_INVALID);
	}

	// теперь тут тоже считаем сразу 8 байт, а не один.
	if (fread(&header[0], sizeof(unsigned char), 8, context->in) != 8)
	{
		fprintf(stderr, "Invalid data in input file.");
		return release_memory(context, ERROR_DATA_INVALID);
	}

	if (!isIHDR(header))
	{
		fprintf(stderr, "Expected IHDR in context->data, got invalid data");
		return release_memory(context, ERROR_DATA_INVALID);
	}
	// Нахожу ширину картинки
	ihdrPart->width = get_height_or_width(context, header);
	if (ihdrPart->width <= 0)
	{
		fprintf(stderr, "Invalid data in input file.");
		return release_memory(context, ERROR_DATA_INVALID);
	}
	// Нахожу высоту картинки
	ihdrPart->height = get_height_or_width(context, header);
	if (ihdrPart->height <= 0)
	{
		fprintf(stderr, "Invalid data in input file.");
		return release_memory(context, ERROR_DATA_INVALID);
	}

	// Считываю последние биты в IHDR.
	if (fread(&header[0], sizeof(unsigned char), 5, context->in) != 5)
	{
		fprintf(stderr, "Wrong data in input file for bit_depth, colorType, interlace_method, filter_method, compression_method");
		return release_memory(context, ERROR_DATA_INVALID);
	}

	// Проверяю корректность bit_depth
	if (header[0] != sizeOfBite)
	{
		fprintf(stderr, "Bit depth != 8");
		return release_memory(context, ERROR_DATA_INVALID);
	}
	ihdrPart->bit_depth = header[0];

	// Проверяю корректность colorType
	if (header[1] != 0 && header[1] != 2 && header[1] != 3)
	{
		fprintf(stderr, "ColorType = %zu is not supported!", header[1]);
		return release_memory(context, ERROR_DATA_INVALID);
	}
	ihdrPart->colorType = header[1];

	if (header[2] != 0)
	{
		fprintf(stderr, "Unsupported compression_method = %zu, but expected compression_method = 0", header[2]);
		return release_memory(context, ERROR_DATA_INVALID);
	}
	ihdrPart->compression_method = header[2];

	if (header[3] != 0)
	{
		fprintf(stderr, "filer_method should be equals 0, but got %zu", header[3]);
		return release_memory(context, ERROR_DATA_INVALID);
	}
	ihdrPart->filter_method = header[3];

	if (header[4] != 0 && header[5] != 1)
	{	 // В спецификации указано что может быть 0 и 1.
		fprintf(stderr, "interlace_method should be equals 0 or 1, but got %zu", header[4]);
		return release_memory(context, ERROR_DATA_INVALID);
	}
	ihdrPart->interlace_method = header[4];

	// Считываем CRC-32
	int step[4];
	if (fread(&step[0], sizeof(unsigned char), 4, context->in) != 4)
	{
		fprintf(stderr, "Invalid data ");
		return release_memory(context, ERROR_DATA_INVALID);
	}
	// считываем размер следующего чанка.
	int size_block = 0;
	unsigned char step_bytes[4] = { 0, 0, 0, 0 };
	if (fread(&step_bytes[0], sizeof(unsigned char), 4, context->in) != 4)
	{
		fprintf(stderr, "Invalid data in file.");
		return release_memory(context, ERROR_DATA_INVALID);
	}
	for (size_t i = 0; i < 4; i++)
	{
		size_block = size_block * 256 + step_bytes[i];
	}

	int IDATcount = 0;
	int index = 0;
	int pointer = 0;
	int afterIDAT = 0;
	int PLTEcount = 0;
	context->data = malloc(size_block * sizeof(unsigned char));	   // наши данные
	if (context->data == NULL)
	{
		fprintf(stderr, "Not enough memory for context->data");
		return release_memory(context, ERROR_OUT_OF_MEMORY);
	}
	bool begin_flag = true;

	while (isIEND(header) != 1)
	{
		if (!begin_flag)
		{
			// Скипаем CRC сумму.
			if (fread(&step, sizeof(unsigned char), 4, context->in) != 4)
			{
				fprintf(stderr, "Error, EOF!");
				return release_memory(context, ERROR_DATA_INVALID);
			}
			// Узнаем размер блока.
			if (fread(&step_bytes[0], sizeof(unsigned char), 4, context->in) != 4)
			{
				fprintf(stderr, "Invalid data in file.");
				return release_memory(context, ERROR_DATA_INVALID);
			}
			for (size_t i = 0; i < 4; i++)
			{
				size_block = size_block * 256 + step_bytes[i];
			}
		}

		begin_flag = false;

		// Считываем тип блока.
		if (fread(&header[0], sizeof(unsigned char), 4, context->in) != 4)
		{
			fprintf(stderr, "Invalid data in file!");
			return release_memory(context, ERROR_DATA_INVALID);
		}

		if (isIDAT(header) && afterIDAT == 0)
		{
			if (IDATcount == 0)
			{
				unsigned char* temp_data = realloc(context->data, size_block);
				if (temp_data == NULL)
				{
					free(temp_data);
					return release_memory(context, ERROR_OUT_OF_MEMORY);
				}
				context->data = realloc(temp_data, size_block);
				if (context->data == NULL)
				{
					fprintf(stderr, "Error! Not enought memory for reallocate memory for data!");
					return release_memory(context, ERROR_OUT_OF_MEMORY);
				}
				pointer = size_block;
			}
			if (index >= pointer)
			{
				unsigned char* temp_data = realloc(context->data, pointer * 2);
				if (temp_data == NULL)
				{
					free(temp_data);
					fprintf(stderr, "Error! Not enough memory for reallocate memory!");
					return release_memory(context, ERROR_OUT_OF_MEMORY);
				}
				pointer = pointer * 2;
				context->data = realloc(temp_data, pointer);
				if (context->data == NULL)
				{
					fprintf(stderr, "Error! Not enought memory for reallocate memory for data!");
					return release_memory(context, ERROR_OUT_OF_MEMORY);
				}
			}
			if (fread(&context->data[index], sizeof(unsigned char), size_block, context->in) != size_block)
			{
				fprintf(stderr, "Invalid data in stream of IDAT");
				return release_memory(context, ERROR_DATA_INVALID);
			}
			index += size_block;
			IDATcount++;
		}
		else if (isIDAT(header) && afterIDAT != 0)
		{
			fprintf(stderr, "Invalid data! IDAT can't be departed by other chunks!");
			return release_memory(context, ERROR_DATA_INVALID);
		}
		else if (isPLTE(header) && IDATcount != 0)
		{
			fprintf(stderr, "Error, PLTE block should go before IDAT.");
			return release_memory(context, ERROR_DATA_INVALID);
		}
		else if (isPLTE(header) && ihdrPart->colorType == 0)
		{
			fprintf(stderr, "Error, PLTE block can not be in PNG with colorType = 0");
			return release_memory(context, ERROR_DATA_INVALID);
		}
		else if (isPLTE(header))
		{
			if (PLTEcount > 0)
			{
				fprintf(stderr, "Error! PLTE chunk must be one!");
				return release_memory(context, ERROR_DATA_INVALID);
			}

			if (size_block % 3 != 0)
			{
				fprintf(stderr, "Error, PLTE lenght must be divide by 3");
				return release_memory(context, ERROR_DATA_INVALID);
			}
			PLTEcount++;
			unsigned char* wrong_data = malloc(size_block * sizeof(unsigned char));

			if (wrong_data == NULL)
			{
				fprintf(stderr, "Not enough memory for garbage collector!");
				free(wrong_data);
				return release_memory(context, ERROR_OUT_OF_MEMORY);
			}

			if (fread(&wrong_data[0], sizeof(unsigned char), size_block, context->in) != size_block)
			{
				free(wrong_data);
				fprintf(stderr, "Invalid data in stream of PLTE chunk");
				return release_memory(context, ERROR_PARAMETER_INVALID);
			}
			free(wrong_data);
		}
		else if (!isIEND(header))
		{
			if (IDATcount > 0)
			{
				afterIDAT++;
			}

			unsigned char* wrong_data = malloc(size_block * sizeof(unsigned char));

			if (wrong_data == NULL)
			{
				fprintf(stderr, "Not enough memory for garbage collector!");
				free(wrong_data);
				return release_memory(context, ERROR_OUT_OF_MEMORY);
			}

			if (fread(&wrong_data[0], sizeof(unsigned char), size_block, context->in) != size_block)
			{
				free(wrong_data);
				fprintf(stderr, "Invalid data in stream of uncritical chunk");
				return release_memory(context, ERROR_PARAMETER_INVALID);
			}
			free(wrong_data);
		}
		else if (isIEND(header))
		{
			unsigned char* wrong_data = malloc((size_block) * sizeof(unsigned char));
			if (fread(&wrong_data[0], sizeof(unsigned char), size_block, context->in) != size_block)
			{
				fprintf(stderr, "Error Data in checksum for IEND");
				return release_memory(context, ERROR_DATA_INVALID);
			}
			if (fread(&wrong_data[0], sizeof(unsigned char), 8, context->in) == 8)
			{
				fprintf(stderr, "IEND block should be last in png!");
				return release_memory(context, ERROR_DATA_INVALID);
			}
			free(wrong_data);
		}
	}

	if (ihdrPart->colorType == 3 && PLTEcount != 1)
	{
		fprintf(stderr, "Error! You have colorType = 3, so there must be one PLTE chunk, but you got %i", PLTEcount);
		return release_memory(context, ERROR_DATA_INVALID);
	}
	fclose(context->in);
	decode_png decodePng[1] = { NULL };
	decodePng->size = (ihdrPart->colorType + 1) * ihdrPart->width * ihdrPart->height + ihdrPart->height;
	decodePng->de_data = malloc(sizeof(unsigned char) * decodePng->size);
	if (decodePng->de_data == NULL)
	{
		free(decodePng->de_data);
		fprintf(stderr, "Not enough memory for de_data");
		return release_memory(context, ERROR_OUT_OF_MEMORY);
	}

#ifdef ZLIB
	int uncompRes = uncompress((Bytef*)decodePng->de_data, (uLongf*)&decodePng->size, (Bytef*)context->data, index);
	if (uncompRes != Z_OK)
	{
		fprintf(stderr, "Error while uncompressed ZLIB");
		return ERROR_DATA_INVALID;
	}
#endif
#ifdef LIBDEFLATE
	size_t tmp = (ihdrPart->colorType + 1) * ihdrPart->width * ihdrPart->height + ihdrPart->height;
	int uncompRes =
		libdeflate_zlib_decompress(libdeflate_alloc_decompressor(), context->data, index, decodePng->de_data, decodePng->size, &tmp);
	if (uncompRes != LIBDEFLATE_SUCCESS)
	{
		fprintf(stderr, "Error while uncompressing LIBDEFLATE");
		return ERROR_DATA_INVALID;
	}
#endif
#ifdef ISAL
	struct inflate_state decomp;
	isal_inflate_init(&decomp);
	decomp.next_in = context->data;
	decomp.avail_in = index;
	decomp.next_out = decodePng->de_data;
	decomp.avail_out = decodePng->size;
	decomp.crc_flag = ISAL_ZLIB;
	int uncompRes = isal_inflate(&decomp);
	if (uncompRes != ISAL_DECOMP_OK)
	{
		free(decodePng->de_data);
		fprintf(stderr, "Error in uncompressing ISA-L");
		return release_memory(context, ERROR_DATA_INVALID);
	}
#endif

	// освобождаю data.
	free(context->data);

	// Фильтруем наши данные.
	filterFunction(decodePng->de_data, ihdrPart->height, ihdrPart->width, ihdrPart->colorType);

	// Записываем данные в фаил.
	int result_of_writing = write_in_file(context, ihdrPart, decodePng, argv[2]);
	if (result_of_writing != SUCCESS)
	{
		fprintf(stderr, "Error in writing in file!");
		return result_of_writing;
	}
	return SUCCESS;
}
