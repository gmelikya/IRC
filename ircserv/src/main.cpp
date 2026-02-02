#include "../includes/serv.hpp"

int main(int argc, char **argv, char **env)
{
	(void)env;
	if (argc == 3)
	{
		serv	data;
		data.setPassword(argv[2]);
		if(data.configurePort(argv[1]))
			return (1);
		if (data.runServer())
			return (1);
	}
	else
		std::cerr << "Wrong number of arguments (./ircserv <port> <password>)" << std::endl;
	return (0);
}