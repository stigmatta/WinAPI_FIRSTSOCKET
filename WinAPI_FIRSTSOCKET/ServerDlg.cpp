#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <string>
#include "WinSock2.h" 
#include <ws2tcpip.h> 
#pragma comment(lib, "Ws2_32.lib") 
#include "ServerDlg.h"
using namespace std;


CServerDlg* CServerDlg::ptr = nullptr;


const int MAXSTRLEN = 255;
WSADATA wsaData;
SOCKET _socket;
SOCKET acceptSocket;
sockaddr_in addr;

void CreateServerSocket() {
	WSAStartup(MAKEWORD(2, 2), &wsaData);
	_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	addr.sin_family = AF_INET;
	inet_pton(AF_INET, "0.0.0.0", &addr.sin_addr);
	addr.sin_port = htons(20000);
	bind(_socket, (SOCKADDR*)&addr, sizeof(addr));
	listen(_socket, 1);
	acceptSocket = accept(_socket, NULL, NULL);
	char buf[MAXSTRLEN];
    GetWindowTextA(CServerDlg::ptr->hEdit, buf, MAXSTRLEN);
    send(acceptSocket, buf, strlen(buf) + 1, 0);
	int i = recv(acceptSocket, buf, MAXSTRLEN, 0);
	buf[i] = '\0';

    int size_needed = MultiByteToWideChar(CP_UTF8, 0, buf, -1, NULL, 0);
    wchar_t* dest = new wchar_t[size_needed];
    MultiByteToWideChar(CP_UTF8, 0, buf, -1, dest, size_needed);

    SetWindowText(CServerDlg::ptr->hEditReadOnly, dest);

	closesocket(acceptSocket);
	closesocket(_socket);
	WSACleanup();
	system("pause");
}







CServerDlg::CServerDlg(void)
{
    ptr = this;
}

CServerDlg::~CServerDlg(void)
{
}

void CServerDlg::Cls_OnClose(HWND hwnd)
{
    EndDialog(hwnd, IDCANCEL);
}

BOOL CServerDlg::Cls_OnInitDialog(HWND hwnd, HWND hwndFocus, LPARAM lParam) {
    hButtonStart = GetDlgItem(hwnd, IDC_START);
    hButtonSend = GetDlgItem(hwnd, IDC_SEND);
    hEditReadOnly = GetDlgItem(hwnd, IDC_READONLY);
    hEdit = GetDlgItem(hwnd, IDC_EDIT1);

    return TRUE;
}

void CServerDlg::Cls_OnCommand(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify) {
    TCHAR text[200];
    TCHAR clientText[200];
    if (id == IDC_START) {
        CreateServerSocket();
    }
    else if (id == IDC_SEND) {
        GetDlgItemText(hwnd, IDC_EDIT1, text, MAXSTRLEN);

        int len = WideCharToMultiByte(CP_UTF8, 0, text, -1, NULL, 0, NULL, NULL);
        char* utf8Text = new char[len];
        WideCharToMultiByte(CP_UTF8, 0, text, -1, utf8Text, len, NULL, NULL);

        send(acceptSocket, utf8Text, strlen(utf8Text), 0);

        delete[] utf8Text;

        closesocket(acceptSocket);
        closesocket(_socket);
        WSACleanup();
    }
}




BOOL CALLBACK CServerDlg::DlgProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {
    switch (message)
    {
        HANDLE_MSG(hwnd, WM_CLOSE, ptr->Cls_OnClose);
        HANDLE_MSG(hwnd, WM_INITDIALOG, ptr->Cls_OnInitDialog);
        HANDLE_MSG(hwnd, WM_COMMAND, ptr->Cls_OnCommand);
    }
    return FALSE;
};