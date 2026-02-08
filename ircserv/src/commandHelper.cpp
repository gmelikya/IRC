#include "../includes/serv.hpp"

void	serv::sendToAllUsers(std::map<int, User*> use, std::string command, std::string msg)
{
	for (std::map<int, User*>::iterator i = use.begin(); i != use.end(); i++)
	{
		send(i->second->getUserFD(), (command + msg).c_str(), command.size() + msg.size(), 0);
	}
}

bool	serv::isValidChannelNameAndKey(std::vector<std::string> arr)
{
	if (arr[0][0] != '#')
		return false;
	return true;
}

map<std::string, Channel*>::iterator serv::findChannelsFromUsers(std::string name)
{
	map<int, User>::iterator i = users.begin();
	map<std::string, Channel*>::iterator it;
	for (; i != users.end(); i++)
	{
		it = i->second.getChannels().find(name);
		if (it->first == name)
			return it;
	}
	it = (users.begin())->second.getChannels().end();
	return it;
}

bool	serv::joinWithTwoArgs(User &user, Channel &chan, std::vector<string> arr, bool flag)
{
	/*debag*/
	std::cout << "flag join 2 args = " << flag << std::endl;
	for (auto it = arr.begin(); it != arr.end(); ++it)
		std::cout << *it << " ";
	std::cout << std::endl;
	
	/*debag*/
	if (flag)
	{
		if (chan.operators.find(user.getNickName()) != chan.operators.end())
		{
			chan.setPassword(arr[1]);
			if (arr[0][0] != '#')
				arr[0] = '#' + arr[0];
			chan.renameChannel(arr[0]);
			notifyJoin(chan, user);
		}
		else
		{
			errors.sendOpPrivilegesNeeded(user.getUserFD(), arr[0]);
			return true;
		}
	}
	else if (chan.getChannelKey() == arr[1])
	{
		if (chan.bannedUsers.find(user.getNickName()) != chan.bannedUsers.end())
		{
			errors.sendBannedFromChannel(user.getUserFD(), arr[0]);
			return true;
		}
		else
		{
			if ((chan.l && chan.max > chan.getParticipants().size()) || !chan.l)
			{
				if (arr[0][0] == '#')
					notifyJoin(chan, user);
				else
				{
					errors.sendChannelNotFound(user.getUserFD(), arr[0]);
					return true;
				}
			}
			else
			{
				errors.sendChannelFull(user.getUserFD(), arr[0]);
				return true;
			}
		}
	}
	else
	{
		errors.sendChannelFull(user.getUserFD(), arr[0]);
		return true;
	}
	return false;
}

bool	serv::joinWithOneArgs(User &user, Channel &chan, std::vector<string> arr, bool flag)
{
	/*debag*/
	std::cout << "flag join 1 args = " << flag << std::endl;
	for (auto it = arr.begin(); it != arr.end(); ++it)
		std::cout << *it << " ";
	std::cout << std::endl; 	
	/*debag*/
	if (flag)
	{
		std::cout << "isjoining" << std::endl;
		isJoining = true;
		chan.renameChannel(arr[0]);
	}
	if (chan.bannedUsers.find(user.getNickName()) != chan.bannedUsers.end())
	{
		errors.sendBannedFromChannel(user.getUserFD(), arr[0]);
		return true;
	}
	else
	{
		if ((chan.l && chan.max > chan.getParticipants().size()) || !chan.l)
		{
			if (chan.getChannelKey().empty())
				notifyJoin(chan, user);
			else
			{
				errors.sendParamsMissing(user.getUserFD(), arr[0]);
				return true;
			}
		}
		else
		{
			errors.sendChannelFull(user.getUserFD(), arr[0]);
			return true;
		}
	}
	return (false);
}

void serv::addUserToChannel(User &user, Channel &chan, std::vector<string> arr, bool flag)
{
	if (arr.size() >= 2)
	{
		std::cout << "1" << std::endl;
		if (joinWithTwoArgs(user, chan, arr, flag))
			return ;
	}
	else if (arr.size() == 1 && chan.getChannelKey().empty())
	{
				std::cout << "2" << std::endl;

		if (joinWithOneArgs(user, chan, arr, flag))
			return ;
	}
	else
		return ;
	std::cout << "3" << std::endl;

	all_channels.insert(make_pair(chan.getChannelName(), chan));
	// all_channels.insert(make_pair<std::string, Channel>(chan.getChannelName(), chan));
	all_channels.find(chan.getChannelName())->second.setMembers(user.getUserFD(), user);
	user.addChannel(all_channels.find(chan.getChannelName())->first, all_channels.find(chan.getChannelName())->second);
	std::cout << "4" << std::endl;
	/*debug*/
	for (std::map<std::string, bool>::const_iterator i = chan.operators.begin(); i != chan.operators.end(); i++)
		std::cout << "name = " << i->first << " bool = " << i->second << std::endl;
	/*debug*/
}


bool serv::isNumber(std::string n)
{
	int i = atoi(n.c_str());
	std::stringstream ss;
    ss << i;
    std::string str = ss.str();
	if (str == n)
		return (true);
	return false;
}

void	serv::removeUserFromChannels(int fd)
{
	map<std::string, Channel>::iterator i = all_channels.begin();
	map<int, User*>::iterator u;
	for(; i != all_channels.end(); i++)
	{
		u = i->second.getParticipants().find(fd);
		if (u != i->second.getParticipants().end())
		{
			if (i->second.operators.find(users.find(fd)->second.getNickName()) != i->second.operators.end())
				i->second.operators.erase(i->second.operators.find(users.find(fd)->second.getNickName()));
			i->second.getParticipants().erase(u);
			if (i->second.getParticipants().empty())
				all_channels.erase(i);
		}
		if (all_channels.empty())
			break ;
	}
}

int	serv::getUserFDByNick (string userNick)
{
	map<int, User>::iterator e = users.end();
	for (map<int, User>::iterator i = users.begin(); i != e; i++)
	{
		if (i->second.getNickName() == userNick)
			return (i->first);
	}
	return (0);
}

void	serv::notifyJoin(Channel &chan, User &user)
{
	std::map<int, User*>::iterator i = chan.getParticipants().begin();
	std::string participants;

	string msg = ":" + user.getNickName() + "!" + user.getUserName() + "@localhost JOIN " + chan.getChannelName();
	send(user.getUserFD(), (msg + "\n").c_str(),msg.length() + 2, 0);
	sendToAllUsers(chan.getParticipants(), (":" + user.getNickName() + "!" + user.getUserName() + "@localhost JOIN "),  (chan.getChannelName() + "\n"));
	for (; i != chan.getParticipants().end(); i++)
	{
		if (chan.operators.find(i->second->getNickName()) != chan.operators.end())
		{
			participants.append("@");
			participants.append(i->second->getNickName());
		}
		else
		{
			participants.append("+");
			participants.append(i->second->getNickName());						
		}
		errors.RPL_NAMREPLY(user.getUserFD(), user.getNickName(), chan.getChannelName(), participants);
		participants = "";
	}
	if (chan.getParticipants().size() == 0)
	{
		participants.append("@");
		participants.append(user.getNickName());
	}
	else if ((chan.getParticipants()).find(user.getUserFD()) == chan.getParticipants().end())
	{
		participants.append("+");
		participants.append(user.getNickName());
	}
	errors.RPL_NAMREPLY(user.getUserFD(), user.getNickName(), chan.getChannelName(), participants);
	errors.RPL_ENDOFNAMES(user.getUserFD(), user.getNickName(), chan.getChannelName());
}

void serv::notifyMode(User &user, Channel &chan)
{
    std::string modeStr = "+";

    // Добавляем флаги, которые реально установлены на канале
    if (chan.i) modeStr += "i";          // Invite-only
    if (chan.t) modeStr += "t";          // Topic только для операторов
    if (!chan.getChannelKey().empty()) modeStr += "k"; // Канал с паролем
    if (!chan.operators.empty()) modeStr += "o";      // Есть операторы
    if (chan.l) modeStr += "l";          // Лимит участников

    // Отправляем пользователю текущие флаги MODE
    std::string msg = "324 " + user.getNickName() + " " + chan.getChannelName() + " " + modeStr + "\n";
    send(user.getUserFD(), msg.c_str(), msg.size(), 0);

    // Если установлен ключ (k) — показать его
    if (!chan.getChannelKey().empty())
    {
        msg = "324 " + user.getNickName() + " " + chan.getChannelName() + " k\n";
        send(user.getUserFD(), msg.c_str(), msg.size(), 0);
    }

    // Можно по желанию показать лимит, если установлен
    if (chan.l)
    {
        msg = "324 " + user.getNickName() + " " + chan.getChannelName() + " l " + std::to_string(chan.max) + "\n";
        send(user.getUserFD(), msg.c_str(), msg.size(), 0);
    }
}
