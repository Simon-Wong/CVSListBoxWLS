// VSListBoxWLS.h : 定义文件
#pragma once

#include"afxvslistbox.h"


//////////////////////////////////////////////////////////////////////////
//CVSListBoxWLS
//by wls
//The memory leaks is made by  MFC,not me,for the little buttons in CVSListBox called CMFCToolTipCtrl or something named XXXButton.
//If you change the Appearance of CVSListBox to false,it will work with no memory leaks.
//That means HEHE.

typedef BOOL (*FUNCDOSOMETHING)(LPVOID /*lpRawData*/,LPVOID /*lpNewData*/,CObject* /*pObj*/);//by wls You know that

BOOL FuncDoNothing(LPVOID,LPVOID,CObject*);//by wls An idle function to do nothing

class CVSListBoxWLS : public CVSListBox
{
    DECLARE_DYNAMIC(CVSListBoxWLS)

public:
    CVSListBoxWLS();
    virtual ~CVSListBoxWLS();

private:
    BOOL m_bEnableRepeatText;//True for enable to insert repeat text,false for disable.
    BOOL m_bDoAfterAddItemWhenever;//True for enable to do something whenever really insert text or not due to m_bEnableRepeatText,false for only really inserting text
    BOOL m_bDoAfterRenameWhenever;//The usage like above
    BOOL m_bAddItem;
    BOOL m_bCheckTextLen;
    BOOL m_nTextLen;

    CString m_strWhenSelecting;//by wls The raw string
    CString m_strAfterOp;//by wls The new string
    CObject* m_pObjforOperatingFunc;

    FUNCDOSOMETHING m_fdsBeforeRemoveItem;
    FUNCDOSOMETHING m_fdsAfterAddItemNoRepeat;
    FUNCDOSOMETHING m_fdsAfterAddItemWhenever;
    FUNCDOSOMETHING m_fdsAfterRenameItemNoRepeat;
    FUNCDOSOMETHING m_fdsAfterRenameItemWhenever;

private:
    BOOL IsExistText(CString strText);
    void GetItemTextAfterOp(int nItem);

public:
    CString GetTextWhenSelecting();

    void EnableRepeatText(BOOL bRepeat=FALSE);//by wls It depends the client code in where you save all the data to the very end.
    void EnableDoAfterAddItemWhenever(BOOL bRepeat=FALSE);
    void EnableDoAfterRenameWhenever(BOOL bRepeat=FALSE);

    void EnableCheckTextLen(BOOL bChk=FALSE);
    void SetTextLegalLength(int nLen=10);

    void SetObjforOperation(CObject* obj=NULL);

    void SetOperationBeforeRemoveItem(FUNCDOSOMETHING fds);
    void SetOperationAfterAddItemNoRepeat(FUNCDOSOMETHING fds);
    void SetOperationAfterAddItemWhenever(FUNCDOSOMETHING fds);
    void SetOperationAfterRenameItemNoRepeat(FUNCDOSOMETHING fds);
    void SetOperationAfterRenameItemWhenever(FUNCDOSOMETHING fds);

public:
    void SetItemText(int nIndex, const CString& strText);

    BOOL OnBeforeRemoveItem(int iItem);
    void OnAfterAddItem(int nItem);
    void OnAfterRenameItem(int nItem);
    void OnSelectionChanged();

protected:
    DECLARE_MESSAGE_MAP()
};
