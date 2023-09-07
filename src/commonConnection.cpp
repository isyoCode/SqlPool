
#include "commonConnection.h"
using namespace std;
#include "public.h"

// 初始化数据库连接
Connection::Connection()
{
	m_conn = mysql_init(nullptr);
}
// 释放数据库连接资源
Connection::~Connection() {
	if (m_conn)
	{
		mysql_close(m_conn);
	}
}

//链接数据库
bool Connection::connect(string ip,unsigned short port,string user,string passwd,string dbname) {
	//创建一个连接链
	MYSQL* p = mysql_real_connect(m_conn, ip.c_str(), user.c_str(), passwd.c_str(), dbname.c_str(), port, nullptr, 0);
	return p != nullptr ? true : false;
}

//更新操作 insert delete update 
bool Connection::update(string sql) {
	//执行sql语句
	if (mysql_query(m_conn,sql.c_str()))
	{
		LOG("更新失败:" + sql);
		return false;
	}
	return true;
}

//查询操作
MYSQL_RES* Connection::query(string sql) {
	if (mysql_query(m_conn,sql.c_str()))
	{
		LOG("查询失败:" + sql);
		return nullptr;
	}
	return mysql_use_result(m_conn);
}