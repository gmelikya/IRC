#include "../includes/channel.hpp"

Channel::Channel(std::string n)
{
	name = n;
	max = 5;
	channelPassword = "";
	i = false;
	t = false;
	l = false;
}

Channel::Channel(Channel const &obj)
{
	this->name = obj.name;
	this->channelPassword = obj.channelPassword;
	this->channelTopic = obj.channelTopic;
	this->i = obj.i;
	this->t = obj.t;
	this->l = obj.l;
	this->max = obj.max;

	for (std::map<int, User*>::const_iterator i = obj.participants.begin(); i != obj.participants.end(); i++)
		this->participants.insert(make_pair(i->first, i->second));
	for (std::map<std::string, User>::const_iterator i = obj.bannedUsers.begin(); i != obj.bannedUsers.end(); i++)
		this->bannedUsers.insert(make_pair(i->first, i->second));
	for (std::map<std::string, bool>::const_iterator i = obj.operators.begin(); i != obj.operators.end(); i++)
		this->operators.insert(make_pair(i->first, i->second));
}


std::string Channel::getChannelName(){return (name);}
void	Channel::renameChannel(std::string n){name = n;}

std::map<int, User*> &Channel::getParticipants(){return (participants);}
void	Channel::setMembers(int fd, User &user){participants.insert(make_pair(fd, &user));}

void	Channel::setPassword(std::string k){channelPassword = k;}
std::string	Channel::getChannelKey(){return (channelPassword);}

void	Channel::setChannelTopic(std::string t){channelTopic = t;}
std::string	Channel::getChannelTopic(){return (channelTopic);}

map<int, User*>::iterator Channel::findUserFromChannel(std::string name)
{
	map<int, User*>::iterator i = participants.begin();
	for (; i != participants.end(); i++)
	{
		if (i->second->getNickName() == name)
			return i;
	}
	return participants.end();
}
