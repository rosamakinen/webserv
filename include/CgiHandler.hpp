
#pragma once

#include "WebServer.hpp"
#include "HttpRequest.hpp"
#include "FileHandler.hpp"
#include "Util.hpp"
#include <fstream>
#include <iostream>
#include <string>

class CgiHandler
{
	public:
		static int	executeCgi(HttpRequest request);
};
