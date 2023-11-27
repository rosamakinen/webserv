#pragma once

#include <iostream>
#include <map>
#include <vector>

class Server
{
	private:
		struct location
		{
			std::string				 root;
			std::vector<std::string> methods;
			std::map<std::string, std::string>	directory_pairs;
		};

		const std::string	ip;
		const unsigned int	port;
		std::string		server_name;
		unsigned int	request_max_body_size;

		std::vector<location> locations;

		Server(void);

		void	copyLocations(const std::vector<location> locations);
		void	copyLocationMethods(const std::vector<std::string> methods, Server::location *location);
	public:
		Server(const std::string& ip, const unsigned int& port);
		~Server(void);
		Server(const Server& rhs);

		Server& operator=(const Server& rhs);

		void	setServerName(const std::string server_name);
		void	setRequestMaxBodySize(const unsigned int request_max_body_size);
		void	addLocation(const std::string root, const std::vector<std::string> methods);
		void	addDirectoryPair(const std::string key, const std::string value, struct location *location);
};
