
#pragma once

#include <iostream>

class Util
{
	public:
		enum METHOD
		{
			NONE,
			GET,
			CGI_GET,
			POST,
			CGI_POST,
			DELETE
		};

		static const std::string	GetTimeDateString();
		static const std::string	translateMethod(Util::METHOD method);
};
