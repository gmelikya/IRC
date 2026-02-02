#ifndef SERV_HPP
# define SERV_HPP

#include <fcntl.h>
#include <map>
#include "data.hpp"
#include "user.hpp"

class User;
class Channel;


class serv : public Data
{
	map<int, User> users;
	map<string, void(serv::*)(string, User&)> command;
	std::vector<int> fd;
    
	bool	isSocketValid();
	bool	isNumber(std::string n);
	int		getMaxFD();
	bool	checkCommand(char *buf);
	bool	handleReadWrite(int fd);
	void	removeUserFromChannels(int fd);
	int		getUserFDByNick(string userNick);
	void	addUserToChannel(User &user, Channel &chan, std::vector<string> arr, bool flag);
	bool	isValidChannelNameAndKey(std::vector<std::string> arr);
	bool	joinWithTwoArgs(User &user, Channel &chan, std::vector<string> arr, bool flag);
	bool	joinWithOneArgs(User &user, Channel &chan, std::vector<string> arr, bool flag);
	void	notifyJoin(Channel &chan, User &user);
	void	removeExtraSpaces(std::string &s);
	void	notifyMode(User &user, Channel &chan);
	void	sendToAllUsers(std::map<int, User*> use, std::string command, std::string msg);
	std::vector<std::string> parsing(char *buf, size_t len);
	std::vector<std::string> parsing(std::string buf);

	map<std::string, Channel*>::iterator findChannelsFromUsers(std::string name);
public:
	serv();


	std::map<std::string, Channel> all_channels;
////////////////////---COMMANDS---//////////////////////
	void	user(string b, User &user);
	void	pass(string b, User &user);
	void	userNick(string b, User &user);
	void	ping(string b, User &user);
	void	pong(string b, User &user);
	void	quit(string b, User &user);
	void	privmsg(string b, User &user);
	void	notice(string b, User &new_user);
	
	void	cap(string b, User &user);
	void	who(string b, User &user);
	void	part(string b, User &user);
	void	join(string b, User &user);
	void	kick(string b, User &user);
	void	invite(string b, User &user);
	void	channelTopic(string b, User &user);
	void	mode(string b, User &user);
//////////////////////////////////////////////////////
///////////////////////HELP///////////////////////////
	bool	runServer();
	void	addNewClient();

};

#endif
