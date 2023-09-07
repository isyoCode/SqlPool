#include "connectionPoll.h"
#include "public.h"
#include <string>


//��ȡ���ӳض���Ψһ=ʵ��
ConnectionPool* ConnectionPool::getConnectionPool() {
	//���õ�����ģʽ  ���ڴ������ӳض���
	static ConnectionPool pool;   
	return &pool;
}

//�������ļ��м���������
bool ConnectionPool::loadConfigFile() {
	FILE* pf = fopen("./mysql.init", "r");
	if (!pf) {
		LOG("mysql.init file is not exit!");
		return false;
	}

	while (!feof(pf))
	{
		char line[1024] = { 0 };
		fgets(line, 1024, pf);
		std::string str = line;
		
		int idx = str.find('=', 0);
		if (idx == -1) //��Ч��������
		{
			continue;
		}
		int endidx = str.find('\n', idx);
		string key = str.substr(0, idx);
		string value = str.substr(idx + 1, endidx - idx - 1);

		if (key == "ip") {
			m_ip = value;
		}
		else if (key == "port") {
			m_port = atoi(value.c_str());
		}
		else if (key == "username") {
			m_username = value;
		}
		else if (key == "passwd") {
			m_passwd = value;
		}
		else if (key == "initSize") {
			m_init_size = atoi(value.c_str());
		}
		else if (key == "MaxSize") {
			m_max_size = atoi(value.c_str());
		}
		else if (key == "maxIdleTime") {
			m_max_IdleTime = atoi(value.c_str());
		}
		else if (key == "connectionTimeOut") {
			m_connectionTimeout = atoi(value.c_str());
		}
		else if (key == "dbname") {
			m_dbname = value;
		}

	}

	return true;
}


//���ӳصĹ��캯��
ConnectionPool::ConnectionPool() {
	//������������
	if (!loadConfigFile())
	{
		return;
	}
	//������ʼ����������
	for (int i = 0; i < m_init_size; i++)
	{
		//Connection* p = nullptr;
		Connection* p = new Connection;
		p->connect(m_ip, m_port, m_username, m_passwd, m_dbname);  //�Ѿ���������
		p->refreshAliveTime();    //ˢ��һ�¿�ʼ���е���ʼʱ��
		m_connectionQueue.push(p);  //������뵽���е���?
		m_connectionCnt++;    //��¼��������
	}

	//����һ���µ��߳� ��Ϊ����������
	thread produce(std::bind(&ConnectionPool::produceConnTask,this));
	produce.detach();  //�������? �൱������Ϊ�ػ��߳�

	//����һ���µĶ�ʱ�̣߳�ɨ�賬��maxIdleTime ʱ��Ŀ�������? ,���ж������ӻ���
	thread scanner(std::bind(&ConnectionPool::scannerConnTask, this));  //ʹ�ð��� ��Ϊthis��scannerConnTask����
	scanner.detach();
	
}

//�����ڶ������̵߳��� ר�Ÿ����̵߳�����
void ConnectionPool::produceConnTask() {
	for (;;) {
		unique_lock<mutex> lock(m_queueMutex);
		while (!m_connectionQueue.empty())
		{ 
			cv.wait(lock); //���в���  �˴������߳̽���ȴ�״�?
		}

		//��������û�е������� �����µ�����
		if (m_connectionCnt < m_max_size)
		{
			//�����µ�����
			Connection* p = new Connection;
			p->connect(m_ip, m_port, m_username, m_passwd, m_dbname);  //�Ѿ���������
			p->refreshAliveTime();    //ˢ��һ�¿�ʼ���е���ʼʱ��
			m_connectionQueue.push(p);  //������뵽���е���?
			m_connectionCnt++;    //��¼��������
		}

		//֪ͨ�������߳̿�����������
		cv.notify_all();
	}
}


//���ⲿ�ṩ�ӿڣ������ӳ��л�ȡһ�����õĿ�������
shared_ptr<Connection> ConnectionPool::getConnection() {
	//��ȡ����ʱ��Ҫ����
	unique_lock<mutex> lock(m_queueMutex);
	
	//�ж϶����Ƿ�Ϊ��
	if (m_connectionQueue.empty()) {
		cv.wait_for(lock, chrono::milliseconds(m_connectionTimeout));
		//���Ѻ�
		if (m_connectionQueue.empty()) {
			LOG("��ȡ�������ӳ�ʱ��.....��ȡ����ʧ��!");
			return nullptr;
		}
	}

	/*
	share_ptr ����ʱ ���connection��Դֱ��delete��  �൱�ڵ���connection���������� connection�ͱ�close
	��Ҫ�Զ���share_ptr ���ͷ���Դ��ʽ ��connectionֱ�ӹ黹��queue����
	*/
	//����һ������ָ��
	shared_ptr<Connection> sp(m_connectionQueue.front(),
		[&](Connection* pcon) {
			//��������Դ�黹���̶߳��е���
			unique_lock<mutex> lock(m_queueMutex); //�����ڷ�����Ӧ���߳��е��õ� һ��Ҫ���Ƕ��е��̰߳�ȫ
			pcon->refreshAliveTime();
			m_connectionQueue.push(pcon); 
		});
	m_connectionQueue.pop();  //�������е�ָ�뵯�� ����������һ��
	cv.notify_all();   //�����������Ժ� ֪ͨ�������߳��ж� �Ƿ����Ϊ��?  �Ͻ���������

	return sp;
}


void ConnectionPool::scannerConnTask() {
	while (1)
	{
		//ͨ��sleepģ�ⶨʱЧ��
		this_thread::sleep_for(chrono::seconds(m_max_IdleTime));

		//ɨ���������� �ͷŶ��������?
		//Ϊ���̰߳�ȫ  ������Ҫ���ӻ�����
		unique_lock<mutex> lock(m_queueMutex);
		while (m_connectionCnt > m_init_size)
		{
			Connection* p = m_connectionQueue.front();
			if (p->getAlivetime() >= (m_connectionTimeout * 1000)) {
				//�ͷŸ�����
				m_connectionQueue.pop();
				m_connectionCnt--;
				delete p;  //����~Connection�����ͷ�����
			}
			else {
				break;  //��ͷ������û�г������ռ�ʱ�� �����϶�û��
			}

		}
	}
}