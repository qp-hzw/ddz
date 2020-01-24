#ifndef _CCFG_H_
#define _CCFG_H_

#include "ServiceCoreHead.h"
#include <string>
#include <fstream>

using std::string;
using std::fstream;

//r&w file
class SERVICE_CORE_CLASS CWHCfg
{

public:    
	CWHCfg(){};  
    ~CWHCfg(){};

    static CWHCfg * Instance();

    int  OpenFile(const string& file_name);
	void CloseFile();

public:
    //get long/int/short/byte (u)
    BYTE GetItemValue(const string& section_name,
                      const string& key_name,
                      long &result);

    //get char*
    BYTE GetItemValue( const string& section_name,
                       const string& key_name,
                       string& key_value);

	//get byte
	byte GetItemValue(const string& section_name,
						const string& key_name,
						BYTE& result);

	byte GetItemValue(const string& section_name,
						const string& key_name,
						DWORD& result);

    //set long/int/short/byte (u)
    BYTE SetItemValue( const string& section_name,
                       const string& key_name,
                       const long& key_value);

    //set char *
    BYTE SetItemValue( const string& section_name,
                       const string& key_name,
                       const string& key_value);
       
private:
    //locate section
    BYTE LocateSection(const string& section_name, string &section_range);

    //locate key value
    BYTE LocateKeyValue(const string& key_name,
                        const string& section_range,
                        string& key_value);
        
private:
    fstream m_fstream;
    string  m_content;

    size_t m_section_start_position;
    size_t m_key_val_start_position;

private:
    static CWHCfg* s_instance;
};

#endif /* _CCFG_H_ */
