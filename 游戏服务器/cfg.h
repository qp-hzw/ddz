#ifndef _CCFG_H_
#define _CCFG_H_

#include <string>
#include <fstream>
#include <winsock2.h>
#define byte BYTE

using std::string;
using std::fstream;

//r&w file
class CCfg
{

public:    
	CCfg(){};  
    ~CCfg(){};

    static CCfg * Instance();

    int  OpenFile(const string& file_name);
	void CloseFile();

public:
    //get long/int/short/byte (u)
    byte GetItemValue(const string& section_name,
                      const string& key_name,
                      long &result);

    //get char*
    byte GetItemValue( const string& section_name,
                       const string& key_name,
                       string& key_value);

	byte GetItemValue(const string& section_name,
						const string& key_name,
						BYTE& result);

	byte GetItemValue(const string& section_name,
						const string& key_name,
						DWORD& result);

    //set long/int/short/byte (u)
    byte SetItemValue( const string& section_name,
                       const string& key_name,
                       const long& key_value);

    //set char *
    byte SetItemValue( const string& section_name,
                       const string& key_name,
                       const string& key_value);
       
private:
    //locate section
    byte LocateSection(const string& section_name, string &section_range);

    //locate key value
    byte LocateKeyValue(const string& key_name,
                        const string& section_range,
                        string& key_value);
        
private:
    fstream m_fstream;
    string  m_content;

    size_t m_section_start_position;
    size_t m_key_val_start_position;

private:
    static CCfg* s_instance;
};

#endif /* _CCFG_H_ */
