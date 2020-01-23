#include <sstream>
#include <iostream>
#include "cfg.h"

using std::cout;
using std::endl;
using std::unitbuf;
using std::stringstream;

CCfg* CCfg::s_instance = NULL;

CCfg * CCfg::Instance()
{
    if(s_instance == NULL)
    {
        s_instance = new CCfg();
    }

    return s_instance;
}
int CCfg::OpenFile(const string& file_name)
{
    //reset buffers
	//cout << unitbuf;
    
    //open file
    m_fstream.open(file_name, fstream::out|fstream::in);
    if(!m_fstream)
    {
        return -1;
    }

    //read file
    string temp;
    while(!m_fstream.eof()) //keep m_fstream alive
    {
        std::getline(m_fstream,  temp);
        m_content += temp + '\n';
    }

    //erase last '\n'
    m_content.erase(m_content.length()-1, 1);

    m_fstream.clear(); //when eof, reset

    if(!m_fstream)
    {
        return -2;
    }

    if(!m_fstream.is_open())
    {
        return -3;
    }
        
	return 0;
}
    
void CCfg::CloseFile()
{
    m_fstream.seekg(0);
    m_fstream << m_content; //TODOLATER  change it

    //m_content 重置
    m_content = "";

    m_fstream.close();
}

//get long/int/short/byte (u)
byte CCfg::GetItemValue( const string& section_name,
                                const string& key_name,
                                long &result)
{
    string str_result;
    byte ret = GetItemValue(section_name, key_name, str_result);
    if (ret != 0)
    {
        return ret;
    }
    
    result = (unsigned long)atol(str_result.c_str());
    return 0;
}

byte CCfg::GetItemValue(const string& section_name,
	const string& key_name,
	BYTE &result)
{
	string str_result;
	byte ret = GetItemValue(section_name, key_name, str_result);
	if (ret != 0)
	{
		return ret;
	}

	result = (BYTE)atol(str_result.c_str());
	return 0;
}

byte CCfg::GetItemValue(const string& section_name,
	const string& key_name,
	DWORD &result)
{
	string str_result;
	byte ret = GetItemValue(section_name, key_name, str_result);
	if (ret != 0)
	{
		return ret;
	}

	result = (DWORD)atol(str_result.c_str());
	return 0;
}

//get char*
byte CCfg::GetItemValue(const string& section_name,
                               const string& key_name,
                               string& key_value)
{
    //check parameter
    if(!m_fstream.is_open())
    {
        return -1;
    }
    
    if ((section_name.empty())
        || (key_name.empty()))
    {
        return -2;
    }

    //locate section
    string section_range;
    byte ret = LocateSection(section_name, section_range);
    if (ret != 0)
    {
        return ret - 50;
    }

    //locate key
    ret = LocateKeyValue( key_name,
                          section_range,
                          key_value);
    if (ret != 0)
    {
        return ret - 100;
    }

    if(key_value.empty())
    {
        return -120;
    }

    return 0;
}

//set long/int/short/byte (u)
byte CCfg::SetItemValue( const string& section_name,
                                const string& key_name,
                                const long& key_value)
{
    stringstream ss;
    string str_key_value;
    ss << key_value;
    ss >> str_key_value;
    return SetItemValue(section_name, key_name, str_key_value);
}

//set char*
byte CCfg::SetItemValue(const string& section_name,
                               const string& key_name,
                               const string &value)
{
    //check parameter
    if(!m_fstream.is_open())
    {
        return -1;
    }

    if(section_name.empty() ||
       key_name.empty() ||
       value.empty())
    {
        return -2;
    }

    //locate section_name
    string section_range;
    byte ret = LocateSection(section_name, section_range);
    if (ret != 0)
    {
        return ret - 10;
    }

    //locate key
    string key_value;
    ret = LocateKeyValue( key_name,
                          section_range,
                          key_value);
    if (ret != 0)
    {
        return ret - 20;
    }

    //write to m_content
    m_content.erase(m_section_start_position  +  m_key_val_start_position, key_value.length());
    m_content.insert(m_section_start_position +  m_key_val_start_position, value);
    
    if(!m_fstream)
    {
        return -31;
    }

    if(!m_fstream.is_open())
    {
        return -32;
    }
    
    return 0;
}

byte CCfg::LocateSection(const string& section_name, string &section_range)
{
    if (section_name.empty())
    {
        return -1;
    }

    //structure "[lower_selection_name]"
    string lower_section_name(section_name);
    lower_section_name = "[" + lower_section_name + "]";
    
    //find sub position
    size_t start_position = m_content.find(lower_section_name);
    if(start_position == string::npos)
    {
        return -3;
    }    

    //find last '[' postion
    size_t end_position = m_content.find('[', start_position+section_name.length());
    if(end_position == string::npos)
    {
        end_position = m_content.length();
    }
    
    //stucture section_range
    section_range = m_content.substr(start_position, (end_position-start_position));

    m_section_start_position = start_position;
    
    return 0;
}

byte CCfg::LocateKeyValue(const string& key_name,
                                 const string& section_range,
                                 string& key_value)
{
    //check parameter
    if(key_name.empty() ||
       section_range.empty())
    {
        return -1;
    }

    //find key_name positon
    size_t start_position = section_range.find(key_name);
    if(start_position == string::npos)
    {
        return -2;
    }

    size_t key_value_start_pos = section_range.find('=', start_position + key_name.length());
    if(key_value_start_pos == string::npos)
    {
        return -3;
    }

    //+1 --> overlook '=';  overlook  ' '
    key_value_start_pos = section_range.find_first_not_of("\x20", key_value_start_pos+1);
    if(key_value_start_pos == string::npos)
    {
        return -4;
    }
    
    size_t key_value_end_pos = section_range.find_first_of("\x20 \\ \n ;", key_value_start_pos);
    if(key_value_end_pos == string::npos)
    {
        key_value_end_pos = section_range.length();
    }
    
    key_value = section_range.substr(key_value_start_pos, key_value_end_pos-key_value_start_pos);

    m_key_val_start_position = key_value_start_pos;

    return 0;
}
