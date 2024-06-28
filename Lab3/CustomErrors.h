#pragma once
struct MemoryError
{
	const char *info() const;
};
struct FileError
{
	const char *info() const;
};
struct OverflowException
{
	const char *info() const;
};
