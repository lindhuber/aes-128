#pragma once

#include <string>

enum Operation
{
	Encryption,
	Decryption
};

enum Mode
{
	ECB
};

struct Arguments
{
	Operation operation;
	Mode mode;
	std::string input_filename;
	std::string output_filename;
	std::string passphrase;
	bool error;
};

const int ARGS_NUM_MIN = 3;

std::string strip_extension(const std::string& filename)
{
	auto last_dot = filename.find_last_of(".");
	if (last_dot == std::string::npos)
	{
		return filename;
	}
	return filename.substr(0, last_dot);
}

std::string get_extension(const std::string& filename)
{
	auto last_dot = filename.find_last_of(".");
	if (last_dot == std::string::npos)
	{
		return "";
	}
	return filename.substr(last_dot, filename.length());
}

Arguments parse_args(int argc, char* argv[])
{
	Arguments args;
	args.error = true;
	
	// has to atleast contain operation flag and input file
	if (argc < ARGS_NUM_MIN)
	{
		return args;
	}

	std::string operation = argv[1];
	if (operation == "-e")
	{
		args.operation = Encryption;
	}
	else if (operation == "-d")
	{
		args.operation = Decryption;
	}
	else
	{
		return args;
	}

	std::string mode = argv[2];
	if (mode == "ecb" || mode == "ECB")
	{
		args.mode = ECB;
	}
	else
	{
		return args;
	}

	args.input_filename = argv[3];
	args.output_filename = args.mode == Encryption ? args.input_filename + ".enc" : strip_extension(args.input_filename);

	args.passphrase = argv[4];

	args.error = false;

	return args;
}