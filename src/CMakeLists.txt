
set(SRC_LIST commonConnection.cpp ConnectionPool.cpp)


#制作一下动态链接库 不包含main文件即可
add_library(mysqlpool SHARED ${SRC_LIST})

install(TARGETS mysqlpool 
        LIBRARY DESTINATION ${PROJECT_SOURCE_DIR}/lib
        PERMISSIONS OWNER_EXECUTE OWNER_WRITE OWNER_READ
                    GROUP_EXECUTE GROUP_READ)

install(FILES mysql.init DESTINATION ${PROJECT_SOURCE_DIR}/bin)       
install(FILES mysql.init DESTINATION ${PROJECT_SOURCE_DIR}/lib)     

add_executable(${PROJECT_NAME} ${SRC_LIST} ./main.cpp)


target_link_libraries(${PROJECT_NAME} mysqlclient pthread)