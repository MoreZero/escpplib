all:
	g++ -g -std=c++11 -Wall -I ../include seq_thread_processor_test.cpp -o seq_thread_processor_test -lpthread
	g++ -g -std=c++11 -Wall -I ../include  log_test.cpp ../src/libescpplib.a  -o log_test -lpthread
	g++ -g -std=c++11 -Wall -I ../include  cost_time_test.cpp ../src/libescpplib.a  -o cost_time_test 
	g++ -g -std=c++11 -Wall -I ../include  config_test.cpp ../src/libescpplib.a  -o config_test 
	g++ -g -std=c++11 -Wall -I ../include  socket_test.cpp ../src/libescpplib.a  -o socket_test 
	g++ -g -std=c++11 -Wall -I ../include  socket_client_test.cpp ../src/libescpplib.a  -o socket_client_test 
	g++ -g -std=c++11 -Wall -I ../include  event_test.cpp ../src/libescpplib.a  -o event_test  -lpthread
	g++ -g -std=c++11 -Wall -I ../include  speed_test.cpp ../src/libescpplib.a  -o speed_test 
	g++ -g -std=c++11 -Wall -I ../include  simple_server_test.cpp ../src/libescpplib.a  -o simple_server_test 
	g++ -g -std=c++11 -Wall -I ../include  udp_socket_test.cpp ../src/libescpplib.a  -o udp_socket_test 
	g++ -g -std=c++11 -Wall -I ../include  udp_socket_pull_proxy.cpp ../src/libescpplib.a  -o udp_socket_pull_proxy 
	g++ -g -std=c++11 -Wall -I ../include  udp_socket_pull_proxy_fifo.cpp ../src/libescpplib.a  -o udp_socket_pull_proxy_fifo
	g++ -g -std=c++11 -Wall -I ../include  udp_socket_hub.cpp ../src/libescpplib.a  -o udp_socket_hub 
	g++ -g -std=c++11 -Wall -I ../include  udp_socket_sender.cpp ../src/libescpplib.a  -o udp_socket_sender 


