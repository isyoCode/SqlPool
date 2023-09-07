#ifndef CONNECTIONPOOL_H
#define CONNECTIONPOOL_H

//ʵ�����ӳع���ģ��

#include <mutex>
#include <iostream>
#include <string>
#include <queue>
#include <atomic>
#include <thread>
#include <condition_variable>
#include <memory>
#include <functional>
#include "commonConnection.h"
using namespace std;


class ConnectionPool
{
public:
	static ConnectionPool* getConnectionPool();   //��ȡ���ӳض���ʵ����Ψһ�ӿ�
	 
	//���ⲿ�ṩ�ӿ� �����ӳ��л�ȡһ�����õĿ�������
	//ʹ������ָ���Զ������ⲿʹ�õ��ͷŹ���
	shared_ptr<Connection> getConnection();

private:
	ConnectionPool(); // #1���캯��˽�л�
	
	bool loadConfigFile();  //�������ļ��м���������

	//�����ڶ������߳��� ר�Ÿ�������������
	void produceConnTask();
	
	//ɨ�賬��maxIdleTimeʱ��Ŀ������ӣ����ж�Ӧ�����ӻ���
	void scannerConnTask();
	



	//���ӳس�Ա����
	string m_ip;  //���ݿ�ip��ַ
	unsigned short m_port;	//���ݿ�˿ں�   Ĭ��Ϊ3306
	string m_username;  //���ݿ��¼�û���
	string m_passwd;	//���ݿ��¼����
	string m_dbname;  //���ӵ����ݿ�����

	int m_init_size;  //���ӳس�ʼ������
	int m_max_size;    //���ӳ��������
	int m_max_IdleTime; //������ʱ��
	int m_connectionTimeout; //mysql��ȡ�������ʱʱ��





	//�������ӳ�
	queue<Connection*> m_connectionQueue; //�洢mysql���Ӷ���
	mutex m_queueMutex;  //ά�����Ӷ��е��̰߳�ȫ������
	
	atomic_int m_connectionCnt;   //��¼������������connection���ӵ�������
	condition_variable cv;  //������������,�������������̺߳����������߳�ͨ��

};

#endif