# SqlPool
数据库连接池

```
编译脚本命令为
./autobuild.sh

```


```

/**
 *  TEST_DEMO :
 * 	API : 
 *  @ ConnectionPool::getConnectionPool(); 
 * 	return -> ConnectionPool *cp; 
 * 
 *  @cp->getConnection();
 *  return -> sp<Connection>;
 * 
 *  @ shared_ptr<Connection> sp = cp->getConnection();
 * 	and provide two api for update and query 
 * 	
 *	@update -> update and  selecet and delete and insert
 * 	@query  ->  return MYSQL *; 
 * 
 * 
*/
```

```
接口功能说明。
首先需要创建一个连接池，连接池的设置为单例模式。

然后每个链接都从两个固定方法中获取即可。


其中 mysql.init为链接的数据库的配置文件,个字段意义见注释所示。
