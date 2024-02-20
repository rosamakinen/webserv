#ifndef SERVER_HPP
#define SERVER_HPP

#include "WebServer.hpp"
#include "Socket.hpp"

#include <iterator>
#include <algorithm>

class Server
{
	private:
		bool			_isDefault;
		std::string		_name;
		size_t			_listenPort;
		std::string		_hostIp;
		size_t			_clientMaxBodySize;
		locationMap		_locations;
		Socket			*_socket;
		std::map<int, std::string> _errorpages;

	public:
		Server();
		Server(std::string serverName,
			size_t listenPort,
			std::string hostIp,
			std::string clientMaxBodySize);
		~Server();

		bool	isKeyInLocation(std::string locationBlock, std::string key) const;
		bool	isLocationInServer(std::string locationBlock) const;
		bool	isValueForKey(std::string locationBlock, std::string key, std::string value) const;

		//Setters
		void setAsDefault();
		void	setName(std::string serverName);
		void	setSocket();
		void	setListenPort(size_t listenPort);
		void	setHostIp(std::string hostIp);
		void	setClientMaxBodySize(size_t clientMaxBodySize);
		void	setLocation(std::string key, vectorMap locationValues);
		static void	addToVectorMap(vectorMap &vMap, std::string values);
		bool	addErrorPage(int status, std::string index);

		//Getters
		bool isDefault() const;
		std::string	getName() const;
		Socket	*getSocket() const;
		size_t	getListenPort() const;
		std::string	getHostIp() const;
		size_t	getClientMaxBodySize() const;
		size_t	getLocationCount() const;
		size_t	getLocationCount(std::string location) const;
		const std::vector<std::string>*	getLocationValue(std::string location, std::string key) const;
		const std::string	getErrorPagePath(int status);
};

#endif
