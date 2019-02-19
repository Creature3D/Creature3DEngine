/* Creature3D - Online Game Engine, Copyright (C) 2005 Wucaihua(26756325@qq.com)
*
* This library is free software; you can redistribute it and/or
* modify it under the terms of the GNU Lesser General Public
* License as published by the Free Software Foundation; either
* version 2.1 of the License, or (at your option) any later version.
*
* This library is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
* Lesser General Public License for more details.
*/
#ifndef CRNETAPP_CRRABBITMQ_H
#define CRNETAPP_CRRABBITMQ_H 1

#include <CRNetApp/appExport.h>
#include <CRCore/ref_ptr.h>
#include <CRCore/Referenced.h>
#include "amqp_tcp_socket.h"
#include <vector>
#include <list>
#include <string>
#include <map>
namespace CRNetApp {

class CRNETAPP_EXPORT crRabbitmq : public CRCore::Referenced
{
public:
	crRabbitmq();
	static crRabbitmq *getInstance();
	virtual void clear();

	int connect(const std::string &strHostname, int iPort, const std::string &strUser, const std::string &strPasswd);
	int disconnect();
	/**
	*   @brief       ExchangeDeclare    声明exchange
	*	@param       [in]               strExchange
	*   @param       [in]               strType
	*   @return 等于0值代表成功创建exchange，小于0代表错误
	*/
	int exchangeDeclare(const std::string &strExchange, const std::string &strType);

	/**
	*   @brief       QueueDeclare                     声明消息队列
	*	@param       [in]               strQueueName  消息队列实例
	*   @param
	*   @return 等于0值代表成功创建queue，小于0代表错误
	*/
	int queueDeclare(const std::string &strQueueName);

	/**
	*   @brief       QueueBind                        将队列，交换机和绑定规则绑定起来形成一个路由表
	*	@param       [in]               strQueueName  消息队列
	*	@param       [in]               strExchange   交换机名称
	*	@param       [in]               strBindKey    路由名称  “msg.#” “msg.weather.**”
	*   @return 等于0值代表成功绑定，小于0代表错误
	*/
	int queueBind(const std::string &strQueueName, const std::string &strExchange, const std::string &strBindKey);

	/**
	*   @brief       QueueUnbind                      将队列，交换机和绑定规则绑定解除
	*	@param       [in]               strQueueName  消息队列
	*	@param       [in]               strExchange   交换机名称
	*	@param       [in]               strBindKey    路由名称  “msg.#” “msg.weather.**”
	*   @return 等于0值代表成功绑定，小于0代表错误
	*/
	int queueUnbind(const std::string &strQueueName, const std::string &strExchange, const std::string &strBindKey);

	/**
	*   @brief       QueueDelete                      删除消息队列。
	*	@param       [in]               strQueueName  消息队列名称
	*	@param       [in]               iIfUnused     消息队列是否在用，1 则论是否在用都删除
	*   @return 等于0值代表成功删除queue，小于0代表错误
	*/
	int queueDelete(const std::string &strQueueName, int iIfUnused);

	/**
	* @brief Publish  发布消息
	* @param [in] strMessage        消息实体
	* @param [in] strExchange       交换器
	* @param [in] strRoutekey       路由规则
	*   1.Direct Exchange – 处理路由键。需要将一个队列绑定到交换机上，要求该消息与一个特定的路由键完全匹配。
	*   2.Fanout Exchange – 不处理路由键。将队列绑定到交换机上。一个发送到交换机的消息都会被转发到与该交换机绑定的所有队列上。
	*   3.Topic Exchange – 将路由键和某模式进行匹配。此时队列需要绑定要一个模式上。符号“#”匹配一个或多个词，符号“*”匹配不多不少一个词。
	*      因此“audit.#”能够匹配到“audit.irs.corporate”，但是“audit.*” 只会匹配到“audit.irs”
	* @return 等于0值代表成功发送消息实体，小于0代表发送错误
	*/
	int publish(const std::string &strMessage, const std::string &strExchange, const std::string &strRoutekey);

	/**
	* @brief consumer  消费消息
	* @param [in]  strQueueName         队列名称
	* @param [out] message				获取的消息实体
	* @param [int] timeout              取得的消息是延迟，若为NULL，表示持续取，无延迟，阻塞状态
	* @return 等于0值代表成功，小于0代表错误，错误信息从ErrorReturn返回
	*/
	int consumer(const std::string &strQueueName, std::string &message, struct timeval *timeout = NULL);


protected:
	virtual ~crRabbitmq();
	static CRCore::ref_ptr<crRabbitmq> m_instance;

	int errorMsg(amqp_rpc_reply_t x, char const *context);

	std::string                 m_strHostname;      // amqp主机
	int                         m_iPort;            // amqp端口
	std::string					m_strUser;
	std::string					m_strPasswd;
	int                         m_iChannel;

	amqp_socket_t               *m_pSock;
	amqp_connection_state_t     m_pConn;
};

}
#endif