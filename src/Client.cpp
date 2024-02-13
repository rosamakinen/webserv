#include "../include/Client.hpp"

Client::Client() : _response(nullptr), _request(nullptr), _status(INCOMING)
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

std::chrono::high_resolution_clock::time_point Client::getRequestStart()
{
	return this->_requestStart;
}
