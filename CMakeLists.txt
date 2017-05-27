cmake_minimum_required (VERSION 2.6)
project (SocketNcurses)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules/")

set(CMAKE_BUILD_TYPE debug)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_EXPORT_COMPILE_COMMAND ON)
find_package(Curses REQUIRED)
find_package(Threads REQUIRED)
if(CURSES_FOUND)
	if(Threads_FOUND)
		set(INCS ${INCS} ${CURSES_INCLUDE_DIRS} "include/" "tests/")
		include_directories(${INCS})
		set(LIBS ${LIBS} ${CURSES_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT} "/usr/lib/libcpptest.so" )
		set(TEST_LIBS ${TEST_LIBS} "/usr/lib/libcpptest.so")

		set(SRC "src")

		add_executable(SocketNcurses main.cpp ${SRC}/Client.cpp ${SRC}/Entity.cpp ${SRC}/Common.cpp ${SRC}/Event.cpp ${SRC}/GameState.cpp ${SRC}/Network.cpp ${SRC}/NetworkStatistics.cpp ${SRC}/Player.cpp ${SRC}/Server.cpp ${SRC}/Timer.cpp ${SRC}/Buffer.cpp ${SRC}/WriteBuffer.cpp ${SRC}/ReadBuffer.cpp ${SRC}/Frame.cpp ${SRC}/Game.cpp ${SRC}/Log.cpp)
		add_executable(RunTests tests/Test_Main.cpp tests/Test_Buffer.cpp tests/Test_BitsRequired.cpp tests/Test_ReadWriteBuffer.cpp ${SRC}/Buffer.cpp tests/Test_Frame.cpp ${SRC}/Frame.cpp ${SRC}/ReadBuffer.cpp ${SRC}/WriteBuffer.cpp)

		target_link_libraries(SocketNcurses ${LIBS})
		target_link_libraries(RunTests ${TEST_LIBS})
	endif(Threads_FOUND)
endif(CURSES_FOUND)

