/*
 *  Multi2Sim
 *  Copyright (C) 2013  Rafael Ubal (ubal@ece.neu.edu)
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#include <cassert>
#include <fstream>
#include <sstream>

#include "IniFile.h"
#include "Misc.h"


using namespace Misc;
using namespace std;


/* Return a section and variable name from a string "<section>\n<var>" or
 * "<section>". In the latter case, the variable name is an empty string. */
void IniFile::ItemToSectionVar(string item, string& section, string& var)
{
	vector<string> tokens;

	/* Extract tokens */
	StringTokenize(item, tokens, "|");
	assert(tokens.size() == 1 || tokens.size() == 2);
	section = tokens[0];
	var = tokens.size() == 2 ? tokens[1] : "";
}


/* Create string "<section>\n<var>" from separate strings.
 * String "<section>" is created if 'var' is an empty string.
 * Remove any spaces on the left or right of both the section and variable
 * names. */
string IniFile::SectionVarToItem(string section, string var)
{
	string item;

	/* Trim section and variable strings */
	StringSingleSpaces(section);
	StringTrim(var);

	/* Check section name */
	if (!section.length())
		panic("%s: invalid section name", __FUNCTION__);

	/* Create return string */
	item = section;
	if (var.length())
		item += '|' + var;
	
	/* Return */
	return item;
}


/* Given a string in the format "<var>=<value>", return a string containing the
 * variable and another string containing the value. If the input string format
 * is invalid, the function returns false. True for all ok. */
bool IniFile::GetVarValue(string& s, string& var, string& value)
{
	vector<string> tokens;

	/* Split string */
	StringTokenize(s, tokens, "=");
	if (tokens.size() != 2)
		return false;

	/* Assign output strings */
	var = tokens[0];
	value = tokens[1];
	StringTrim(var);
	StringTrim(value);
	return true;
}


/* Insert a new section into the 'item_table' hash table. If the section already
 * exists, return false. Return true on success. */
bool IniFile::InsertSection(string section)
{
	bool exists;

	/* Get sections */
	StringSingleSpaces(section);
	exists = items.count(section);

	/* Add section */
	items[section] = "";
	if (!exists)
		sections.push_back(section);

	/* Return value */
	return !exists;
}


/* Insert a variable and its value into the 'items' hash table.
 * If 'var' already existed, the previous value is replaced by the
 * new value, returning false. If 'var' did not exist, return true. */
bool IniFile::InsertVariable(string section, string var, string value)
{
	string item;
	bool exists;

	/* Combine section and variable */
	item = SectionVarToItem(section, var);

	/* Set new value */
	exists = items.count(item);
	items[item] = value;
	return !exists;
}


IniFile::IniFile()
{
	path = "<IniFile>";
}


IniFile::IniFile(const std::string &path)
{
	Load(path);
}


void IniFile::Dump(std::ostream& os)
{
	string tmp_section;
	string section;
	string item;
	string var;
	string value;

	for (auto it = sections.begin(); it != sections.end(); ++it)
	{
		section = *it;
		os << "[ " << section << " ]\n";
		for (auto it2 = items.begin(); it2 != items.end(); ++it2)
		{
			item = it2->first;
			ItemToSectionVar(item, tmp_section, var);
			if (!strcasecmp(section.c_str(), tmp_section.c_str())
					&& item.length() > section.length()
					&& item[section.length()] == '|')
				os << var << " = " << it2->second << '\n';
		}
		os << '\n';
	}
}


static const char *ini_file_err_format =
	"\tA syntax error was detected while parsing a configuration INI file.\n"
	"\tThese files are formed of sections in brackets (e.g. '[ SectionName ]')\n"
	"\tfollowed by pairs 'VariableName = Value'. Comments preceded with ';'\n"
	"\tor '#' characters can be used, as well as blank lines. Please verify\n"
	"\tthe integrity of your input file and retry.\n";


void IniFile::Load(const std::string &path)
{
	string line;
	string section;
	string var;
	string value;

	int line_num;
	bool ok;

	/* Open file */
	this->path = path;
	ifstream f(path);
	if (!f)
		fatal("%s: cannot read from file", path.c_str());
	
	/* Read lines */
	line_num = 0;
	section = "";
	while (getline(f, line))
	{
		/* One more line */
		line_num++;

		/* Comment or blank line */
		StringTrim(line);
		if (!line.length() || line[0] == ';' || line[0] == '#')
			continue;
		
		/* New "[ <section> ]" entry */
		if (line[0] == '[' && line[line.length() - 1] == ']')
		{
			/* Get section name */
			section = line.substr(1, line.length() - 2);
			StringSingleSpaces(section);

			/* Insert section */
			ok = InsertSection(section);
			if (!ok)
				fatal("%s: line %d: duplicated section '%s'.\n%s",
					path.c_str(), line_num,
					section.c_str(), ini_file_err_format);

			/* Done for this line */
			continue;
		}

		/* Check that there is an active section */
		if (section == "")
			fatal("%s: line %d: section name expected.\n%s",
				path.c_str(), line_num, ini_file_err_format);
		
		/* New "<var> = <value>" entry. */
		ok = GetVarValue(line, var, value);
		if (!ok)
			fatal("%s: line %d: invalid format.\n%s",
				path.c_str(), line_num, ini_file_err_format);

		/* New variable */
		ok = InsertVariable(section, var, value);
		if (!ok)
			fatal("%s: line %d: duplicated variable '%s'.\n%s",
				path.c_str(), line_num, var.c_str(),
				ini_file_err_format);
	}
	
	/* End */
	f.close();
}


void IniFile::Save(const std::string &path)
{
	/* Open file */
	ofstream f(path);
	if (!f)
		fatal("%s: cannot write to file", path.c_str());
	
	/* Dump */
	Dump(f);
	f.close();
}


bool IniFile::Exists(string section)
{
	StringSingleSpaces(section);
	return items.count(section);
}


bool IniFile::Exists(string section, string var)
{
	string item = SectionVarToItem(section, var);
	return items.count(item);
}


bool IniFile::Remove(string section)
{
	string item;

	/* Remove spaces */
	StringSingleSpaces(section);

	/* Remove section and variables */
	for (auto it = items.begin(); it != items.end(); )
	{
		/* Item is section name */
		item = it->first;
		if (!strcasecmp(item.c_str(), section.c_str()) ||
				(!strncasecmp(item.c_str(), section.c_str(), section.length())
				&& item[section.length()] == '|'))
		{
			it = items.erase(it);
			continue;
		}

		/* Next element */
		++it;
	}

	/* Remove section from list */
	for (auto it = sections.begin(); it != sections.end(); ++it)
	{
		if (!strcasecmp(it->c_str(), section.c_str()))
		{
			sections.erase(it);
			return true;
		}
	}

	/* Section not found */
	return false;
}


bool IniFile::Remove(string section, string var)
{
	string item = SectionVarToItem(section, var);
	return items.erase(item);
}


void IniFile::WriteString(string section, string var, string value)
{
	string item;

	/* Create item, section, and variable strings */
	item = SectionVarToItem(section, var);
	
	/* Insert section and variable into table of allowed items */
	allowed_items.insert(section);
	allowed_items.insert(item);

	/* Write value */
	InsertSection(section);
	InsertVariable(section, var, value);
}


void IniFile::WriteInt(string section, string var, int value)
{
	char buffer[100];
	
	snprintf(buffer, sizeof buffer, "%d", value);
	WriteString(section, var, buffer);
}


void IniFile::WriteInt64(string section, string var, long long value)
{
	char buffer[100];
	
	snprintf(buffer, sizeof buffer, "%lld", value);
	WriteString(section, var, buffer);
}


void IniFile::WriteBool(string section, string var, bool value)
{
	char buffer[100];
	
	snprintf(buffer, sizeof buffer, "%s", value ? "True" : "False");
	WriteString(section, var, buffer);
}


void IniFile::WriteDouble(string section, string var, double value)
{
	char buffer[100];
	
	snprintf(buffer, sizeof buffer, "%f", value);
	WriteString(section, var, buffer);
}


void IniFile::WriteEnum(string section, string var, int value, StringMap map)
{
	string s;
	bool error;

	/* Translate value */
	s = Misc::StringMapValue(map, value, error);
	if (error)
		fatal("%s: invalid value for enumeration (%d)",
				__FUNCTION__, value);
	
	/* Write */
	WriteString(section, var, s);
}


void IniFile::WritePointer(string section, string var, void *value)
{
	char buffer[100];
	
	snprintf(buffer, sizeof buffer, "%p", value);
	WriteString(section, var, buffer);
}


string IniFile::ReadString(string section, string var, string def)
{
	Allow(section, var);
	string item = SectionVarToItem(section, var);
	auto it = items.find(item);
	return it == items.end() ? def : it->second;
}


int IniFile::ReadInt(string section, string var, int def)
{
	string value;
	StringError error;
	int result;

	/* Obtain value */
	value = ReadString(section, var);
	if (value == "")
		return def;

	/* Interpret */
	result = StringToInt(value, error);
	if (error)
		fatal("%s: section [%s], variable '%s', value '%s': %s\n",
				path.c_str(), section.c_str(), var.c_str(),
				value.c_str(), StringGetErrorString(error));

	/* Return */
	return result;
}


long long IniFile::ReadInt64(string section, string var, long long def)
{
	string value;
	StringError error;
	long long result;

	/* Obtain value */
	value = ReadString(section, var);
	if (value == "")
		return def;

	/* Interpret */
	result = StringToInt64(value, error);
	if (error)
		fatal("%s: section [%s], variable '%s', value '%s': %s\n",
				path.c_str(), section.c_str(), var.c_str(),
				value.c_str(), StringGetErrorString(error));

	/* Return */
	return result;
}


bool IniFile::ReadBool(string section, string var, bool def)
{
	string s;

	/* Read variable */
	s = ReadString(section, var);
	if (s == "")
		return def;

	/* True */
	if (!strcasecmp(s.c_str(), "t") || !strcasecmp(s.c_str(), "True")
			|| !strcasecmp(s.c_str(), "On"))
		return true;
	
	/* False */
	if (!strcasecmp(s.c_str(), "f") || !strcasecmp(s.c_str(), "False")
			|| !strcasecmp(s.c_str(), "Off"))
		return false;

	/* Invalid value */
	fatal("%s: section [%s], variable '%s', invalid value '%s'\n"
			"\tPossible values are {t|True|On|f|False|Off}\n",
			path.c_str(), section.c_str(), var.c_str(), s.c_str());
	return false;
}


double IniFile::ReadDouble(string section, string var, double def)
{
	istringstream ss;
	string s;
	double value;

	/* Read value */
	s = ReadString(section, var);
	if (s == "")
		return def;

	/* Convert */
	ss.str(s);
	ss >> value;
	if (!ss || !ss.eof())
		fatal("%s: section [%s], variable '%s', invalid double value '%s'\n",
				path.c_str(), section.c_str(), var.c_str(), s.c_str());
	
	/* Convert */
	return value;
}


int IniFile::ReadEnum(string section, string var, StringMap map, int def)
{
	string s;

	int value;
	bool error;

	/* Read value */
	s = ReadString(section, var);
	if (s == "")
		return def;
	
	/* Convert */
	value = StringMapStringCase(map, s.c_str(), error);
	if (!error)
		return value;

	/* Error, show options */
	fatal("%s: section [%s], variable '%s', invalid value '%s'\n"
			"\tPossible values are %s",
			path.c_str(), section.c_str(), var.c_str(), s.c_str(),
			StringMapGetValues(map).c_str());
	return 0;
}


void *IniFile::ReadPointer(string section, string var, void *def)
{
	string s;
	void *value;

	/* Read value */
	s = ReadString(section, var);
	if (s == "")
		return def;
	
	/* Convert */
	sscanf(s.c_str(), "%p", &value);
	return value;
}


void IniFile::Allow(string section)
{
	StringSingleSpaces(section);
	allowed_items.insert(section);
}


void IniFile::Allow(string section, string var)
{
	Allow(section);
	string item = SectionVarToItem(section, var);
	allowed_items.insert(item);
}


void IniFile::Enforce(string section)
{
	StringSingleSpaces(section);
	allowed_items.insert(section);
	enforced_items.insert(section);
}


void IniFile::Enforce(string section, string var)
{
	Enforce(section);
	string item = SectionVarToItem(section, var);
	enforced_items.insert(item);
	allowed_items.insert(item);
}


void IniFile::Check()
{
	string item;
	string section;
	string var;

	/* Check that all mandatory items are present */
	for (auto it = enforced_items.begin(); it != enforced_items.end(); ++it)
	{
		/* Item is present */
		item = *it;
		if (items.count(item))
			continue;

		/* Item not present */
		ItemToSectionVar(item, section, var);
		if (var == "")
			fatal("%s: section [%s] missing",
				path.c_str(), section.c_str());
		else
			fatal("%s: section [%s], variable '%s' missing",
				path.c_str(), section.c_str(), var.c_str());
	}

	/* Check that all present items are allowed */
	for (auto it = items.begin(); it != items.end(); ++it)
	{
		/* Item is allowed */
		item = it->first;
		if (allowed_items.count(item))
			continue;

		/* Item not allowed */
		ItemToSectionVar(item, section, var);
		if (var == "")
			fatal("%s: invalid section [%s]",
				path.c_str(), section.c_str());
		else
			fatal("%s: section [%s], invalid variable '%s'",
				path.c_str(), section.c_str(), var.c_str());
	}
}
