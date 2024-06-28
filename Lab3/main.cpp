#include "CustomErrors.h"
#include "LN.h"
#include "return_codes.h"

#include <algorithm>
#include <fstream>
#include <iostream>
#include <stack>

int main(int argc, const char **argv)
{
	//	if (argc != 3)
	//	{
	//		std::cerr << "Wrong input format. Correct format: <count_of_arguments> <input_file_name>
	//<output_file_name>\n"; 		return ERROR_PARAMETER_INVALID;
	//	}
	std::ifstream in;
	in.open(argv[1]);
	if (!in.is_open())
	{
		std::cerr << "Can't open a file\n";
		return ERROR_CANNOT_OPEN_FILE;
	}
	std::stack< LN > stack;
	std::string line;
	while (in >> line)
	{
		try
		{
			if (line[0] == '-')
			{
				if (line.length() == 1)
				{
					LN op1 = stack.top();
					stack.pop();
					LN op2 = stack.top();
					stack.pop();
					LN res = op1 - op2;
					stack.emplace(res);
				}
				else
				{
					if (line[0] == '=')
					{
						LN op1 = stack.top();
						stack.pop();
						LN op2 = stack.top();
						stack.pop();
						op1 += op2;
						stack.emplace(op1);
					}
					else
					{
						LN num = LN(line);
						stack.emplace(num);
					}
				}
			}
			else if (line[0] == '+' || line[0] == '*' || line[0] == '/' || line[0] == '%' || line[0] == '>' ||
					 line[0] == '<' || line[0] == '=' || line[0] == '!')
			{
				long long boolen = 0;
				LN op1 = stack.top();
				stack.pop();
				LN op2 = stack.top();
				stack.pop();
				LN res = LN(0ll);
				switch (line[0])
				{
				case '+':
					if (line.length() == 2)
					{
						op1 += op2;
						res = op1;
					}
					else
					{
						res = op1 + op2;
					}
					break;
				case '*':
					if (line.length() == 2)
					{
						op1 *= op2;
						res = op1;
					}
					else
					{
						res = op1 * op2;
					}
					break;
				case '/':
					if (line.length() == 2)
					{
						op1 /= op2;
						res = op1;
					}
					else
					{
						res = op1 / op2;
					}
					break;
				case '%':
					if (line.length() == 2)
					{
						op1 %= op2;
						res = op1;
					}
					else
					{
						res = op1 % op2;
					}
					break;
				case '!':
					boolen = op1 != op2;
					res = LN(boolen);
					break;
				case '>':
					if (line.length() == 1)
					{
						boolen = op1 > op2;
					}
					else
					{
						boolen = op1 >= op2;
					}
					res = LN(boolen);
					break;
				case '<':
					if (line.length() == 1)
					{
						boolen = op1 < op2;
					}
					else
					{
						boolen = op1 <= op2;
					}
					res = LN(boolen);
					break;
				case '=':
					boolen = op1 == op2;
					res = LN(boolen);
					break;
				}
				stack.emplace(res);
			}
			else if (line[0] == '_' || line[0] == '~')
			{
				LN op = stack.top();
				stack.pop();
				LN res = LN(0ll);
				switch (line[0])
				{
				case '_':
					res = -op;
					break;
				case '~':
					res = ~op;
					break;
				}
				stack.emplace(res);
			}
			else
			{
				std::transform(line.begin(), line.end(), line.begin(), ::toupper);
				stack.emplace(line);
			}
		} catch (MemoryError &e)
		{
			in.close();
			std::cerr << e.info();
			return ERROR_OUT_OF_MEMORY;
		}
	}
	in.close();
	std::ofstream out;
	out.open(argv[2], std::ios_base::out);
	if (!out.is_open())
	{
		std::cerr << "Can't open a out file\n";
		return ERROR_CANNOT_OPEN_FILE;
	}
	try
	{
		while (!stack.empty())
		{
			LN value = LN(stack.top());
			out << value << std::endl;
			stack.pop();
		}
	} catch (MemoryError &e)
	{
		out.close();
		std::cerr << e.info();
		return ERROR_OUT_OF_MEMORY;
	}
	out.close();
	return SUCCESS;
}