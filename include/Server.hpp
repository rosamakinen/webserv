#pragma once

#include "../include/WebServer.hpp"


class Server
{
	private:
		std::string			_name;
		size_t				_listenPort;
		std::string			_hostIp;
		size_t				_clientMaxBodySize;
		locationMap			_locations;

	public:
		Server();
		Server(std::string serverName,
			size_t listenPort,
			std::string hostIp,
			std::string clientMaxBodySize);
		~Server();

		bool								isKeyInLocation(std::string locationBlock, std::string key) const;
		bool								isLocationInServer(std::string locationBlock) const;
		bool								isValueForKey(std::string locationBlock, std::string key, std::string value) const;

		//Setters
		void								setName(std::string serverName);
		void								setListenPort(size_t listenPort);
		void								setHostIp(std::string hostIp);
		void								setClientMaxBodySize(size_t clientMaxBodySize);
		void								setLocation(std::string key, vectorMap locationValues);
		void								addToVectorMap(vectorMap &vMap, std::string values);

		//Getters
		std::string							getName() const;
		size_t								getListenPort() const;
		std::string							getHostIp() const;
		size_t								getClientMaxBodySize() const;
		size_t								getLocationCount() const;
		size_t								getLocationCount(std::string location) const;
		const std::vector<std::string>*		getLocationValue(std::string location, std::string key) const;

};
