#include "../include/Client.hpp"

Client::Client()
{
}

Client::~Client()
{
}

void Client::setResponse(HttpResponse *response)
{
	this->_response = response;
}

void Client::setRequest(HttpRequest *request)
{
	this->_request = request;
}

HttpResponse *Client::getResponse()
{
	return this->_response;
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
