#include "../include/HttpResponseParser.hpp"

std::string HttpResponseParser::ParseRedirect(const std::string location)
{
	std::string responseString;
	responseString.append(HTTP_VERSION);
	responseString.append(SPACE);
	std::pair<unsigned int, std::string> httpStatus(301, "Redirect");
	responseString.append(std::to_string(httpStatus.first));
	responseString.append(SPACE);
	responseString.append(httpStatus.second);
	responseString.append(HTTP_LINEBREAK);

	responseString.append("Location: ");
	responseString.append(location);
	responseString.append(HTTP_LINEBREAK);
	responseString.append(HTTP_LINEBREAK);

	return responseString;
}

std::string HttpResponseParser::Parse(const HttpResponse &response)
{
	if (response.getCgiResponse().empty() == false)
	{
		return response.getCgiResponse();
	}

	std::string responseString;
	responseString.append(HTTP_VERSION);
	responseString.append(SPACE);
	std::pair<unsigned int, std::string> httpStatus = response.getStatus();
	responseString.append(std::to_string(httpStatus.first));
	responseString.append(SPACE);
	responseString.append(httpStatus.second);
	responseString.append(HTTP_LINEBREAK);

	responseString.append("Date: ");
	responseString.append(Util::getTimeDateString());
	responseString.append(HTTP_LINEBREAK);
	responseString.append("Content-Length: ");
	responseString.append(std::to_string(response.getContentLenght()));
	responseString.append(HTTP_LINEBREAK);
	responseString.append("Content-Type: ");
	responseString.append(response.getContentType());
	responseString.append(HTTP_LINEBREAK);
	responseString.append(HTTP_LINEBREAK);

	responseString.append(response.getBody());
	responseString.append(HTTP_LINEBREAK);
	responseString.append(HTTP_LINEBREAK);

	return responseString;
}
