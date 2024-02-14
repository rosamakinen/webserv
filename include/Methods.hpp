#ifndef METHODS_HPP
#define METHODS_HPP

#include "HttpRequest.hpp"
#include "FileHandler.hpp"
#include "WebServer.hpp"


class Methods
{
	public:
		static bool executePost(HttpRequest request);
};

#endif