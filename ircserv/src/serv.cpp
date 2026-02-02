#include "../includes/serv.hpp"

serv::serv()
{
	command["USER"] = &serv::user;
	command["PASS"] = &serv::pass;
	command["NICK"] = &serv::userNick;
	command["PING"] = &serv::ping;
	command["PONG"] = &serv::pong;
	command["QUIT"] = &serv::quit;
	command["PRIVMSG"] = &serv::privmsg;
	command["CAP"] = &serv::cap;
	command["NOTICE"] = &serv::notice;
	command["WHO"] = &serv::who;
	
	command["PART"] = &serv::part;
	command["JOIN"] = &serv::join;
	command["KICK"] = &serv::kick;
	command["INVITE"] = &serv::invite;
	command["TOPIC"] = &serv::channelTopic;
	command["MODE"] = &serv::mode;
}

void	serv::addNewClient()
{
	User	new_user(0);

	new_user.setUserFD(accept(serv_fd, (sockaddr*)(&addr), (socklen_t *)(&addr_len)));
	if (new_user.getUserFD() < 0)
	{
		perror("accept");
		return ;
	}
	fcntl(new_user.getUserFD(), F_SETFL, O_NONBLOCK);
	FD_SET(new_user.getUserFD(), &def);
	fd.push_back(new_user.getUserFD());
	users.insert(pair<int, User> (new_user.getUserFD(), new_user));
	std::cout << "New client added" << std::endl;	
}

bool	serv::handleReadWrite(int fd)
{
	string e;
	string str;
	char buf[1025] = {0};
	int len = 0;

	if ((len = recv(fd, buf, 1025, MSG_DONTWAIT)) == 0)
    {
		if(fd != serv_fd)
		{
			cout << "Host disconnected, ip " << inet_ntoa(addr.sin_addr) << ", port " << port << endl;
	    	FD_CLR(fd, &def);
			removeUserFromChannels(fd);
			users.erase(fd);
			if (fd != -1)
				close(fd);
		}
		return true;
    }
	else if (len == -1)
		return false;
	buf[len] = '\0';
	std::vector<std::string> lines = parsing(buf, len);
	size_t i = 0;
	while(!lines.empty() && i != lines.size())
	{
		cout << "line = [" << lines[i] << "]" <<endl; 
		stringstream ss(lines[i]);
		ss >> e;
		map<string, void(serv::*)(string, User&)>::iterator it = command.find(e);
		cout << "client [" << fd << "] = "  << e << std::endl;
		if(it == command.end())
			errors.sendCmdNotFound(fd, e);
		else
		{
			if (users.find(fd)->second.isAuthenticated() || (!users.find(fd)->second.isAuthenticated() && it->first == "PASS"))
			{
				str = lines[i];
				str = str.substr(e.size(), str.size() - e.size());
				if (users.find(fd)->second.functionality || (!users.find(fd)->second.functionality 
					&& ((it->first == "CAP") || (it->first == "USER")
					|| (it->first == "NICK") || (it->first == "PASS") || (it->first == "PING"))))
				{
					(this->*(it->second))(str, users.find(fd)->second);
				}
				else
					errors.sendUserNotRegistered(users.find(fd)->first, e);
			}
			else
				errors.sendUserNotRegistered(users.find(fd)->first, e);
		}
		i++;
	}
	return (false);
}

bool	serv::runServer()
{
	int opt = 1;
	serv_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (serv_fd < 0)
	{
		perror("faild socket!");
		return true;
	}
	if (setsockopt(serv_fd, SOL_SOCKET, SO_REUSEADDR, (char *)&opt, sizeof(opt)) < 0)
	{
		perror("setsockopt");
		return (true);
	}

	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = INADDR_ANY;
	addr.sin_port = htons(port);
	if (bind(serv_fd,(const sockaddr *)&addr, sizeof(addr)) < 0)
	{
		perror("bind");
		return true;
	}
	if (listen(serv_fd, 100) < 0)
	{
		perror("listen");
		return true;
	}
	addr_len = sizeof(addr);
	int select_flag;
	fd_set	readFD;

	FD_ZERO(&def);
	readFD = def;
	cout << "ServerFD = "<< serv_fd << endl;
	fcntl(serv_fd, F_SETFL, O_NONBLOCK);
	FD_SET(serv_fd, &def);
	while (true)
	{
		readFD = def;
		select_flag = select(getMaxFD() + 1, &readFD, NULL, NULL, NULL);
		if (select_flag <= 0)
		{
			std::cout << "select :Something went wrong" << std::endl;
			continue;
		}
		for (int fd = 2; fd <= getMaxFD(); fd++)
        {
            if (FD_ISSET(fd, &readFD))
            {
                if (fd == serv_fd)
                {
                    addNewClient();
                }
                else
				{
					if (handleReadWrite(fd))
					{
						break ;
					}
				}
			}
        }
	}
	return (false);
}