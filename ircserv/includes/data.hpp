#ifndef DATA_HPP
# define DATA_HPP

#include <map>
#include <vector>
#include <array>
#include <unistd.h>
#include <errno.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sstream>
#include "MsgErr.hpp"


using	namespace std;
class MsgErr;

#define MAX_LENGTH 20

class Data : public MsgErr
{
	bool	isNotValidPort(string &port);
public:
	Data();
	int		port;
	string	password;

	int		serv_fd;
	int		max_sd;
	fd_set	def;

	bool	isJoining;
	struct sockaddr_in addr;
	int addr_len;
	MsgErr errors;
	void	setPassword(string pass);
	bool	configurePort(string port);
	vector<std::string> splitString(std::string &str);
};


#endif
