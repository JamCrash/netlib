#ifndef CALLBACKS_H
#define CALLBACKS_H

#include "src/basics/Timestamp.h"

#include <functional>
#include <memory>

namespace netlib
{
namespace net
{ 

class TcpConnection;
class Buffer;

typedef std::shared_ptr<TcpConnection> TcpConnectionPtr;

typedef std::function<void(const TcpConnectionPtr& conn)> ConnectionCallBack;
typedef std::function<void(const TcpConnectionPtr& conn,
                           Buffer* buf,
                           Timestamp receiveTime)> MessageCallBack;
typedef std::function<void(const TcpConnectionPtr& conn)> WriteCompleteCallBack;
typedef std::function<void(const TcpConnectionPtr& conn)> ErrorCallBack;

} // net
} // netlib

#endif  /* CALLBACKS_H */
