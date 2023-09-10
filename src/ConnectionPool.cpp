#include "connectionPoll.h"
#include "public.h"
#include <string>


//单例模式获取链接
ConnectionPool* ConnectionPool::getConnectionPool() {
	//���õ�����ģʽ  ���ڴ������ӳض���
	static ConnectionPool pool;   
	return &pool;
}

//从配置文件中加载配置项目
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
		if (idx == -1) 
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


//create connectpool
ConnectionPool::ConnectionPool() {
	if (!loadConfigFile())
	{
		return;
	}
	//create init connect
	for (int i = 0; i < m_init_size; i++)
	{
		//Connection* p = nullptr;
		Connection* p = new Connection;
		p->connect("localhost", m_port, "yd", "123456", "t1");  //here should be non-const 
		p->refreshAliveTime();    
		m_connectionQueue.push(p);  
		m_connectionCnt++;    
	}
	//create a productor for connection link
	thread produce(std::bind(&ConnectionPool::produceConnTask,this));
	produce.detach();  

	//create a scanner for outtime link
	thread scanner(std::bind(&ConnectionPool::scannerConnTask, this));  
	scanner.detach();
	
}

//productor for conntask
void ConnectionPool::produceConnTask() {
	for (;;) {
		unique_lock<mutex> lock(m_queueMutex);
		while (!m_connectionQueue.empty())
		{ 
			cv.wait(lock); 
		}

		if (m_connectionCnt < m_max_size)
		{
			Connection* p = new Connection;
			p->connect(m_ip, m_port, m_username, m_passwd, m_dbname);  
			p->refreshAliveTime();   
			m_connectionQueue.push(p);  
			m_connectionCnt++;   
		}
		cv.notify_all();
	}
}

shared_ptr<Connection> ConnectionPool::getConnection() {
	unique_lock<mutex> lock(m_queueMutex);
	
	if (m_connectionQueue.empty()) {
		cv.wait_for(lock, chrono::milliseconds(m_connectionTimeout));
		if (m_connectionQueue.empty()) {
			LOG("the connectionQue is empty and get connection falied!");
			return nullptr;
		}
	}

	/*
	here we set the allocator which will add the connection who finished task to queue
	*/
	//����һ������ָ��
	shared_ptr<Connection> sp(m_connectionQueue.front(),
		[&](Connection* pcon) {
			unique_lock<mutex> lock(m_queueMutex);
			pcon->refreshAliveTime();
			m_connectionQueue.push(pcon); 
		});
	m_connectionQueue.pop();  
	cv.notify_all();  

	return sp;
}

void ConnectionPool::scannerConnTask() {
	while (1)
	{
		// by sleep simulate definite-time
		this_thread::sleep_for(chrono::seconds(m_max_IdleTime));
		unique_lock<mutex> lock(m_queueMutex);
		while (m_connectionCnt > m_init_size)
		{
			Connection* p = m_connectionQueue.front();
			if (p->getAlivetime() >= (m_connectionTimeout * 1000)) {
				
				m_connectionQueue.pop();
				m_connectionCnt--;
				delete p; 
			}
			else {
				break;  
			}
		}
	}
}