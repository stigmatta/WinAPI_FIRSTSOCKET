#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <string>
#include "WinSock2.h" 
#include <ws2tcpip.h> 
#include "ClientDlg.h"
#pragma comment(lib, "Ws2_32.lib") 

using namespace std;

CClientDlg* CClientDlg::ptr = nullptr;

const int MAXSTRLEN = 255;
WSADATA wsaData;
SOCKET _socket;
sockaddr_in addr;

void CreateClientSocket(HWND hwnd) {
	WSAStartup(MAKEWORD(2, 2), &wsaData);
	_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	addr.sin_family = AF_INET;
	inet_pton(AF_INET, "127.0.0.1", &addr.sin_addr);
	addr.sin_port = htons(20000);
	connect(_socket, (SOCKADDR*)&addr, sizeof(addr));

	char buf[MAXSTRLEN],buf2[MAXSTRLEN];
    int i = recv(_socket, buf, MAXSTRLEN, 0);
    buf[i] = '\0';
    SetWindowTextA(CClientDlg::ptr->hEditReadOnly, buf);
    GetWindowTextA(CClientDlg::ptr->hEdit, buf2,strlen(buf2));
	send(_socket, buf2, strlen(buf2), 0);

	


	closesocket(_socket);
	WSACleanup();
	system("pause");
}


CClientDlg::CClientDlg(void) {
    ptr = this;
}

CClientDlg::~CClientDlg(void) {
}

void CClientDlg::Cls_OnClose(HWND hwnd) {
    EndDialog(hwnd, IDCANCEL);
}

BOOL CClientDlg::Cls_OnInitDialog(HWND hwnd, HWND hwndFocus, LPARAM lParam) {
    hButtonConnect = GetDlgItem(hwnd, IDCONNECT);
    hButtonSend = GetDlgItem(hwnd, IDC_SEND);
    hEditReadOnly = GetDlgItem(hwnd, IDC_READONLY);
    hEdit = GetDlgItem(hwnd, IDC_EDIT2);
    return TRUE;
}

void CClientDlg::Cls_OnCommand(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify) {
    TCHAR text[200];
    if (id == IDCONNECT) {
        CreateClientSocket(hwnd);
    }
    else if (id == IDC_SEND) {
        GetDlgItemText(hwnd, IDC_EDIT2, text, MAXSTRLEN);
        int len = WideCharToMultiByte(CP_UTF8, 0, text, -1, NULL, 0, NULL, NULL);
        char* utf8Text = new char[len];
        WideCharToMultiByte(CP_UTF8, 0, text, -1, utf8Text, len, NULL, NULL);
        send(_socket, utf8Text, strlen(utf8Text), 0);
        delete[] utf8Text;
        closesocket(_socket);
        WSACleanup();
    }
}

BOOL CALLBACK CClientDlg::DlgProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {
    switch (message) {
        HANDLE_MSG(hwnd, WM_CLOSE, ptr->Cls_OnClose);
        HANDLE_MSG(hwnd, WM_INITDIALOG, ptr->Cls_OnInitDialog);
        HANDLE_MSG(hwnd, WM_COMMAND, ptr->Cls_OnCommand);
    }
    return FALSE;
}
