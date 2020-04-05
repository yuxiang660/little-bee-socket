#include "Server.h"

namespace cbee
{

void Server::setConnectedFunc(const ConnectedFunc &f)
{
   connectedFunc = f;
}

void Server::setReadDoneFunc(const ReadDoneFunc &f)
{
   readDoneFunc = f;
}

void Server::setWriteDoneFunc(const WriteDoneFunc &f)
{
   writeDoneFunc = f;
}

void Server::setDisConnectedFunc(const DisConnectedFunc &f)
{
   disConnectedFunc = f;
}

} // namespace cbee
