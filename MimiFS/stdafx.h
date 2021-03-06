// stdafx.h : 标准系统包含文件的包含文件，
// 或是经常使用但不常更改的
// 特定于项目的包含文件
//

#pragma once

#include "targetver.h"

#define FS_FILETYPE_FOLDER 1
#define FS_FILETYPE_FILE 0

#define SINGLE_INSTANCE 

#include <stdio.h>
#include <io.h>

#include <tchar.h>
#include <unordered_map>
#include <functional>
#include <iostream>
#include <sstream>
#include<windows.h>
#include <exception>

struct HelpItem
{
	std::string title;
	std::string detail;
};

namespace Helper {
	void cutPathFromString(std::string pathname, std::vector<std::string>&out);

}
#include "MyFileWriter.h"

#define FDASSERT(f,m) if(!(f)){throw std::exception(m);}


#include "MiniFile.h"
#include "CommandHandler.h"


#include "ConsoleApp.h"

#define REGISTER_HANDLER(classname,cmd,helpTitle,helpDetail) namespace{static auto myRegisterHelper##__COUNTER__=[]()->int{ConsoleApp::getInstance()->handler[cmd]=new classname;ConsoleApp::getInstance()->helpList[cmd]={helpTitle,helpDetail};return 0;}();};








// TODO: 在此处引用程序需要的其他头文件
