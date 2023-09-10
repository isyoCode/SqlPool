
#include "commonConnection.h"
using namespace std;
#include "public.h"


Connection::Connection()
{
	m_conn = mysql_init(nullptr);
}

Connection::~Connection() {
	if (m_conn)
	{
		mysql_close(m_conn);
	}
}

bool Connection::connect(string ip,unsigned short port,string user,string passwd,string dbname) {
	MYSQL* p = mysql_real_connect(m_conn, ip.c_str(), user.c_str(), passwd.c_str(), dbname.c_str(), port, nullptr, 0);
	if( p == nullptr) std::cout << "connect error !" <<std::endl;
	return p != nullptr ? true : false;
}

// insert delete update 
bool Connection::update(string sql) {
	//ִ��sql���
	if (mysql_query(m_conn,sql.c_str()))
	{
		LOG("update error :" + sql);
		return false;
	}
	return true;
}


MYSQL_RES* Connection::query(string sql) {
	if (mysql_query(m_conn,sql.c_str()))
	{
		std::cout << "query error !" << std::endl;
		return nullptr;
	}
	return mysql_use_result(m_conn);
}