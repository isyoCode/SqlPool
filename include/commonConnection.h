#ifndef COMMONCONNECTION_H
#define COMMONCONNECTION_H

#include <mysql.h>
#include <string>
#include <ctime>
using namespace std;
#include "public.h"
// ���ݿ������
class Connection
{
public:
	// ��ʼ�����ݿ�����
	Connection();
	
	// �ͷ����ݿ�������Դ
	~Connection();
	
	// �������ݿ�
	bool connect(string ip, 
				unsigned short port, 
				string user,
				string password,
				string dbname);
		
	// ���²��� insert��delete��update
	bool update(string sql);
	
	// ��ѯ���� select
	MYSQL_RES* query(string sql);
	

	//ˢ��һ�����ӵĴ��ʱ��
	void refreshAliveTime() {
		m_alivetime = clock();
	}
	clock_t getAlivetime() {
		return clock() - m_alivetime;
	}

private:
	MYSQL* m_conn; // ��ʾ��MySQL Server��һ������
	clock_t m_alivetime;  //�������״̬��Ĵ��ʱ��
};


#endif