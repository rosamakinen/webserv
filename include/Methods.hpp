#ifndef METHODS_HPP
#define METHODS_HPP

#include "HttpRequest.hpp"
#include "FileHandler.hpp"
#include "WebServer.hpp"
#include "Exceptions.hpp"
#include <cstdio>


class Methods
{
	public:
		static void executePost(HttpRequest request);
		static void executeDelete(HttpRequest request);
};

#endif
