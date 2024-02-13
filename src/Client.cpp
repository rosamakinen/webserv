#include "../include/Client.hpp"
#include "Client.hpp"

Client::Client() : _response(nullptr), _request(nullptr), _server(nullptr), _status(NONE)
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
	this->_request = request;
	this->setStatus(Client::STATUS::INCOMING);
}

void Client::appendRequest(std::string requestToAdd)
{
	this->_request->appendBody(requestToAdd);
	this->setStatus(Client::STATUS::INCOMING);
}

void Client::setServer(Server *server)
{
	this->_server = server;
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

Server *Client::getServer()
{
	return this->_server;
}

std::chrono::high_resolution_clock::time_point Client::getRequestStart()
{
	return this->_requestStart;
}
