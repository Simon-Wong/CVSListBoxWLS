// VSListBoxWLS.cpp : 实现文件
//

#include "stdafx.h"
#include "VSListBoxWLS.h"

BOOL FuncDoNothing(LPVOID,LPVOID,CObject*){return TRUE;}
// CVSListBoxWLS

IMPLEMENT_DYNAMIC(CVSListBoxWLS, CVSListBox)

CVSListBoxWLS::CVSListBoxWLS()
{
    m_bEnableRepeatText=FALSE;
    m_bDoAfterAddItemWhenever=FALSE;
    m_bDoAfterRenameWhenever=FALSE;
    m_bAddItem=FALSE;

    m_bCheckTextLen=FALSE;
    m_nTextLen=10;

    m_fdsBeforeRemoveItem=FuncDoNothing;
    m_fdsAfterAddItemNoRepeat=FuncDoNothing;
    m_fdsAfterAddItemWhenever=FuncDoNothing;
    m_fdsAfterRenameItemNoRepeat=FuncDoNothing;
    m_fdsAfterRenameItemWhenever=FuncDoNothing;

    m_strWhenSelecting=TEXT("");
    m_strAfterOp=TEXT("");

    m_pObjforOperatingFunc=NULL;
}

CVSListBoxWLS::~CVSListBoxWLS()
{
}

BEGIN_MESSAGE_MAP(CVSListBoxWLS, CVSListBox)
END_MESSAGE_MAP()

void CVSListBoxWLS::SetTextLegalLength(int nLen/*=10*/)
{
    m_nTextLen=nLen;
}

void CVSListBoxWLS::EnableDoAfterRenameWhenever(BOOL bRename)
{
    m_bDoAfterRenameWhenever=bRename;
}

void CVSListBoxWLS::EnableRepeatText(BOOL bRepeat)
{
    m_bEnableRepeatText=bRepeat;
}

void CVSListBoxWLS::EnableDoAfterAddItemWhenever(BOOL bRepeat)
{
    m_bDoAfterAddItemWhenever=bRepeat;
}

void CVSListBoxWLS::EnableCheckTextLen(BOOL bChk/*=FALSE*/)
{
    m_bCheckTextLen=bChk;
}

void CVSListBoxWLS::OnSelectionChanged()
{
    m_strWhenSelecting=GetItemText(GetSelItem());
    OutputDebugString(TEXT("[")+m_strWhenSelecting+TEXT("]\n"));
}

void CVSListBoxWLS::SetItemText(int nIndex, const CString& strText)
{    
    CString strPrompt;
    if (m_bEnableRepeatText==FALSE && IsExistText(strText))
    {
        strPrompt.Format(TEXT("【%s】已存在"),strText);
        MessageBox(strPrompt,TEXT("提示"),MB_ICONINFORMATION|MB_OK);

        m_bAddItem=FALSE;

        return;
    }

    if (strText==TEXT(""))
    {
        strPrompt.Format(TEXT("未输入内容"),strText);
        MessageBox(strPrompt,TEXT("提示"),MB_ICONINFORMATION|MB_OK);

        m_bAddItem=FALSE;
    }

    if (m_bCheckTextLen==TRUE && strText.GetLength()>m_nTextLen)
    {
        strPrompt.Format(TEXT("请输入%d个以内的字符"),m_nTextLen);
        MessageBox(strPrompt,TEXT("提示"),MB_ICONINFORMATION|MB_OK);

        m_bAddItem=FALSE;

        return;
    }

    m_bAddItem=TRUE;

    CVSListBox::SetItemText(nIndex,strText);
}

BOOL CVSListBoxWLS::OnBeforeRemoveItem(int nItem)
{
    GetItemTextAfterOp(nItem);

    CString strPrompt;
    strPrompt.Format(TEXT("确定删除【%s】吗？"),m_strAfterOp);
    if (MessageBox(strPrompt,TEXT("提示"),MB_ICONQUESTION|MB_OKCANCEL)==IDOK)
    {    
        //by wls op
        m_fdsBeforeRemoveItem((LPVOID)&m_strWhenSelecting,(LPVOID)&m_strAfterOp,m_pObjforOperatingFunc);

        return TRUE;
    }

    return FALSE;
}

CString CVSListBoxWLS::GetTextWhenSelecting()
{
    return m_strWhenSelecting;
}

void CVSListBoxWLS::GetItemTextAfterOp(int nItem)
{
    m_strAfterOp=GetItemText(nItem);
}

void CVSListBoxWLS::OnAfterAddItem(int nItem)
{
    GetItemTextAfterOp(nItem);

    if (m_bDoAfterAddItemWhenever==FALSE && m_bAddItem==TRUE)
    {
        OutputDebugString(TEXT("不重复 成功\n"));
        //by wls op
        m_fdsAfterAddItemNoRepeat((LPVOID)&m_strWhenSelecting,(LPVOID)&m_strAfterOp,m_pObjforOperatingFunc);

        return;
    }
    else
    if (m_bDoAfterAddItemWhenever==TRUE /*&& m_bAddItem==TRUE*/)
    {
        OutputDebugString(TEXT("重复+不重复 成功\n"));
        //by wls op
        m_fdsAfterAddItemWhenever((LPVOID)&m_strWhenSelecting,(LPVOID)&m_strAfterOp,m_pObjforOperatingFunc);
    }

    if (m_strAfterOp==TEXT(""))
    {
        RemoveItem(nItem);//by wls Remove the item with no text
    }
}

void CVSListBoxWLS::OnAfterRenameItem(int nItem)
{
    GetItemTextAfterOp(nItem);

    if (m_bDoAfterRenameWhenever==FALSE && m_bAddItem==TRUE)
    {
        OutputDebugString(TEXT("重命 不重复 值\n"));
        //by wls op
        m_fdsAfterRenameItemNoRepeat((LPVOID)&m_strWhenSelecting,(LPVOID)&m_strAfterOp,m_pObjforOperatingFunc);

        return;
    }
    else
    if (m_bDoAfterAddItemWhenever==TRUE)
    {
        //by wls op
        m_fdsAfterRenameItemWhenever((LPVOID)&m_strWhenSelecting,(LPVOID)&m_strAfterOp,m_pObjforOperatingFunc);
    }
}

BOOL CVSListBoxWLS::IsExistText(CString strText)
{
    for (int i=0;i<GetCount();i++)
    {
        if (strText==GetItemText(i))
        {
            return TRUE;
        }
    }

    return FALSE;
}

void CVSListBoxWLS::SetObjforOperation(CObject* obj)
{
    m_pObjforOperatingFunc=obj;
}

void CVSListBoxWLS::SetOperationBeforeRemoveItem(FUNCDOSOMETHING fds)
{
    m_fdsBeforeRemoveItem=fds;
}
void CVSListBoxWLS::SetOperationAfterAddItemNoRepeat(FUNCDOSOMETHING fds)
{
    m_fdsAfterAddItemNoRepeat=fds;
}
void CVSListBoxWLS::SetOperationAfterAddItemWhenever(FUNCDOSOMETHING fds)
{
    m_fdsAfterAddItemWhenever=fds;
}
void CVSListBoxWLS::SetOperationAfterRenameItemNoRepeat(FUNCDOSOMETHING fds)
{
    m_fdsAfterRenameItemNoRepeat=fds;
}
void CVSListBoxWLS::SetOperationAfterRenameItemWhenever(FUNCDOSOMETHING fds)
{
    m_fdsAfterRenameItemWhenever=fds;
}
