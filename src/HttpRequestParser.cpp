#include "../include/HttpRequestParser.hpp"

HttpRequestParser::HttpRequestParser() {}

HttpRequestParser::~HttpRequestParser() {}

HttpRequest *HttpRequestParser::parseHttpRequest(std::string requestInput, std::map<std::string, Server *>& servers)
{
	HttpRequest *request = new HttpRequest();

	try
	{
		std::stringstream ss(requestInput);
		std::string requestLine;

		getline(ss, requestLine);
		parseRequestLine(requestLine, request);

		while (getline(ss, requestLine))
		{
			if (requestLine.compare("\r") == 0)
				break;
			parseHeader(requestLine, request);
		}

		parseHost(request);
		Server *server = getServer(request, servers);
		parseDirectoryAndLocation(request, server);
		if (request->getIsRedirected())
			return request;
		validateMethod(request, server);
		parseIndexPathAndDirectoryListing(request, server);
		parseCgiMethod(request);
		parseContentLength(request);
		parseContentType(request);

		if (request->getMethod() == Util::METHOD::POST || request->getMethod() == Util::METHOD::CGI_POST)
		{
			request->setBodyLength(countBody(requestInput, request));
			std::string contentType = request->getHeader(H_CONTENT_TYPE);
			if (contentType.compare(CT_TXT) == 0 || contentType.compare(CT_FRM) == 0 || contentType.compare(CT_TXT2) == 0)
			{
				while (getline(ss, requestLine))
				{
					if (requestLine.compare("\r") == 0)
						break;
					request->appendBody(requestLine);
					request->appendBody("\n");
				}
				static int i;
				std::string filename = std::to_string(i);
				filename.append(".txt");
				request->setFileName(filename);
				i++;
			}

			if (contentType.find(CT_MLTP) != std::string::npos)
			{
				size_t bound_pos = contentType.find(" boundary=");
					if (bound_pos == std::string::npos)
						throw BadRequestException("No boundary given for multipart request");

				std::string boundary = contentType.substr(bound_pos + 10);

				std::string body_part_boundary = MLTP_LINEBREAK;
				body_part_boundary.append(boundary);
				body_part_boundary.append("\r");

				std::string last_line_boundary = MLTP_LINEBREAK;
				last_line_boundary.append(boundary);
				last_line_boundary.append(MLTP_LINEBREAK);
				last_line_boundary.append("\r");

				while (getline(ss, requestLine))
				{
					if (requestLine.compare(body_part_boundary) == 0)
					{
						getline(ss, requestLine);
						size_t dis_pos = requestLine.find("Content-Disposition: form-data;");
						if (dis_pos == std::string::npos)
							throw BadRequestException("Invalid Content-Disposition given on request");
						dis_pos = requestLine.find("filename=");
						if (dis_pos == std::string::npos)
							throw BadRequestException("No filename given on request");
						if (request->getFileName().empty())
							request->setFileName(requestLine.substr(dis_pos + 10, ((requestLine.length() - (dis_pos + 10)) - 2)));
					}
					else if (requestLine.find("Content-Type:") != std::string::npos)
					{
						size_t cs_pos = requestLine.find("Content-Type: ");
						if (cs_pos == std::string::npos)
							throw BadRequestException("Invalid Content-Type given on request");
						std::string content = requestLine.substr(cs_pos + 14);
						if (content.compare("text/plain\r") != 0)
							throw BadRequestException("Invalid Content-Type given on request");

						while (getline(ss, requestLine))
						{
							if (requestLine.compare("\r") == 0)
								continue;
							if (requestLine.compare(last_line_boundary) == 0)
								break;
							request->appendBody(requestLine);
							request->appendBody("\n");
						}
					}
					if (requestLine.compare(last_line_boundary) == 0)
						break;

					if (requestLine.compare("\r") == 0)
						continue;
				}
			}
		}
		validateSize(request, server);
	}
	catch(const Exception& e)
	{
		delete request;
		throw;
	}
	return request;
}

size_t HttpRequestParser::countBody(std::string requestInput, HttpRequest *request)
{
	std::string bodystr;
	std::stringstream ss(requestInput);
	std::string line;
	while (getline(ss, line))
	{
		if (line.empty() || line.compare("\r") == 0)
			break;
	}
	while (getline(ss, line))
	{
		bodystr.append(line);
		bodystr.append("\n");
	}
	size_t count;
	if (request->getContentType().compare("multipart/form-data") == 0)
	{
		count = bodystr.length();
	}
	else
	{
		if (bodystr.empty() || bodystr.length() == 0)
			count = 0;
		else
			count = bodystr.length() - 1;
	}
	return count;
}

Server *HttpRequestParser::getServer(HttpRequest *request, std::map<std::string, Server *>& servers)
{
	return Server::getServer(request->getServerName(), request, servers);
}

void HttpRequestParser::parseHost(HttpRequest *request)
{
	std::string host = request->getHeader("Host");
	request->setHost(host);
	if (host.empty())
		throw BadRequestException("Missing the Host header");

	std::string	name;
	size_t	colon_pos = host.find_last_of(':');
	request->setServerName(host.substr(0, colon_pos));
	int port = -1;
	if (colon_pos == std::string::npos)
	{
		request->setHasHostDefined(true);
		request->setPort(port);
		return;
	}

	try
	{
		port = std::stoi(host.substr(colon_pos + 1));
	}
	catch(const std::logic_error& e)
	{
		throw BadRequestException("Invalid Host header port");
	}

	request->setHasHostDefined(true);
	request->setPort(port);
}

static std::vector<std::string> _contenttypes =
{
	CT_TXT,
	CT_TXT2,
	CT_MLTP,
	CT_FRM
};

bool HttpRequestParser::validContentType(std::string contentTypeToFind)
{
	for (auto ct : _contenttypes)
	{
		if (contentTypeToFind.find(CT_TXT) != std::string::npos)
			return true;
		if (contentTypeToFind.find(CT_TXT2) != std::string::npos)
			return true;
		if (contentTypeToFind.find(CT_MLTP) != std::string::npos)
			return true;
		if (contentTypeToFind.find(CT_FRM) != std::string::npos)
			return true;
	}

	return false;
}

void HttpRequestParser::parseContentType(HttpRequest *request)
{
	std::string method = Util::translateMethod(request->getMethod());
	if (method.compare(HTTP_POST) == 0)
	{
		std::string contentType = request->getHeader(H_CONTENT_TYPE);
		size_t found = contentType.find(';');
		if (found != std::string::npos)
		{
			std::string multiType = contentType.substr(0, found);
			request->setContentType(multiType);
		}
		else
			request->setContentType(contentType);
		if (request->getContentType().empty() && request->getParameters().empty())
			throw BadRequestException("No Content-Type for POST request");
		if (!validContentType(contentType))
			throw UnsupportedMediaTypeException("Do not support given media type");
	}
}

void HttpRequestParser::parseContentLength(HttpRequest *request)
{
	size_t length = 0;
	std::string contentLengthString = request->getHeader(H_CONTENT_LENGTH);

	if (contentLengthString.empty() || contentLengthString.compare("0") == 0)
	{
		if (request->getMethod() == Util::METHOD::POST || request->getMethod() == Util::METHOD::CGI_POST)
		{
			if (request->getParameters().empty() && request->getBody().empty())
				throw BadRequestException("Expected body or query parameters with POST request");
			else if (request->getParameters().empty())
				throw BadRequestException("Expected the Content-Length header with POST request with body");
		}

		request->setContentLength(length);
		return;
	}

	if (request->getMethod() != Util::METHOD::POST && request->getMethod() != Util::METHOD::CGI_POST)
	{
		request->setContentLength(length);
		return;
	}

	try
	{
		length = std::stoi(contentLengthString);
	}
	catch(const std::logic_error& e)
	{
		throw BadRequestException("Could not parse the header Content-Length");
	}

	request->setContentLength(length);
}

void HttpRequestParser::parseRequestLine(std::string &requestLine, HttpRequest *request)
{
	if (requestLine.empty() || requestLine.length() < 14)
		throw BadRequestException("Invalid requestline");
	parseMethod(requestLine, request);
	parseUri(requestLine, request);
	validateVersion(requestLine);
}

void HttpRequestParser::validateSize(HttpRequest *request, Server *server)
{
	if ((request->getMethod() == Util::METHOD::POST || request->getMethod() == Util::METHOD::CGI_POST)
		 && request->getContentLength() > server->getClientMaxBodySize())
		throw PayloadTooLargeException("Request body is too large");
}

void HttpRequestParser::parseDirectoryAndLocation(HttpRequest *request, Server *server)
{
	std::string directoryPath = Util::getDirectoryFromUri(request->getUri());
	if (!server->isLocationInServer(directoryPath))
		throw NotFoundException("Location does not exist");
	request->setLocation(directoryPath);
	if (parseRedirection(request, server))
		return ;

	const std::vector<std::string>* workingDir = server->getLocationValue(directoryPath, LOCAL_DIR);
	if (workingDir == nullptr || workingDir->size() != 1)
		throw BadRequestException("Location has missing or invalid values");

	request->setDirectory(workingDir->front());
}

void HttpRequestParser::validateMethod(HttpRequest *request, Server *server)
{
	const std::vector<std::string> *values = server->getLocationValue(request->getLocation(), HTTP_METHOD);
	if (values == nullptr || values->size() < 1)
		throw MethodNotAllowedException("Requested method is not allowed for the location");

	Util::METHOD method = request->getMethod();
	for (std::vector<std::string>::const_iterator it = values->begin(); it != values->end(); it++)
	{
		if (it->compare(Util::translateMethod(method)) == 0)
			return;
	}

	throw MethodNotAllowedException("Requested method is not supported");
}

void HttpRequestParser::parseMethod(std::string &requestLine, HttpRequest *request)
{
	Util::METHOD method = Util::METHOD::NONE;
	if (compareAndSubstring("GET ", requestLine) == 0)
		method = Util::METHOD::GET;
	else if (compareAndSubstring("POST ", requestLine) == 0)
		method = Util::METHOD::POST;
	else if (compareAndSubstring("DELETE ", requestLine) == 0)
		method = Util::METHOD::DELETE;
	else if (compareAndSubstring("PUT ", requestLine) == 0)
		method = Util::METHOD::PUT;
	else if (compareAndSubstring("HEAD ", requestLine) == 0)
		method = Util::METHOD::HEAD;
	else if (compareAndSubstring("OPTIONS ", requestLine) == 0)
		method = Util::METHOD::OPTIONS;
	else if (compareAndSubstring("CONNECT ", requestLine) == 0)
		method = Util::METHOD::CONNECT;
	else if (compareAndSubstring("TRACE ", requestLine) == 0)
		method = Util::METHOD::TRACE;
	else if (compareAndSubstring("PATCH ", requestLine) == 0)
		method = Util::METHOD::PATCH;
	else
		parseMethodStr(requestLine);

	request->setMethod(method);
}


bool HttpRequestParser::parseRedirection(HttpRequest *request, Server *server)
{
	const std::vector<std::string> *redirectionValues = server->getLocationValue(request->getLocation(), REDIR);
	if (redirectionValues != nullptr)
	{
		std::string location = redirectionValues->front();
		request->setRedirLocation(location);
		request->setIsRedirected(true);
	}

	return request->getIsRedirected();
}

void HttpRequestParser::parseIndexPathAndDirectoryListing(HttpRequest *request, Server *server)
{
	std::string uri = request->getUri();
	if (request->getMethod() == Util::METHOD::POST)
	{
		std::string indexPath = request->getDirectory();
		indexPath.append(Util::getFileFromUri(request->getUri()));
		request->setResourcePath(indexPath);
		return;
	}
	if (uri[uri.length() - 1] == '/')
	{
		const std::vector<std::string> *indexValues = server->getLocationValue(request->getLocation(), INDEX);
		if (indexValues != nullptr)
		{
			std::string indexPath = request->getDirectory();
			indexPath.append(indexValues->front());
			request->setResourcePath(indexPath);
			return;
		}

		const std::vector<std::string> *autoIndexValues = server->getLocationValue(request->getLocation(), AUTO_INDEX);
		if (autoIndexValues != nullptr && autoIndexValues->size() >= 1 && autoIndexValues->front().compare("true") == 0)
		{
			request->setResourcePath(request->getDirectory());
			request->setIsDirListing(true);
			return;
		}

		throw ForbiddenException("Autoindexing not enabled for requested resource");
	}
	else
	{
		std::string indexPath = request->getDirectory();
		indexPath.append(Util::getFileFromUri(request->getUri()));
		request->setResourcePath(indexPath);
	}
}

void HttpRequestParser::parseCgiMethod(HttpRequest *request)
{
	Util::METHOD method = request->getMethod();
	std::string path = request->getResourcePath();
	if (!findCgi(path))
		return ;

	if (method == Util::METHOD::GET)
		request->setMethod(Util::METHOD::CGI_GET);
	else if (method == Util::METHOD::POST)
		request->setMethod(Util::METHOD::CGI_POST);
}

bool HttpRequestParser::findCgi(std::string uri)
{
	size_t found = uri.find(CGI_LOCATION);
	if (found != std::string::npos)
	{
		bool ret = validateCgi(uri);
		return ret;
	}
	return false;
}

bool HttpRequestParser::validateCgi(std::string path)
{
	std::string suffix = ".py";
	std::string fullPath = FileHandler::getFilePath(path);
	if (access(fullPath.c_str(), X_OK) == 0)
	{
		size_t pos = fullPath.find(suffix);
		if (pos != std::string::npos)
			return true;
	}
	throw BadRequestException("Bad CGI request");
	return false;
}

int	HttpRequestParser::compareAndSubstring(std::string method, std::string &requestLine)
{
	if (requestLine.compare(0, method.length(), method) == 0)
	{
		requestLine = requestLine.substr(method.length(), requestLine.length());
		return 0;
	}
	return 1;
}

void HttpRequestParser::validateVersion(std::string &requestLine)
{
	size_t version = requestLine.find(HTTP_VERSION);
	if (version == std::string::npos)
		throw HttpVersionNotSupportedException("Unsupported HTTP version");
}

void HttpRequestParser::parseMethodStr(std::string &requestLine)
{
	std::string method;
	size_t		pos;

	pos = requestLine.find(' ');
	method = requestLine.substr(0, pos);
	requestLine = requestLine.substr(pos + 1, requestLine.length());
}

void HttpRequestParser::parseParameters(std::string uri, std::map<std::string, std::string>& parameters)
{
	std::stringstream input(uri.append("&"));
	std::string segment;
	while (std::getline(input, segment, '&'))
	{
		size_t pos = segment.find('=');
		std::string key = segment.substr(0, pos);
		std::string value = segment.substr(pos + 1, segment.length());
		if (pos == std::string::npos || key.empty() || value.empty())
			throw BadRequestException("Invalid request parameter");

		std::pair<std::map<std::string, std::string>::iterator, bool> result;
		result = parameters.insert(std::pair<std::string, std::string>(key, value));
		if (result.second == false)
			throw BadRequestException("Duplicate request parameter");
	}
}

void HttpRequestParser::parseUri(std::string &requestLine, HttpRequest *request)
{
	std::string uri;
	std::map<std::string, std::string> parameters;
	size_t uriPos = requestLine.find('?');
	size_t paramPos = requestLine.find(' ');
	if (paramPos == std::string::npos)
		throw BadRequestException("Invalid requestline");
	if (uriPos < paramPos)
	{
		uri = requestLine.substr(0, uriPos);
		requestLine = requestLine.substr(uriPos + 1, requestLine.length());
		parseParameters(requestLine.substr(0, requestLine.find(' ')), parameters);
	}
	else
	{
		uri = requestLine.substr(0, paramPos);
		requestLine = requestLine.substr(paramPos, requestLine.length());
	}

	request->setUri(uri);
	request->setParameters(parameters);
}

const std::string HttpRequestParser::getHeaderValue(std::map<std::string, std::string> &headers, std::string toFind)
{
	std::map<std::string, std::string>::iterator it;

	it = headers.find(toFind);
	if (it == headers.end())
		throw BadRequestException("Header not found");
	return it->second;
}

void HttpRequestParser::parseHeader(const std::string &line, HttpRequest *request)
{
	size_t pos = line.find(':');
	if (pos != std::string::npos)
	{
		std::string key = line.substr(0, pos);
		std::string value = line.substr(pos + 2);
		value.replace(value.length() - 1, value.length(), "");

		if (!request->setHeader(key, value))
			throw BadRequestException("Duplicate header found");
	}
}

void HttpRequestParser::parseBody(std::string newLine, std::string &body)
{
	newLine.replace(newLine.length() - 1, newLine.length(), "");
	body.append(newLine);
}

