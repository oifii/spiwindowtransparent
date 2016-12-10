/*
 * Copyright (c) 2015-2016 Stephane Poirier
 *
 * stephane.poirier@oifii.org
 *
 * Stephane Poirier
 * 3532 rue Ste-Famille, #3
 * Montreal, QC, H2X 2L1
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */
#include "stdafx.h"
#include "spiutility.h"
#include "CMainWindow.h"
#include "resource.h"
#include <assert.h>
#include <string>
#include <map>
//#include <wchar.h>

using namespace std;

// Message handler for about box.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}

/*
std::map<HWND, wstring> global_hwndstringmap;
BOOL CALLBACK EnumWindowsProc(HWND hwnd, LPARAM lParam)
{

	WCHAR classname[1024];
	WCHAR title[1024];
	GetClassName(hwnd, classname, sizeof(classname));
	//GetWindowText(hwnd, title, sizeof(title));
	if (wcsstr(classname, L"spiwindowtransparentclass") != NULL)
	{
		global_hwndstringmap.insert(pair<HWND, wstring>(hwnd, classname));
	}

	return TRUE;
}
*/

CMainWindow::CMainWindow(LPCTSTR lpszCaption, LPWNDCLASSEX lpwndClassEx) 
: CWindowTransparent(lpszCaption, lpwndClassEx)
{

}

CMainWindow::CMainWindow(LPCTSTR lpszCaption, HINSTANCE hInstance, LPCTSTR lpszClassName,
	int x, int y, int xwidth, int yheight,
	BYTE alpha, int titlebardisplay, int menubardisplay)
	: CWindowTransparent(lpszCaption, hInstance, lpszClassName,
	x, y, xwidth, yheight,
	alpha, titlebardisplay, menubardisplay)
{

}

CMainWindow::CMainWindow(LPCTSTR lpszCaption, HINSTANCE hInstance, LPCTSTR lpszClassName,
	UINT style, HICON hIcon, HCURSOR hCursor, HBRUSH hbrBackground,
	LPCTSTR lpszMenuName, HICON hIconSm,
	int x, int y, int xwidth, int yheight,
	BYTE alpha, int titlebardisplay, int menubardisplay)
	: CWindowTransparent(lpszCaption, hInstance, lpszClassName,
	style, hIcon, hCursor, hbrBackground,
	lpszMenuName, hIconSm,
	x, y, xwidth, yheight,
	alpha, titlebardisplay, menubardisplay)
{

}

CMainWindow::~CMainWindow()
{
}

LRESULT CALLBACK CMainWindow::wndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) 
{

	switch (uMsg)
	{
	case WM_CREATE:
		{

		}
		return 0;
	case WM_COMMAND:
		{
			int wmId = LOWORD(wParam);
			int wmEvent = HIWORD(wParam);
			// Parse the menu selections:
			switch (wmId)
			{
			case IDM_ABOUT:
				//DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hwnd, About);
				DialogBox((HINSTANCE)GetWindowLong(hwnd, GWL_HINSTANCE), MAKEINTRESOURCE(IDD_ABOUTBOX), hwnd, About);
				break;
			case IDM_EXIT:
				DestroyWindow(hwnd);
				break;
			default:
				return DefWindowProc(hwnd, uMsg, wParam, lParam);
			}
			break;
		}
	case WM_SIZE:
	{
		
		if (wParam == SIZE_MINIMIZED)
		{
			_windows.EnumWindowsStructRefresh();
			std::map<HWND, wstring>::iterator it;
			for (it = _windows.hwndstringmap.begin(); it != _windows.hwndstringmap.end(); it++) 
			{
				HWND myHWND = it->first;
				if (myHWND != hwnd) ShowWindow(myHWND, SW_MINIMIZE);
			}
		}
		else if (wParam == SIZE_RESTORED)
		{
			_windows.EnumWindowsStructRefresh();
			std::map<HWND, wstring>::iterator it;
			for (it = _windows.hwndstringmap.begin(); it != _windows.hwndstringmap.end(); it++)
			{
				HWND myHWND = it->first;
				if (myHWND != hwnd) ShowWindow(myHWND, SW_RESTORE);
			}
		}
		
		break;
	}
/*	case WM_ERASEBKGND:
		{
		}
		break;
*/
		/*
	case WM_SYSCOMMAND:
	{
		if (wParam == SC_MINIMIZE) return true;
	}
	*/
	case WM_KEYDOWN:
		{
			if (wParam == VK_RIGHT || wParam == VK_LEFT || wParam == VK_DOWN || wParam == VK_UP)
			{
				HMONITOR hMonitor = MonitorFromWindow(hwnd, MONITOR_DEFAULTTOPRIMARY);
				int totalmonitors = _monitors.rcMonitors.size();
				if (wParam == VK_RIGHT)
				{
					if (totalmonitors == 1) return 0;
					int i = -1;
					for (i = 0; i < _monitors.hMonitors.size(); ++i) 
					{
						if(hMonitor== _monitors.hMonitors[i]) break;
					}
					i++; //take the next monitor
					if (i >= _monitors.hMonitors.size()) i = 0;
					HMONITOR desthMonitor = _monitors.hMonitors[i];
					std::map<HMONITOR, RECT>::iterator it;
					it = _monitors.hmapMonitors.find(desthMonitor);
					if (it != _monitors.hmapMonitors.end())
					{
						RECT myRECT = (*it).second;
						SetWindowPos(hwnd, HWND_TOP, myRECT.left, myRECT.top, abs(myRECT.right - myRECT.left), abs(myRECT.bottom-myRECT.top), SWP_SHOWWINDOW);
					}
					else
					{
						assert(false);
					}
				}
				else if (wParam == VK_LEFT)
				{
					if (totalmonitors == 1) return 0;
					int i = _monitors.hMonitors.size();
					for (i = 0; i < _monitors.hMonitors.size(); ++i)
					{
						if (hMonitor == _monitors.hMonitors[i]) break;
					}
					i--; //take the next monitor
					if (i < 0) i = _monitors.hMonitors.size()-1;
					HMONITOR desthMonitor = _monitors.hMonitors[i];
					std::map<HMONITOR, RECT>::iterator it;
					it = _monitors.hmapMonitors.find(desthMonitor);
					if (it != _monitors.hmapMonitors.end())
					{
						RECT myRECT = (*it).second;
						SetWindowPos(hwnd, HWND_TOP, myRECT.left, myRECT.top, abs(myRECT.right - myRECT.left), abs(myRECT.bottom - myRECT.top), SWP_SHOWWINDOW);
					}
					else
					{
						assert(false);
					}

				}
				else if (wParam == VK_DOWN)
				{
					SetWindowPos(hwnd, HWND_BOTTOM, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
					//global_hwndstringmap.clear();
					//EnumWindows(EnumWindowsProc, NULL);
					_windows.EnumWindowsStructRefresh();
					std::map<HWND, wstring>::iterator it;
					it = _windows.hwndstringmap.find(hwnd);
					if (it != _windows.hwndstringmap.end())
					{
						it++; //take the next hwnd
						if (it == _windows.hwndstringmap.end()) it = _windows.hwndstringmap.begin();
						HWND nexthwnd = (*it).first;
						
						SetWindowPos(nexthwnd, HWND_TOP, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_SHOWWINDOW);
						//SwitchToThisWindow(nexthwnd, false);
						//SetActiveWindow(nexthwnd);
						
						//SetWindowPos(nexthwnd, HWND_TOP, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
						//ShowWindow(nexthwnd, SW_MAXIMIZE | SW_SHOW);
					}
				}
				else if (wParam == VK_UP)
				{
					SetWindowPos(hwnd, HWND_BOTTOM, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
					//global_hwndstringmap.clear();
					//EnumWindows(EnumWindowsProc, NULL);
					_windows.EnumWindowsStructRefresh();
					std::map<HWND, wstring>::iterator it;
					it = _windows.hwndstringmap.find(hwnd);
					if (it != _windows.hwndstringmap.end())
					{
						//it--; //take the next hwnd
						//if (it == _windows.hwndstringmap.begin()) it = _windows.hwndstringmap.end();
						it++; //take the next hwnd
						if (it == _windows.hwndstringmap.end()) it = _windows.hwndstringmap.begin();
						HWND nexthwnd = (*it).first;
						
						SetWindowPos(nexthwnd, HWND_TOP, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_SHOWWINDOW);
						//SwitchToThisWindow(nexthwnd, false);
						//SetActiveWindow(nexthwnd);
						
						//SetWindowPos(nexthwnd, HWND_TOP, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
						//ShowWindow(nexthwnd, SW_MAXIMIZE | SW_SHOW);
					}
				}
			}
		}
		return 0;
	case WM_DESTROY:
		::PostQuitMessage(0);
		return 0;
	default:
		return ::DefWindowProc(hwnd, uMsg, wParam, lParam);
	}
}