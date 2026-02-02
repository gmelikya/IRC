#include "../includes/user.hpp"

User::User(int f){fd = f; pass = false; functionality = false;}

User::User(User const &obj)
{
	this->fd = obj.fd;
	this->pass = obj.pass;
	this->hostName = obj.hostName;
	this->uName = obj.uName;
	this->realName = obj.realName;
	this->servName = obj.servName;
	this->userNickName = obj.userNickName;
	this->functionality = obj.functionality;

	for (std::map<std::string, Channel*>::const_iterator i = obj.channels.begin(); i != obj.channels.end(); i++)
		this->channels.insert(make_pair(i->first, i->second));
}

void	User::setUserHost(std::string &n){hostName = n;}
void	User::setUserName(std::string &n){uName = n;}
void	User::setUserRealName(std::string &n){realName = n;}
void	User::setServName(std::string &n){servName = n;}
void	User::setUserNickname(std::string &n){userNickName = n;}
void	User::changePassFlag(){!pass ? pass = true : pass = false;}
void	User::setUserFD(int f){fd = f;}
void	User::addChannel(std::string n, Channel &chan){channels.insert(make_pair(n, &chan));}

std::string &User::getUserHost(){return(hostName);}
std::string &User::getUserName(){return(uName);}
std::string &User::getUserRealName(){return(realName);}
std::string User::getNickName(){return(userNickName);}
std::string &User::getUserServer(){return(servName);}
bool		 User::isAuthenticated(){return(pass);}
int 		&User::getUserFD(){return(fd);}
map<std::string, Channel*> &User::getChannels(){return (channels);}
