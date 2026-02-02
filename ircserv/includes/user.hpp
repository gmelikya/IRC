#ifndef USER_HPP
# define USER_HPP

#include "channel.hpp"
class Channel;


class User
{
	int			fd;
	bool 		pass;
	std::string hostName;
	std::string uName;
	std::string realName;
	std::string servName;
	std::string userNickName;

	map<std::string, Channel*> channels;
	// map<std::string, Channel&> channels;
public:
	bool		functionality;
	User(int f);
	User(User const &obj);
	void	setUserHost(std::string &n);
	void	setUserNickname(std::string &n);
	void	setUserName(std::string &n);	
	void	setUserRealName(std::string &n);
	void	setServName(std::string &n);
	void	setUserFD(int f);
	void	addChannel(std::string n, Channel &chan);

	void	changePassFlag();

	std::string &getUserHost();
	std::string &getUserName();
	std::string &getUserRealName();
	std::string &getUserServer();
	std::string getNickName();
	bool 		isAuthenticated();
	int 		&getUserFD();
	map<std::string, Channel*> &getChannels();
};

#endif
