// stdafx.cpp : 只包括标准包含文件的源文件
// MimiFS.pch 将作为预编译标头
// stdafx.obj 将包含预编译类型信息

#include "stdafx.h"

// TODO: 在 STDAFX.H 中引用任何所需的附加头文件，
//而不是在此文件中引用

#pragma comment(lib,"winmm.lib")


void Helper::cutPathFromString(std::string pathname, std::vector<std::string>&out) {

	out.clear();
	int last = 0;
	int cur = 0;

	for (auto i : pathname) {
		if (i == '/' || i == '\\') {
			if (cur > last) {
				out.push_back(pathname.substr(last, cur - last));
			}
			last = cur + 1;
		}
		cur++;
	}

	if (cur > last) {
		out.push_back(pathname.substr(last, cur - last));
	}

}