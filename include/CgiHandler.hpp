#ifndef CGIHANDLER_HPP
#define CGIHANDLER_HPP

#include "WebServer.hpp"
#include "HttpRequest.hpp"
#include "FileHandler.hpp"
#include "Util.hpp"
#include <fstream>
#include <iostream>
#include <string>
#include <signal.h>
#include <chrono>
#include <thread>
#include <sys/types.h>
#include <sys/wait.h>

class CgiHandler
{
	public:
		static std::string	executeCgi(HttpRequest request);
};

#endif
