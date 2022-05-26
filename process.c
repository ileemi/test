/* ====================================================
* Copyright: 275659704@qq.com
* Author: wfy
* Date: 2018-4-15
* Description: 提供CreateProcess()执行
* 命令并通过匿名管道获取的封装
==================================================== */

#ifndef __CMD_HANDLER_H__
#define __CMD_HANDLER_H__

#include <Windows.h>

/* buffer的最大长度 */
#define PIPE_BUFFER_SIZE 1024

/* 命令参数 */
 typedef struct _CHCmdParam
{
    /* 初始化为 sizeof(CommandParam) */
    int iSize;

    /* 外部命令，不需要用则设置为-1, 提供给外部使用
    */
    int iCommand;

    /* 超时时间，单位秒 */
    int iTimeOut;

    /* 命令行要执行的命令 */
    char szCommand[MAX_PATH];

    /* 用户数据 */
    void* pUserData;

    /* 命令执行后的回调 */
    void (*OnCmdEvent)(const CHCmdParam* pParam, HRESULT hResultCode, char* szResult);
}CHCmdParam;

class CCmdHandler
{
private:
    BOOL m_bInit;
    STARTUPINFOA m_startupInfo;
    PROCESS_INFORMATION m_processInfo;
    SECURITY_ATTRIBUTES m_saOutPipe;
    DWORD m_dwErrorCode;
    HANDLE m_hPipeRead;
    HANDLE m_hPipeWrite;
    CHCmdParam m_CommandParam;
    TCHAR m_szReadBuffer[PIPE_BUFFER_SIZE];
    TCHAR m_szWriteBuffer[PIPE_BUFFER_SIZE];
    char m_szPipeOut[PIPE_BUFFER_SIZE];
    HRESULT ExecuteCmdWait();
public:
    CCmdHandler();
    ~CCmdHandler();
    /*
    * 初始化接口，调用其余接口之前调用
    * 成功返回S_OK
    */
    HRESULT Initalize();
    /*
    * 结束接口
    */
    HRESULT Finish();
    /*
    * 执行命令接口，接口调用成功返回S_OK
    * param[in] pCommmandParam: 指向一个CHCmdParam命令参数结构的指针
    */
    HRESULT HandleCommand(CHCmdParam* pCommmandParam);
    /*
    * 返回错误码，便于差距接口调用失败后产生什么错误
    */
    DWORD GetErrorCode() { return m_dwErrorCode; }
};

#endif // !__CMD_HANDLER_H__



// .cpp

#include "cmd.h"
#include <tchar.h>

#define EXCEPTIION_STATE_CHECK \
    if (!m_bInit) return E_NOTIMPL

CCmdHandler::CCmdHandler() :
    m_bInit(FALSE),
    m_dwErrorCode(0),
    m_hPipeRead(NULL),
    m_hPipeWrite(NULL)
{
    ZeroMemory(m_szReadBuffer, sizeof(m_szReadBuffer));
    ZeroMemory(m_szWriteBuffer, sizeof(m_szWriteBuffer));
    ZeroMemory(&m_CommandParam, sizeof(m_CommandParam));
}
CCmdHandler::~CCmdHandler()
{
}

HRESULT CCmdHandler::Initalize()
{
    // 初始化，创建匿名管道
    if (m_bInit) return S_OK;
    m_bInit = TRUE;
    ZeroMemory(m_szReadBuffer, sizeof(m_szReadBuffer));
    ZeroMemory(&m_saOutPipe, sizeof(m_saOutPipe));

    m_saOutPipe.nLength = sizeof(SECURITY_ATTRIBUTES);
    m_saOutPipe.lpSecurityDescriptor = NULL;
    m_saOutPipe.bInheritHandle = TRUE;

    ZeroMemory(&m_startupInfo, sizeof(STARTUPINFO));
    ZeroMemory(&m_processInfo, sizeof(PROCESS_INFORMATION));
    if (!CreatePipe(&m_hPipeRead, &m_hPipeWrite, &m_saOutPipe, PIPE_BUFFER_SIZE))
    {
        m_dwErrorCode = GetLastError();
        return E_FAIL;
    }
    return S_OK;
}
HRESULT CCmdHandler::Finish()
{
    EXCEPTIION_STATE_CHECK;
    if (m_hPipeRead)
    {
        CloseHandle(m_hPipeRead);
        m_hPipeRead = NULL;
    }
    if (m_hPipeWrite)
    {
        CloseHandle(m_hPipeWrite);
        m_hPipeWrite = NULL;
    }
    return S_OK;
}
HRESULT CCmdHandler::HandleCommand(CHCmdParam* pCommmandParam)
{
    EXCEPTIION_STATE_CHECK;
    if (!pCommmandParam || pCommmandParam->iSize != sizeof(CHCmdParam))
        return E_INVALIDARG;
    if (strlen(pCommmandParam->szCommand) <= 0)
        return E_UNEXPECTED;
    memset(&m_CommandParam, 0, sizeof(m_CommandParam));
    m_CommandParam = *pCommmandParam;
    return ExecuteCmdWait();
}
HRESULT CCmdHandler::ExecuteCmdWait()
{
    EXCEPTIION_STATE_CHECK;
    HRESULT hResult = E_FAIL;
    DWORD dwReadLen = 0;
    DWORD dwStdLen = 0;
    m_startupInfo.cb = sizeof(STARTUPINFO);
    m_startupInfo.dwFlags = STARTF_USESTDHANDLES | STARTF_USESHOWWINDOW;
    m_startupInfo.hStdOutput = m_hPipeWrite;
    m_startupInfo.hStdError = m_hPipeWrite;
    m_startupInfo.wShowWindow = SW_HIDE;
    DWORD dTimeOut = m_CommandParam.iTimeOut >= 3000 ? m_CommandParam.iTimeOut : 5000;

    if (!CreateProcessA(NULL, m_CommandParam.szCommand,
        NULL, NULL, TRUE, CREATE_NEW_CONSOLE, NULL, NULL,
        &m_startupInfo, &m_processInfo))
    {
        m_dwErrorCode = GetLastError();
        hResult = E_FAIL;
    }


    if (WAIT_TIMEOUT == WaitForSingleObject(m_processInfo.hProcess, dTimeOut))
    {
        m_dwErrorCode = GetLastError();
        hResult = CO_E_SERVER_START_TIMEOUT;
        if (m_CommandParam.OnCmdEvent)
            m_CommandParam.OnCmdEvent(&m_CommandParam, CO_E_SERVER_START_TIMEOUT, (char*)"");
        //break;
    }
    // 预览管道中数据的内容
    if (!PeekNamedPipe(m_hPipeRead, NULL, 0, NULL, &dwReadLen, NULL)
        || dwReadLen <= 0)
    {
        m_dwErrorCode = GetLastError();
        hResult = E_FAIL;
        //break;
    }
    else
    {
        ZeroMemory(m_szPipeOut, sizeof(m_szPipeOut));
        // 读取管道中的数据
        if (ReadFile(m_hPipeRead, m_szPipeOut, dwReadLen, &dwStdLen, NULL))
        {
            hResult = S_OK;
            if (m_CommandParam.OnCmdEvent)
                m_CommandParam.OnCmdEvent(&m_CommandParam, S_OK, m_szPipeOut);
            //break;
        }
        else
        {
            m_dwErrorCode = GetLastError();
            //break;
        }
    }

    //do
    //{

    //} while (0);
    if (m_processInfo.hThread)
    {
        CloseHandle(m_processInfo.hThread);
        m_processInfo.hThread = NULL;
    }
    if (m_processInfo.hProcess)
    {
        CloseHandle(m_processInfo.hProcess);
        m_processInfo.hProcess = NULL;
    }
    return hResult;
}



// main.cpp

#include <Windows.h>
#include <stdio.h>
#include <tchar.h>
#include <string>
#include "cmd.h"

#define SYSTEM_PAUSE system("pause")

void OnCommandEvent(const CHCmdParam* pParam, HRESULT hResultCode, char* szResult);

int main(int argc, TCHAR** argv)
{
    CHCmdParam cmdParam;
    CCmdHandler cmdHandler;
    HRESULT cmdResult = S_OK;

    ZeroMemory(&cmdParam, sizeof(cmdParam));
    cmdParam.iSize = sizeof(CHCmdParam);
    // 这里测试F磁盘Test目录下，命令格式为 cmd.exe /C + 命令
    char* szCmd = (char*)("cmd.exe /C netstat -ano && echo S_OK || echo E_FAIL");
    strcpy(cmdParam.szCommand, szCmd);
    cmdParam.OnCmdEvent = OnCommandEvent;
    cmdParam.iTimeOut = 5000;

    cmdResult = cmdHandler.Initalize();
    if (cmdResult != S_OK)
    {
        printf("cmd handler 初始化失败\n");
        SYSTEM_PAUSE;
        return 0;
    }
    cmdResult = cmdHandler.HandleCommand(&cmdParam);
    if (cmdResult != S_OK)
    {
        printf("cmd handler 执行命令接口调用失败\n");
        cmdHandler.Finish();
        SYSTEM_PAUSE;
        return 0;
    }
    system("pause");
    return 0;
}

void OnCommandEvent(const CHCmdParam* pParam, HRESULT hResultCode, char* szResult)
{
    if (!szResult || !szResult[0]) return;
    if (!pParam || hResultCode != S_OK) return;
    printf("============== 回调 ==============\n");
    std::string echo_data(szResult);
    std::string s_ok("S_OK");
    std::string::size_type pos = echo_data.find(s_ok);
    if (pos != std::string::npos)
        printf("命令执行成功\n");
    else
        printf("命令执行失败\n");
    printf("执行返回的结构:\n");
    printf("========================================\n");
    printf("%s\n", szResult);
}

