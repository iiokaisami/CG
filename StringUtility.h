#pragma once
#include <string>
#include <dxgidebug.h>


// 文字コードユーティリティ
namespace StringUtility
{
	// stringを wstringに変換する
	std::wstring ConvertString(const std::string& str);

	// wstringを stringに変換する
	std::string ConvertString(const std::wstring& str);

}