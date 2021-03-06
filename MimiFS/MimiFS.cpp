// MimiFS.cpp: 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "ConsoleApp.h"

using namespace std;

int main()
{
	auto app = ConsoleApp::getInstance();

	string cmd;

	cout << "\n";
	cout << "welcome to mimiFS!\n";
	cout << "version 1.0\n\n";

	app->printPrefix();

	while (getline(cin, cmd)) {
		if (cmd == "exit") 
			break;
		else 
			app->handleCommand(cmd);
	}

    return 0;
}

