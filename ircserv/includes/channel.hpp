#ifndef CHANNEL_HPP
# define CHANNEL_HPP

#include "serv.hpp"
class User;

class Channel
{
	std::map<int, User*> participants;
	std::string name;
	std::string channelPassword;
	std::string channelTopic;
public:
	bool	i;
	bool	t;
	bool	l;
	size_t		max;
	std::map<std::string, User> bannedUsers;
	std::map<std::string, bool> operators;

	Channel(std::string n);
	Channel(Channel const &obj);
	
	std::string getChannelName();
	void renameChannel(std::string k);

	std::map<int, User*> &getParticipants();
	void	setMembers(int fd, User &user);

	void	setPassword(std::string k);
	std::string	getChannelKey();

	void	setChannelTopic(std::string t);
	std::string getChannelTopic();
	
	map<int, User*>::iterator findUserFromChannel(std::string name);

};

#endif