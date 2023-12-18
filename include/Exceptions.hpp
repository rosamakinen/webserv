
#pragma once

#include <iostream>

class Exception : public std::exception
{
	protected:
		const char	*_msg;
	public:
		bool		_showToUser;

		Exception(const std::string msg, bool showToUser) { _msg = msg.c_str(); _showToUser = showToUser; };

		virtual const char* what() const throw() = 0;
};

class InternalException : public Exception
{
	public:
		InternalException(const std::string msg) : Exception(msg, false) { };

		virtual const char* what() const throw() = 0;
};
