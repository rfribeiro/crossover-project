README.TXT

Prerequisites;
	Install and compile boost library:
		- Download latest boost library from page: http://www.boost.org/
		- Extract files to c:\boost
		- run build_boost.bat

	Configuring database:
		- using Sqlite database
		- Not necessary
		- database will be save at current folder with name test.db

Assumptions:
	We assume that the server will always receive the package and answer the client
	The server will check the package before insert it in the database
	The is no authentication previosly the client connect and send a post message to server
	the post message is fixed.
	It is possible to extend implementation to receive data from any source using json post

Project Compilation:
	The system was developed using Visual Studio Community version 2015.
	To compile:
	- open crossover-project.sln in Visual Studio
	- set build configuration to "Release x86"
	- Build solution

System details:
	copy all configuration xml files to same folder of executable file client-configuration.xml, client-data.xml, server-configuration.xml
	run server-app.exe in terminal
	run client-app.exe in terminal

	if you like to add more clients, change client key in client-configuration.xml
	also check if client is registered on server in file client-data.xml

	
Difficults:
	- problem when run tests using boost.test library.
	- test.db (TODO: read from configuration file)
	- CServerConnection (TODO: improve packet reading from any size)
	- CServerConnection and CClientConnection (TODO: improve authentication)

