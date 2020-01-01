/*
 * LogFile.h
 *
 *  Created on: 2013-8-22
 *      Author: zengjf
 */

#ifndef __CONFIGFILE_H__
#define __CONFIGFILE_H__

#include <stdio.h>
#include <winsock2.h>

class CConfigFile
{

public:
    char    *m_pszFilename;     /* �����Ҫ��ȡ�������ļ��� */
	CConfigFile();  
    ~CConfigFile();

    unsigned int IsOpen();
    unsigned int GetItemValue( const char *pszSectionName, 
                        const char *pszKeyName, 
                        char *pszReturnedString, 
                        unsigned int nSize );
    unsigned int SetItemValue( const char *pszSectionName, 
                        const char *pszKeyName, 
                        const char *pszKeyValue );
    unsigned int GetItemValue( const char *pszSectionName, 
                        const char *pszKeyName,
							unsigned int &lReturnedValue );
    unsigned int GetItemValue( const char *pszSectionName,
                        const char *pszKeyName, 
                        int &lReturnedValue );
    unsigned int GetItemValue( const char *pszSectionName,
                               const char *pszKeyName, 
                               BYTE &lReturnedValue );

     unsigned int GetItemValue( const char *pszSectionName, 
                        const char *pszKeyName, 
                        short &lReturnedValue );
    unsigned int GetItemValue( const char *pszSectionName, 
                        const char *pszKeyName, 
                        unsigned short &lReturnedValue );
    unsigned int SetItemValue( const char *pszSectionName, 
                        const char *pszKeyName, 
                        int lKeyValue );
    unsigned int GetItemValue( const char *pszSectionName, 
                        const char *pszKeyName, 
                        int &lReturnedValue,
                        int lDefaultValue );
    unsigned int GetItemValue( const char *pszSectionName, 
                        const char *pszKeyName, 
                        char *pszReturnedString, 
                        unsigned int nSize, 
                        const char *pszDefaultValue);
    unsigned int SetItemValue( const char *pszSectionName,
                        const char *pszKeyName,
                        unsigned long lKeyValue );
    unsigned int GetItemValue( const char *pszSectionName,
                        const char *pszKeyName,
                        unsigned long &lReturnedValue );

    unsigned int GetItemValue( const char *pszSectionName,
                           const char *pszKeyName,
                           float &lReturnedValue );

	int  OpenFile(const char *pszFilename);
    
	void CloseFile();

private:
    
    unsigned int LocateSection(const char *pszSectionName, 
                        char * &pszSectionBegin, 
                        char * &pszSectionEnd);
    unsigned int LocateKeyRange(const char *pszKeyName, 
                        const char *pszSectionBegin, 
                        const char *pszSectionEnd, 
                        char * &pszKeyBegin, 
                        char * &pszKeyEnd);
    unsigned int LocateKeyValue(const char *pszKeyName, 
                        const char *pszSectionBegin, 
                        const char *pszSectionEnd, 
                        char * &pszValueBegin, 
                        char * &pszValueEnd);
    char *LocateStr(    const char *pszCharSet, 
                        const char *pszBegin, 
                        const char *pszEnd );
	char *SearchMarchStr(const char *pszBegin, const char *pszCharSet);

    char *MapToContent(const char *p);
    char *MapToShadow(const char *p);

	void ToLower( char * pszSrc, size_t len);

private:
    char    *m_pszContent;      /* �����ļ���ԭʼ���� */
    char    *m_pszShadow;       /* �����ļ�������ȫ��ת����Сд */
    size_t  m_nSize;            /* �����ļ����ݵĳ��ȣ�����������NULL */
    short   m_bIsOpen;          /* �����ļ��Ƿ�򿪳ɹ��ı�־ */

};

#endif
