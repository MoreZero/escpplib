all:
	g++ -g -std=c++0x -I ../include seq_thread_processor_test.cpp -o seq_thread_processor_test -lpthread
	g++ -g -std=c++0x -I ../include  log_test.cpp ../src/libescpplib.a  -o log_test -lpthread
	g++ -g -std=c++0x -I ../include  cost_time_test.cpp ../src/libescpplib.a  -o cost_time_test 
	g++ -g -std=c++0x -I ../include  config_test.cpp ../src/libescpplib.a  -o config_test 
	g++ -g -std=c++0x -I ../include  socket_test.cpp ../src/libescpplib.a  -o socket_test 
	g++ -g -std=c++0x -I ../include  socket_client_test.cpp ../src/libescpplib.a  -o socket_client_test 
	g++ -g -std=c++0x -I ../include  event_test.cpp ../src/libescpplib.a  -o event_test 

