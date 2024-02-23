#include "../include/Client.hpp"

Client::Client() : _response(nullptr), _request(nullptr), _server(nullptr), _status(NONE), _closeConnection(false)
{
	_requestStart = std::chrono::high_resolution_clock::now();
}

Client::~Client()
{
	if (_request != nullptr)
		delete _request;
	if (_response != nullptr)
		delete _response;
}

void Client::setResponse(HttpResponse *response)
{
	this->_response = response;
	this->setStatus(Client::STATUS::OUTGOING);
}

void Client::setRequest(HttpRequest *request)
{
	std::string	host = request->getHeader("Host");
	if (host.empty())
		throw BadRequestException("No host given as a header");

	std::string	host_name;
	std::string port;
	int	colon_pos = host.find_last_of(':');
	host_name = host.substr(0, colon_pos);
	try
	{
		port = std::stoi(host.substr(colon_pos + 1));
	}
	catch(const std::exception& e)
	{
		throw BadRequestException("Invalid header \"Host\" given");
	}

	std::string closeConnectionString = request->getHeader("Connection");
	if (closeConnectionString.empty() == false && closeConnectionString.compare("close") == 0)
		this->_closeConnection = true;

	this->_request = request;
	this->setStatus(Client::STATUS::INCOMING);
}

void Client::appendRequest(std::string requestToAdd)
{
	this->_request->appendBody(requestToAdd);
	this->setStatus(Client::STATUS::INCOMING);
	_requestStart = std::chrono::high_resolution_clock::now();
}

void Client::setServer(Server *server)
{
	this->_server = server;
}

HttpResponse *Client::getResponse()
{
	return this->_response;
}

bool Client::closeConnection()
{
	return this->_closeConnection;
}

void Client::setCloseConnection(bool close)
{
	this->_closeConnection = close;
}

HttpRequest *Client::getRequest()
{
	return this->_request;
}

void Client::setStatus(STATUS status)
{
	this->_status = status;
}

Client::STATUS Client::getStatus()
{
	return this->_status;
}

Server *Client::getServer()
{
	return this->_server;
}

std::chrono::high_resolution_clock::time_point Client::getRequestStart()
{
	return this->_requestStart;
}

void Client::updateStatus()
{
	if (this->_request == nullptr)
		return;

	if (this->_status == Client::STATUS::INCOMING)
	{
		if (this->_request->getMethod() != Util::METHOD::POST && this->_request->getMethod() != Util::METHOD::CGI_POST)
		{
			this->setStatus(Client::STATUS::READY_TO_HANDLE);
			return;
		}

		std::string chunkedString = this->_request->getHeader(H_ENCODING);
		if (chunkedString.empty() || chunkedString.compare("chunked") != 0)
		{
			if (this->_request->getContentLength() != this->_request->getBodyLength())
				throw BadRequestException("The request is not chunked but the body was not fully received");

			this->setStatus(Client::STATUS::READY_TO_HANDLE);
			return;
		}

		if (this->_request->getContentLength() == this->_request->getBody().length())
		{
			this->setStatus(Client::STATUS::READY_TO_HANDLE);
			return;
		}
	}
}
