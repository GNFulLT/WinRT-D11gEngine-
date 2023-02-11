#pragma once

#include <cstddef>
#include <cstdint>
#include <exception>
#include <fstream>
#include <stdexcept>
#include <system_error>
#include <vector>
#include <Windows.h>
#include "../Logger.h"
namespace GNF::Common::Utils
{
    inline std::string WCharToChar(const wchar_t* n)
    {
        std::wstring in(n);
        int size_needed = WideCharToMultiByte(CP_UTF8, 0, in.c_str(), (int)in.size(), NULL, 0,NULL,NULL);
        std::string out(size_needed,0);
        WideCharToMultiByte(CP_UTF8, 0, in.c_str(), (int)in.size(), &out[0], size_needed, NULL, NULL);
       
        return out;
    }

    inline std::vector<uint8_t> ReadData(_In_z_ const wchar_t* name)
    {
        std::ifstream inFile(name, std::ios::in | std::ios::binary | std::ios::ate);
        Logger::LogDebug(std::format("Trying to read {}", WCharToChar(name).c_str()).c_str());
#if !defined(WINAPI_FAMILY) || (WINAPI_FAMILY == WINAPI_FAMILY_DESKTOP_APP)
        if (!inFile)
        {
            wchar_t moduleName[_MAX_PATH] = {};
            if (!GetModuleFileNameW(nullptr, moduleName, _MAX_PATH))
            {
                Logger::LogError("Module cannot getted");
                throw std::system_error(std::error_code(static_cast<int>(GetLastError()), std::system_category()), "GetModuleFileNameW");
            }
            wchar_t drive[_MAX_DRIVE];
            wchar_t path[_MAX_PATH];

            if (_wsplitpath_s(moduleName, drive, _MAX_DRIVE, path, _MAX_PATH, nullptr, 0, nullptr, 0))
            {
                Logger::LogError("_wsplitpath_s");
                throw std::runtime_error("_wsplitpath_s");
            }

            wchar_t filename[_MAX_PATH];
            if (_wmakepath_s(filename, _MAX_PATH, drive, path, name, nullptr))
            {
                Logger::LogError("_wmakepath_s");
                throw std::runtime_error("_wmakepath_s");
            }

            inFile.open(filename, std::ios::in | std::ios::binary | std::ios::ate);
        }
#endif

        if (!inFile)
        {
            Logger::LogError("inFile ReadData");

            throw std::runtime_error("ReadData");
        }
        const std::streampos len = inFile.tellg();
        if (!inFile)
        {
            Logger::LogError("inFile2 ReadData");
            throw std::runtime_error("ReadData");
        }

        std::vector<uint8_t> blob;
        blob.resize(size_t(len));

        inFile.seekg(0, std::ios::beg);
        if (!inFile)
        {
            Logger::LogError("inFile3 ReadData");
            throw std::runtime_error("ReadData");
        }
        inFile.read(reinterpret_cast<char*>(blob.data()), len);
        if (!inFile)
        {
            Logger::LogError("inFile4 ReadData");
            throw std::runtime_error("ReadData");
        }
        inFile.close();

        return blob;
    }
}