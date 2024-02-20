#ifndef CONNECTION_HPP
#define CONNECTION_HPP

#include <chrono>

class Connection
{
	public:
		Connection();
		void updateTS();

		std::chrono::high_resolution_clock::time_point getTS();

	private:
		std::chrono::high_resolution_clock::time_point _lastTrafficTS;
};

#endif
