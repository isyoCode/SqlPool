#ifndef CONNECTIONPOOL_H
#define CONNECTIONPOOL_H



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
	static ConnectionPool* getConnectionPool();   
	 
	shared_ptr<Connection> getConnection();

private:
	ConnectionPool(); 
	
	bool loadConfigFile();  
	
	void produceConnTask();
	
	void scannerConnTask();
	


	string m_ip;  
	unsigned short m_port;	
	string m_username;  
	string m_passwd;	
	string m_dbname;  

	int m_init_size;  
	int m_max_size;    
	int m_max_IdleTime; 
	int m_connectionTimeout; 

	// create connection_pool 
	queue<Connection*> m_connectionQueue; 
	mutex m_queueMutex;  // the mutex
	
	atomic_int m_connectionCnt;   // count for connection 
	condition_variable cv;  

};

#endif