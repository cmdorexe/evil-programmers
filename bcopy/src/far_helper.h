/*
    far_helper.h
    Copyright (C) 2008 zg

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program. If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef __FAR_HELPER_H__
#define __FAR_HELPER_H__

#include <tchar.h>
#if defined(__GNUC__) && !defined(_export)
# define _export
#endif
#include "plugin.hpp"
extern PluginStartupInfo Info;
#define ArraySize(a) (sizeof(a)/sizeof(a[0]))

#ifdef UNICODE
#define INIT_DLG_DATA(item,str) item.PtrData=str
#define INIT_MENU_TEXT(index,str) MenuItems[index].Text=str
#define DLG_DATA(item) item.PtrData
class CFarDialog
{
  private:
    HANDLE iDlg;
  public:
    inline CFarDialog(): iDlg(INVALID_HANDLE_VALUE) {};
    ~CFarDialog();
    int Execute(INT_PTR PluginNumber,int X1,int Y1,int X2,int Y2,const TCHAR* HelpTopic,struct FarDialogItem* Item,int ItemsNumber,DWORD Reserved,DWORD Flags,FARWINDOWPROC DlgProc,LONG_PTR Param);
    inline HANDLE Handle(void) {return iDlg;};
    int Check(int index);
    const wchar_t* Str(int index);
    inline DWORD Flags(int index)
    {
      FarDialogItem DialogItem;
      if(Info.SendDlgMessage(iDlg,DM_GETDLGITEMSHORT,index,(LONG_PTR)&DialogItem)) return DialogItem.Flags;
      return 0;
    };
    inline DWORD Type(int index)
    {
      FarDialogItem DialogItem;
      if(Info.SendDlgMessage(iDlg,DM_GETDLGITEMSHORT,index,(LONG_PTR)&DialogItem)) return DialogItem.Type;
      return 0;
    };
    inline int ListPos(int index) {return Info.SendDlgMessage(iDlg,DM_LISTGETCURPOS,index,0);};
};
#define EXP_NAME(p) _export p ## W
#define EXP_NAME_CALL(p) p ## W
#define t_CharToOem(s,d) wcscpy(d,s)
#define t_OemToChar(s,d) wcscpy(d,s)
typedef wchar_t UTCHAR;
#define PANEL_FILENAME lpwszFileName
#define FIRST_PARAM int
#define SECOND_PARAM LONG_PTR
#define ControlShort(a,b,c) Control(a,b,0,c)
#define ControlShort2(a,b,c) Control(a,b,c,0)
#define ControlShort3(a,b,c) Control(a,b,c,0)
#define FCTL_GETPANELSHORTINFO FCTL_GETPANELINFO
#define FCTL_GETANOTHERPANELSHORTINFO FCTL_GETPANELINFO
#define FCTL_UPDATEANOTHERPANEL FCTL_UPDATEPANEL
#define FCTL_REDRAWANOTHERPANEL FCTL_REDRAWPANEL
#define BOX1 0x2591
#define BOX2 0x2588
#define wc2mb(src,dst,dstsize) {_tcsncpy(dst,src,dstsize-1); dst[dstsize-1]=0;}
#define ConvertNameToReal(a,b,c) FSF.ConvertPath(CPM_REAL,a,b,c)
#else
#define DM_GETDLGITEMSHORT DM_GETDLGITEM
#define DM_SETDLGITEMSHORT DM_SETDLGITEM
#define INIT_DLG_DATA(item,str) _tcscpy(item.Data,str)
#define INIT_MENU_TEXT(index,str) _tcscpy(MenuItems[index].Text,str)
#define DLG_DATA(item) item.Data
class CFarDialog
{
  private:
    FarDialogItem* iItems;
  public:
    inline CFarDialog() {};
    inline int Execute(INT_PTR PluginNumber,int X1,int Y1,int X2,int Y2,const TCHAR* HelpTopic,struct FarDialogItem* Item,int ItemsNumber,DWORD Reserved,DWORD Flags,FARWINDOWPROC DlgProc,LONG_PTR Param)
    {
      iItems=Item;
      return Info.DialogEx(PluginNumber,X1,Y1,X2,Y2,HelpTopic,Item,ItemsNumber,Reserved,Flags,DlgProc,Param);
    };
    inline FarDialogItem* Handle(void) {return iItems;};
    inline int Check(int index) {return iItems[index].Selected;};
    inline const char* Str(int index) {return iItems[index].Data;};
    inline DWORD Flags(int index) {return iItems[index].Flags;};
    inline DWORD Type(int index) {return iItems[index].Type;};
    inline int ListPos(int index) {return iItems[index].ListPos;};
};
#define EXP_NAME(p) _export p
#define EXP_NAME_CALL(p) p
#define t_CharToOem(s,d) CharToOem(s,d)
#define t_OemToChar(s,d) OemToChar(s,d)
typedef unsigned char UTCHAR;
#define PANEL_FILENAME cFileName
#define FIRST_PARAM void*
#define SECOND_PARAM void*
#define ControlShort(a,b,c) Control(a,b,c)
#define ControlShort2(a,b,c) Control(a,b,&c)
#define ControlShort3(a,b,c) Control(a,b,c)
#define BOX1 '�'
#define BOX2 '�'
#define wc2mb(src,dst,dstsize) WideCharToMultiByte(CP_OEMCP,0,src,-1,dst,dstsize,NULL,NULL);
#define ConvertNameToReal(a,b,c) FSF.ConvertNameToReal(a,b,c)
#endif

#ifdef UNICODE
void Realloc(TCHAR*& aData,int& aLength,int aNewLength);
void Realloc(PluginPanelItem*& aData,int& aSize,int aNewSize);

class CFarPanel
{
  private:
    HANDLE iPlugin;
    PanelInfo iInfo;
    int iResult;
    TCHAR* iCurDir;
    int iCurDirSize;
    PluginPanelItem* iItem;
    int iItemSize;
  private:
    CFarPanel();
  public:
    CFarPanel(HANDLE aPlugin,int aCommand);
    ~CFarPanel();
    inline bool IsOk(void) {return iResult;}
    inline int PanelType(void) {return iInfo.PanelType;};
    inline int Plugin(void) {return iInfo.Plugin;};
    inline int ItemsNumber(void) {return iInfo.ItemsNumber;};
    inline int SelectedItemsNumber(void) {return iInfo.SelectedItemsNumber;};
    inline int CurrentItem(void) {return iInfo.CurrentItem;};
    inline DWORD Flags(void) {return iInfo.Flags;};
    TCHAR* CurDir(void);
    PluginPanelItem& operator[](size_t index);
    PluginPanelItem& Selected(size_t index);
    void StartSelection(void);
    void RemoveSelection(size_t index);
    void CommitSelection(void);
};
#else
#define PANEL_ACTIVE  ((HANDLE)(-1))
#define PANEL_PASSIVE ((HANDLE)(-1))
class CFarPanel
{
  private:
    HANDLE iPlugin;
    PanelInfo iInfo;
    int iResult;
  private:
    CFarPanel();
  public:
    inline CFarPanel(HANDLE aPlugin,int aCommand): iPlugin(aPlugin) {iResult=Info.Control(aPlugin,aCommand,(void*)&iInfo);};
    inline bool IsOk(void) {return iResult;};
    inline int PanelType(void) {return iInfo.PanelType;};
    inline int Plugin(void) {return iInfo.Plugin;};
    inline int ItemsNumber(void) {return iInfo.ItemsNumber;};
    inline int SelectedItemsNumber(void) {return iInfo.SelectedItemsNumber;};
    inline int CurrentItem(void) {return iInfo.CurrentItem;};
    inline DWORD Flags(void) {return iInfo.Flags;};
    inline TCHAR* CurDir(void) {return iInfo.CurDir;};
    inline PluginPanelItem& operator[](size_t index) {return iInfo.PanelItems[index];};
    inline PluginPanelItem& Selected(size_t index) {return iInfo.SelectedItems[index];};
    void StartSelection(void) {};
    inline void RemoveSelection(size_t index) {iInfo.PanelItems[index].Flags&=~PPIF_SELECTED;};
    void CommitSelection(void);
};
#endif

#endif
