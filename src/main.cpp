#include <iostream>
using namespace std;

#include "commonConnection.h"
#include "connectionPoll.h"

  
int main() {

	
	//cp->loadConfigFile();
	ConnectionPool* cp = ConnectionPool::getConnectionPool();
	// Connection conn;
	// char sql[1024] = { 0 };
	// sprintf(sql, "insert into user(id,name,age,sex) values(%d,'%s',%d,'%s')", 2021210, "zhangsan", 20, "male");
	// auto ret = conn.connect("localhost", 3306, "yd", "123456", "t1");
	// if(false == ret) std::cout << "error " <<std::endl;
	// conn.update(sql);

	clock_t begin = clock();
	for (size_t i = 0; i < 1000; i++)
	{
		/*Connection conn;*/
		char sql[1024] = { 0 };
		sprintf(sql, "insert into user(id,name,age,sex) values(%d,'%s',%d,'%s')", 2021210+i, "zhangsan", 20, "male");

		shared_ptr<Connection> sp = cp->getConnection();
		sp->update(sql);
	}

	//���̲߳���
	// thread t1([]() {
	// 	ConnectionPool* cp = ConnectionPool::getConnectionPool();
	// for (size_t i = 0; i < 2500; i++)
	// {
	// 	char sql[1024] = { 0 };
	// 	sprintf(sql, "insert into user(id,name,age,sex) values(%d,'%s',%d,'%s')", 2021210+i, "zhangsan", 20, "male");
	// 	shared_ptr<Connection> sp = cp->getConnection();
	// 	sp->update(sql);
	// }
	// 	});


	// thread t2([]() {
	// 	ConnectionPool* cp = ConnectionPool::getConnectionPool();
	// 	shared_ptr<Connection> sp = cp->getConnection();
	// for (size_t i = 0; i < 2500; i++)
	// {
	// 	char sql[1024] = { 0 };
	// 	sprintf(sql, "insert into user(id,name,age,sex) values(%d,'%s',%d,'%s')", 2021210 + i, "zhangsan", 20, "male");
	// 	shared_ptr<Connection> sp = cp->getConnection();
	// 	sp->update(sql);
	// }
	// 	});


	// thread t3([]() {
	// 	ConnectionPool* cp = ConnectionPool::getConnectionPool();
	// 	shared_ptr<Connection> sp = cp->getConnection();
	// for (size_t i = 0; i < 2500; i++)
	// {
	// 	char sql[1024] = { 0 };
	// 	sprintf(sql, "insert into user(id,name,age,sex) values(%d,'%s',%d,'%s')", 2021210 + i, "zhangsan", 20, "male");
	// 	shared_ptr<Connection> sp = cp->getConnection();
	// 	sp->update(sql);
	// }
	// 	});



	// thread t4([]() {
	// 	ConnectionPool* cp = ConnectionPool::getConnectionPool();
		
	// for (size_t i = 0; i < 2500; i++)
	// {
	// 	char sql[1024] = { 0 };
	// 	sprintf(sql, "insert into user(id,name,age,sex) values(%d,'%s',%d,'%s')", 2021210 + i, "zhangsan", 20, "male");
	// 	shared_ptr<Connection> sp = cp->getConnection();
	// 	sp->update(sql);
	// }
	// 	});


	// t1.join();
	// t2.join();
	// t3.join();
	// t4.join();

	clock_t end = clock();
	cout << end - begin << " ms " << endl;

	return 0;
}