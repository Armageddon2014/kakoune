#include "option_manager.hh"

#include <sstream>

namespace Kakoune
{

Option& OptionManager::operator[] (const String& name)
{
    auto it = m_options.find(name);
    if (it != m_options.end())
        return it->second;
    else if (m_parent)
        return (*m_parent)[name];
    else
        return m_options[name];
}

const Option& OptionManager::operator[] (const String& name) const
{
    auto it = m_options.find(name);
    if (it != m_options.end())
        return it->second;
    else if (m_parent)
        return (*m_parent)[name];
    else
        throw option_not_found(name);
}

CandidateList OptionManager::complete_option_name(const String& prefix,
                                                  size_t cursor_pos)
{
    String real_prefix = prefix.substr(0, cursor_pos);
    CandidateList result;
    if (m_parent)
        result = m_parent->complete_option_name(prefix, cursor_pos);
    for (auto& option : m_options)
    {
        if (option.first.substr(0, real_prefix.length()) == real_prefix and
            not contains(result, option.first))
            result.push_back(option.first);
    }
    return result;
}

GlobalOptionManager::GlobalOptionManager()
    : OptionManager()
{
    (*this)["tabstop"] = 8;
}

}