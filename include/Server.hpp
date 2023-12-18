#pragma once

#include "../include/WebServer.hpp"

class Server
{
	private:
		std::string			_name;
		size_t				_listenPort;
		std::string			_hostIp;
		std::string			_rootDir;
		struct sockaddr_in	_address;
		size_t				_clientMaxBodySize;
		std::string			_index;
		simpleMap			_errorPages;
		locationMap			_location;

	public:
		Server();
		Server(std::string serverName,
			size_t listenPort,
			std::string hostIp,
			std::string rootDir,
			std::string index,
			std::string clientMaxBodySize);
		~Server();

		bool								isErrorPage(std::string error) const;
		bool								isKeyInLocation(std::string locationBlock, std::string key) const;
		bool								isLocationInServer(std::string locationBlock) const;
		bool								isValueForKey(std::string locationBlock, std::string key, std::string value) const;

		//Setters
		void								setName(std::string serverName);
		void								setListenPort(size_t listenPort);
		void								setHostIp(std::string hostIp);
		void								setRoot(std::string rootDir);
		void								setIndex(std::string index);
		void								setClientMaxBodySize(size_t clientMaxBodySize);
		void								setErrorPage(std::string errorCode, std::string errorPage);
		void								setLocation(std::string key, vectorMap locationValues);
		void								setKeyValue(
												std::string location,
												std::string key,
												std::vector<std::string> values);

		//Getters
		std::string							getName() const;
		size_t								getListenPort() const;
		std::string							getHostIp() const;
		std::string							getRoot() const;
		std::string							getIndex() const;
		size_t								getClientMaxBodySize() const;
		std::string							getErrorPage(std::string error) const;
		struct sockaddr_in					getAddress() const;
		size_t								getLocationCount() const;
		size_t								getLocationCount(std::string location) const;
		const std::vector<std::string>*		getLocationValue(std::string location, std::string key) const;

};
