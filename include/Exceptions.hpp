
#pragma once

#include <iostream>

// Http response handled exceptions
class Exception : public std::logic_error
{
	public:
		Exception(const std::string message) : logic_error(message.c_str()) { }
};

class InternalException : public Exception
{
	public:
		InternalException(const std::string message) : Exception(message) { };
};

class BadRequestException : public Exception
{
	public:
		BadRequestException(const std::string message) : Exception(message) { };
};

class MethodNotAllowedException : public Exception
{
	public:
		MethodNotAllowedException(const std::string message) : Exception(message) { };
};

class NotFoundException : public Exception
{
	public:
		NotFoundException(const std::string message) : Exception(message) { };
};

// Internal exceptions - cause the program to shut down
class ConfigurationException : public std::logic_error
{
	public:
		ConfigurationException(const std::string message) : logic_error(message.c_str()) { };
};

class TimeOutException : public std::logic_error
{
	public:
		TimeOutException(const std::string message) : logic_error(message.c_str()) { };
};

class PollException : public std::logic_error
{
	public:
		PollException(const std::string message) : logic_error(message.c_str()) { };
};

class FileException : public std::logic_error
{
	public:
		FileException(const std::string message) : logic_error(message.c_str()) { };
};
