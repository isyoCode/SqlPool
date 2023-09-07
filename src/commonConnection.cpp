
#include "commonConnection.h"
using namespace std;
#include "public.h"

// ��ʼ�����ݿ�����
Connection::Connection()
{
	m_conn = mysql_init(nullptr);
}
// �ͷ����ݿ�������Դ
Connection::~Connection() {
	if (m_conn)
	{
		mysql_close(m_conn);
	}
}

//�������ݿ�
bool Connection::connect(string ip,unsigned short port,string user,string passwd,string dbname) {
	//����һ��������
	MYSQL* p = mysql_real_connect(m_conn, ip.c_str(), user.c_str(), passwd.c_str(), dbname.c_str(), port, nullptr, 0);
	return p != nullptr ? true : false;
}

//���²��� insert delete update 
bool Connection::update(string sql) {
	//ִ��sql���
	if (mysql_query(m_conn,sql.c_str()))
	{
		LOG("����ʧ��:" + sql);
		return false;
	}
	return true;
}

//��ѯ����
MYSQL_RES* Connection::query(string sql) {
	if (mysql_query(m_conn,sql.c_str()))
	{
		LOG("��ѯʧ��:" + sql);
		return nullptr;
	}
	return mysql_use_result(m_conn);
}