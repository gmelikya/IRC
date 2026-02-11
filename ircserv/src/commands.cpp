#include "../includes/serv.hpp"

void	serv::user(string b, User &user)
{
	if (!user.functionality)
	{
		size_t k = b.find_first_of(":");
		std::string rname;
		std::string s;
		std::vector<std::string> arr;
		if (k != string::npos)
		{
			rname = b.substr(k, b.size());
			s = b.substr(0, k);
			arr = splitString(s);
		}
		else
		{
			s = b;
			arr = splitString(s);
			if (!arr.empty())
				rname = arr.back();
		}
		if (arr.empty() || arr.size() < 3 || (arr.size() <= 3 && k == string::npos))
		{
			errors.sendParamsMissing(user.getUserFD(), "USER");
			return;
		}
		user.setUserName(arr[0]);
		user.setUserHost(arr[1]);
		user.setServName(arr[2]);
		user.setUserRealName(rname);
		if (!user.functionality && user.getNickName() != "" && user.isAuthenticated())
		{
			errors.RPL_REGISTER(user.getUserFD(), user.getNickName(), user.getUserName());
			user.functionality = true;
		}
	}
	else
		errors.sendAlreadyRegistered(user.getUserFD(), "");
}

void	serv::pass(string b, User &user)
{
	std::vector<string> arr = splitString(b);

	size_t i = b.find(':');
	
	if (arr.size() < 1)
	{
		errors.sendParamsMissing(user.getUserFD(), "PASS");
		return ;
	}
	if (user.isAuthenticated())
	{
		errors.sendAlreadyRegistered(user.getUserFD(), user.getNickName());
		return ;
	}
	else if (arr[0] == password)
	{
		user.changePassFlag();
		send(user.getUserFD(), "PASS :You loged in\n", 19, 0);
		if (!user.functionality && !user.getNickName().empty() && !user.getUserHost().empty())
		{
			errors.RPL_REGISTER(user.getUserFD(), user.getNickName(), user.getUserName());
			user.functionality = true;
		}
	}
	else if(i != std::string::npos && password.compare(&(arr[0].c_str())[i + 1]))
	{
		user.changePassFlag();
		send(user.getUserFD(), "PASS :You loged in\n", 19, 0);
		if (!user.functionality && !user.getNickName().empty() && !user.getUserHost().empty())
		{
			errors.RPL_REGISTER(user.getUserFD(), user.getNickName(), user.getUserName());
			user.functionality = true;
		}
	}
}


void	serv::userNick(string b, User &user)
{
	std::vector<string> arr = splitString(b);

	if (arr.empty())
	{
		errors.sendParamsMissing(user.getUserFD(), "NICK");
		return ;
	}
	if (arr[0].find_first_of(",@!:") != string::npos || arr[0].size() > MAX_LENGTH)
	{
		errors.sendInvalidNick(user.getUserFD(), user.getNickName());
		return;
	}
	for (size_t i = 0; i != arr[0].size(); i++)
	{
		if (!isprint(arr[0][i]))
		{
			errors.sendInvalidNick(user.getUserFD(), user.getNickName());
			return;	
		}
	}
	if (arr.empty() || (!arr.empty() && arr[0].empty()))
	{
		errors.sendInvalidNick(user.getUserFD(), user.getNickName());
		return;
	}
	if (!user.getNickName().empty())
	{
		if (getUserFDByNick(arr[0]) && getUserFDByNick(arr[0]) != user.getUserFD())
		{
			errors.sendNickTaken(user.getUserFD(), user.getNickName());
			return;
		}
		user.setUserNickname(arr[0]);
	}
	else
	{
		if (getUserFDByNick(arr[0]))
		{
			errors.sendNickTaken(user.getUserFD(), user.getNickName());
			return;
		}
		user.setUserNickname(arr[0]);
	}
	if(!user.functionality && !user.getUserHost().empty() && user.isAuthenticated())
	{
		errors.RPL_REGISTER(user.getUserFD(), user.getNickName(), user.getUserName());
		user.functionality = true;
	}
}

void	serv::ping(string b, User &use)
{
	if (use.isAuthenticated())
	{
		std::vector<string> arr = splitString(b);

		if (arr.empty() || (!arr.empty() && arr[0].empty()))
			errors.sendOriginMissing(use.getUserFD(), "PING");
		else
			pong(arr[0], use);
	}
}

void	serv::pong(string b, User &user)
{
	std::vector<string> arr = splitString(b);

	if (arr.empty() || (!arr.empty() && arr[0].empty()))
	{
		errors.sendOriginMissing(user.getUserFD(), "PONG");
		return;
	}
	errors.RPL_PONG(user.getUserFD(), user.getNickName());
}

void	serv::cap(string b, User &user)
{
	b = "";
	cout<< "cap called by fd "<< user.getUserFD() << endl;
}

void	serv::quit(string b, User &user)
{
	if (b.empty())
		errors.RPL_QUIT(user.getUserFD(), user.getNickName(), "");
	else
		errors.RPL_QUIT(user.getUserFD(), user.getNickName(), b);
	FD_CLR(user.getUserFD(), &def);
	removeUserFromChannels(user.getUserFD());
	cout << user.getUserFD() << endl;
	close(user.getUserFD());
	users.erase(user.getUserFD());
}

void	serv::privmsg(string b, User &user)
{
	string userNick;
	string msg;
	size_t sp = b.find(":");

	if (sp == string::npos)
	{
		errors.sendTextMissing(user.getUserFD(), "PRIVMSG");
		return ;
	}
	userNick = b.substr(0, sp);
	msg = b.substr(sp, b.size() - sp);
	msg = msg.substr(msg.find(":"), msg.size());
	std::vector<std::string> part = splitString(userNick);	
	
	if (part.empty())
	{
		errors.sendRecipientMissing(user.getUserFD(), "PRIVMSG");
		return ;
	}
	else if (part.size() != 1)
	{
		errors.sendTooManyTargets(user.getUserFD(), part[part.size() - 1]);
		return ;
	}
	else if (msg.empty())
	{
		errors.sendTextMissing(user.getUserFD(), "PRIVMSG");
		return ;
	}
	else if (user.getChannels().find(part[0]) == user.getChannels().end())
	{
		if (!getUserFDByNick(part[0]))
		{
			errors.sendNickNotFound(user.getUserFD(), part[0]);
			return ;
		}
		else
		{
			errors.RPL_PRIVMSG(getUserFDByNick(part[0]), user.getNickName(), user.getUserName(), part[0], msg);
			return;
		}
		errors.sendCannotMsgChannel(user.getUserFD(), part[0]);
		return ;
	}
	std::map<std::string, Channel*>::iterator i = user.getChannels().find(part[0]);
	std::string prefix = ":" + user.getNickName() + "!" + user.getUserName() +  "@localhost PRIVMSG ";
	for (std::map<int, User*>::iterator it = i->second->getParticipants().begin(); it != i->second->getParticipants().end(); it++)
	{
		std::string m = prefix + i->second->getChannelName() + " " + msg + "\n";
		if (it->second->getUserFD() != user.getUserFD())
			send(it->second->getUserFD(), m.c_str(), m.size() ,0);
	}
}

void	serv::join(string b, User &user)
{
	std::vector<string> arr = splitString(b);

	if (arr.empty() || (!arr.empty() && arr[0].empty()))
	{
		errors.sendParamsMissing(user.getUserFD(), "JOIN");
		return ;
	}
	if (arr[0][0] != '#')
		arr[0] = '#' + arr[0];
	map<std::string, Channel>::iterator i = all_channels.find(arr[0]);
	if (i != all_channels.end())
	{
		if (!i->second.i)
		{
			addUserToChannel(user, i->second, arr, false);
		}
		else
		{
			errors.sendChannelInviteOnly(user.getUserFD(), arr[0]);
			return ;
		}
	}
	else
	{
		Channel chan(arr[0]);
		chan.operators.insert(std::pair<std::string, bool> (user.getNickName(), true));
		addUserToChannel(user, chan, arr, true);
	}
}

void	serv::kick(std::string b, User &user)
{
	std::vector<std::string> arr = splitString(b);

	if (arr.size() < 2)
	{
		errors.sendParamsMissing(user.getUserFD(), "KICK");
		return ;
	}
	std::map<std::string, Channel*>::iterator i = user.getChannels().find(arr[0]);
	if (i != user.getChannels().end())
	{
		if (i->second->operators.find(user.getNickName()) != i->second->operators.end())
		{
			std::map<int, User*>::iterator it = i->second->getParticipants().find(getUserFDByNick(arr[1]));
			if (it == i->second->getParticipants().end())
			{
				errors.sendNickNotFound(user.getUserFD(), arr[1]);
				return ;
			}
			User* kickedUser = it->second;

			errors.RPL_KICK(it->first, user.getNickName(), user.getUserName(), it->second->getNickName(), i->first);
			std::string prefix = ":" + user.getNickName() + "!" + user.getUserName() + "@localhost KICK ";
			std::string msg = i->first + " " + it->second->getNickName() + "\n";
			sendToAllUsers(i->second->getParticipants(), prefix, msg);

			kickedUser->getChannels().erase(i->first);
			i->second->getParticipants().erase(it);

			if (i->second->getParticipants().empty())
				all_channels.erase(i->second->getChannelName());
		}
		else
		{
			errors.sendOpPrivilegesNeeded(user.getUserFD(), arr[0]);
			return ;
		}
	}
	else
	{
		errors.sendChannelNotFound(user.getUserFD(), arr[0]);
		return ;
	}
}

void	serv::invite(std::string b, User &user)
{
	std::vector<std::string> arr = splitString(b);
	if (arr.size() < 2)
	{
		errors.sendParamsMissing(user.getUserFD(), "INVITE");
		return ;
	}
	std::map<std::string, Channel*>::iterator ch = user.getChannels().find(arr[1]);
	if (ch == user.getChannels().end())
	{
		errors.sendNotOnChannel(user.getUserFD(), arr[0]);
		return ;
	}

	if (ch->second->operators.find(user.getNickName()) == ch->second->operators.end())
	{
		errors.sendOpPrivilegesNeeded(user.getUserFD(), arr[1]);
		return ;
	}

	if (!getUserFDByNick(arr[0]))
	{
		errors.sendNickNotFound(user.getUserFD(), arr[0]);
		return ;
	}
	std::map<int, User>::iterator it = users.find(getUserFDByNick(arr[0]));
	if (it == users.end())
	{
		errors.sendUserOnChannel(user.getUserFD(), arr[1], arr[0]);
		return ;
	}
	cout << it->first << endl;

	cout << it->second.getUserFD() << endl;
	ch->second->setMembers(getUserFDByNick(arr[0]), users.find(getUserFDByNick(arr[0]))->second);
	it->second.addChannel(ch->second->getChannelName(), *(ch->second));
	errors.RPL_INVITING(user.getUserFD(), ch->second->getChannelName(), user.getNickName(), user.getUserName(), arr[0]);

}

void	serv::channelTopic(std::string b, User &user)
{
	std::vector<std::string> arr = splitString(b);
	if (arr.size() == 0)
	{
		errors.sendParamsMissing(user.getUserFD(), "JOIN");
		return ;
	}
	std::map<std::string, Channel*>::iterator it = user.getChannels().find(arr[0]);
	if (it == user.getChannels().end())
	{
		errors.sendNotOnChannel(user.getUserFD(), arr[0]);
		return ;
	}
	if (arr.size() == 1)
	{
		if (it->second->getChannelTopic().empty())
		{
			errors.RPL_TOPIC(user.getUserFD(),it->second->getChannelName(), true, "");
			return ;
		}
		errors.RPL_TOPIC(user.getUserFD(),it->second->getChannelName(), false, it->second->getChannelTopic());
	}
	else
	{
		size_t d = b.find(':');
		std::string t;
		if (d != std::string::npos)
			t = b.substr(b.find(':') + 1);
		else
			t = arr[1];
		if (it->second->t && it->second->operators.find(user.getNickName()) != it->second->operators.end())
			it->second->setChannelTopic(t);
		else if (!it->second->t)
			it->second->setChannelTopic(t);
		else
		{
			errors.sendOpPrivilegesNeeded(user.getUserFD(), arr[1]);
			return ;
		}
	}
}

void	serv::mode(string b, User &user)
{
	std::vector<std::string> arr = splitString(b);
	if (arr.size() < 1)
	{
		errors.sendParamsMissing(user.getUserFD(), "MODE");
		return ;
	}
	std::map<std::string, Channel*>::iterator it = user.getChannels().find(arr[0]);
	if (it == user.getChannels().end())
	{
		errors.sendNotOnChannel(user.getUserFD(), arr[0]);
		return ;
	}
	if (arr.size() == 1)
		notifyMode(user, *(it->second));
	else
	{
		if ((it->second->operators.find(user.getNickName())) == it->second->operators.end() && arr[1][0] != 'b')
		{
			errors.sendOpPrivilegesNeeded(user.getUserFD(), arr[0]);
			return ;
		}
		if ((arr[1].size() == 2 && (arr[1][0] != '-' && arr[1][0] != '+') && arr[1][1] != 'i'
			&& arr[1][1] != 't' && arr[1][1] != 'k' && arr[1][1] != 'o' && arr[1][1] != 'l')
			|| (arr[1].size() == 1 && arr[1][0] != 'b' && arr[1][0] != 'i' && arr[1][0] != 't'
			&& arr[1][0] != 'k' && arr[1][0] != 'o' && arr[1][0] != 'l'))
		{
			errors.sendModeUnknown(user.getUserFD(), arr[1]);
			return ;
		}
		int i = -1;
		array<string, 5> mode = {"i", "t", "k", "o", "l"};
		if (arr[1].size() == 1)
			while (++i != 5 && mode[i][0] != arr[1][0]);
		else if (arr[1].size() > 1)
			while (++i != 5 && mode[i][0] != arr[1][1]);
		switch (i)
		{
		case 0:
			if (arr[1][0] == '+' || arr[1][0] == 'i')
			{
				it->second->i = true;
			}
			else if (arr[1][0] == '-')
				it->second->i = false;
			break;
		case 1:
			if (arr[1][0] == '+' || arr[1][0] == 't')
				it->second->t = true;
			else if (arr[1][0] == '-')
				it->second->t = false;
			break;
		case 2:
			if (arr[1][0] == '+' || arr[1][0] == 'k')
			{
				if (arr.size() >= 3)
						it->second->setPassword(arr[2]);
				else
				{
					errors.sendParamsMissing(user.getUserFD(), "MODE");
					return ;
				}
			}
			else if (arr[1][0] == '-')
				it->second->setPassword("");
			break;
		case 3:
			if (arr.size() >= 3)
			{
				if (arr[1][0] == '+' || arr[1][0] == 'o')
				{
					if (it->second->findUserFromChannel(arr[2]) != it->second->getParticipants().end())
					{
						it->second->operators[arr[2]] = true;
					}
					else
					{
						errors.sendNotOnChannel(user.getUserFD(), arr[0]);
						return ;
					}
				}
				else if (arr[1][0] == '-')
				{
					if (it->second->findUserFromChannel(arr[2]) != it->second->getParticipants().end())
						it->second->operators.erase(arr[2]);
					else
					{
						errors.sendNotOnChannel(user.getUserFD(), arr[0]);
						return ;
					}
				}
			}
			else
			{
				errors.sendParamsMissing(user.getUserFD(), "MODE");
				return ;
			}
			break;
		case 4:
			if (arr[1][0] == '+' || arr[1][0] == 'l')
			{
				if (arr.size() >= 3)
				{
					if (isNumber(arr[2]))
					{
						it->second->l = true;
						int i = std::atoi(arr[2].c_str());
						if (i >= 0 && i < 10)
							it->second->max = std::atoi(arr[2].c_str());
						else
						{
							send(user.getUserFD(), ":MODE@localhost Invalid limit\n", 31, 0);
							return ;
						}
					}
				}
				else
				{
					errors.sendParamsMissing(user.getUserFD(), "MODE");
					return ;
				}
			}
			else if (arr[1][0] == '-')
			{
				it->second->l = false;
				it->second->max = 0;
			}
			break;
		}
	}
}


void	serv::notice(std::string b, User &user)
{
	string userNick;
	string msg;
	size_t sp = b.find(":");

	if (sp == string::npos)
		return ;
	userNick = b.substr(0, sp);
	msg = b.substr(sp, b.size());
	msg = msg.substr(msg.find(":"), msg.size());
	std::vector<std::string> part = splitString(userNick);
	if (part.empty() || part.size() != 1 || msg.empty())
		return ;
	else if (user.getChannels().find(part[0]) == user.getChannels().end())
	{
		if (!getUserFDByNick(part[0]))
			return ;
		else
		{
			errors.RPL_PRIVMSG(getUserFDByNick(part[0]), user.getNickName(), user.getUserName(), part[0], msg);
			return;
		}
		return ;
	}
	std::map<std::string, Channel*>::iterator i = user.getChannels().find(part[0]);
	std::string prefix = ":" + user.getNickName() + "!" + user.getUserName() +  "@localhost NOTICE ";
	for (std::map<int, User*>::iterator it = i->second->getParticipants().begin(); it != i->second->getParticipants().end(); it++)
	{
		std::string m = prefix + it->second->getNickName() + " " + msg;
		if (it->second->getUserFD() != user.getUserFD())
			send(it->second->getUserFD(), m.c_str(), m.size() ,0);
	}
}

void	serv::who(string b, User &user)
{
	std::vector<std::string> arr = splitString(b);
	if (!arr.empty())
	{
		std::map<std::string, Channel>::iterator c = all_channels.find(arr[0]);
		if (c != all_channels.end())
		{
			std::map<int, User*>::iterator i = c->second.getParticipants().begin();
			for (; i != c->second.getParticipants().end(); i++)
				errors.RPL_WHOREPLY(user.getUserFD(), user.getNickName(), c->second.getChannelName(), i->second->getNickName(), user.getUserName());
			errors.RPL_ENDOFWHO(user.getUserFD(), user.getNickName(), c->second.getChannelName());
		}
		else
		{
			errors.sendChannelNotFound(user.getUserFD(), arr[0]);
			return ;
		}
	}
}

void	serv::part(string b, User &user)
{
	std::vector<std::string> arr = splitString(b);

	if (arr.size() < 1)
	{
		errors.sendParamsMissing(user.getUserFD(), "PART");
		return ;
	}
	std::map<std::string, Channel>::iterator i = all_channels.find(arr[0]);
	if (i == all_channels.end())
	{
		errors.sendChannelNotFound(user.getUserFD(), arr[0]);
		return ;
	}
	std::map<int, User*>::iterator j = (i->second.getParticipants()).find(user.getUserFD());
	if (j == (i->second.getParticipants()).end())
	{
		errors.sendNotOnChannel(user.getUserFD(), arr[0]);
		return ;
	}
	i->second.getParticipants().erase(j->first);
	j = i->second.getParticipants().begin();
	for (; j != i->second.getParticipants().end(); j++)
		errors.RPL_PART(j->second->getUserFD(), user.getNickName(), user.getUserName(), arr[0]);
	errors.RPL_PART(user.getUserFD(), user.getNickName(), user.getUserName(), arr[0]);
	std::map<std::string, bool>::iterator o = i->second.operators.find(user.getNickName());
	if (o != i->second.operators.end())
		i->second.operators.erase(user.getNickName());
}
