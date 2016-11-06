README.TXT

Prerequisites;
	Install and compile boost library
		- Download latest boost library from page: http://www.boost.org/
		- Extract files to c:\boost
		- run build_boost.bat



#include<string>  //for length()

request_stream << "POST /title/ HTTP/1.1 \r\n";
request_stream << "Host:" << some_host << "\r\n";
request_stream << "User-Agent: C/1.0";
request_stream << "Content-Type: application/json; charset=utf-8 \r\n";
request_stream << "Accept: */*\r\n";
request_stream << "Content-Length: " << json.length() << "\r\n";    
request_stream << "Connection: close\r\n\r\n";  //NOTE THE Double line feed
request_stream << json;