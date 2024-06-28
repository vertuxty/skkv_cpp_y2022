#include "CustomErrors.h"
const char* FileError::info() const
{
	return "Error, file don't open!\n";
}
const char* MemoryError::info() const
{
	return "Error, not enough memory!\n";
}
const char* OverflowException::info() const
{
	return "Error, overflow!\n";
}