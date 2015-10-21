#include "XMLNode.h"

XMLNode::XMLNode(const string &xmlFile)
	: m_parent(NULL)
{
	// correctly format the file name for the platform
	string name(xmlFile);
#ifndef _WIN32
	name = "/app_home/" + name;
#endif

	// read the entire file in binary form into memory
	wstring xmlData;
	FILE *fd = fopen(name.c_str(), "rb");
	fseek(fd, 0, SEEK_END);
	long fileSize = ftell(fd);
	char *stream = (char*)malloc(fileSize);
	fseek(fd, 0, SEEK_SET);
	int bytes_read = fread(stream, 1, fileSize, fd);
	fclose(fd);

#ifndef _WIN32
	// each pair of bytes represents one UTF-16 character
	// these pairs of bytes must be reversed because they are stored in little endian format but the PowerPC chip in the PS3 is big endian
	for (unsigned c = 0; c < bytes_read; c += 2)
	{
		char b = stream[c];
		stream[c] = stream[c + 1];
		stream[c + 1] = b;
	}
#endif

	// store the data in a wstring for ease of manipulation
	xmlData = (wchar_t*)stream;
	
	// unpack the xml into a readable structure in memory
	vector<XMLElement> elements;
	lex(xmlData, elements);
	buildFromElements(elements);
}

XMLNode::XMLNode(const wstring &name, const wstring &value)
	: m_parent(NULL), m_name(name), m_value(value)
{
}

XMLNode::XMLNode(const XMLNode &other)
	: m_name(other.m_name), m_value(other.m_value), m_parent(other.m_parent), m_attributes(other.m_attributes)
{
	// copy children
	for (unsigned c = 0; c < other.m_children.size(); ++c)
		m_children.push_back(new XMLNode(*other.m_children[c]));
}

XMLNode::XMLNode(XMLNode *parent, const vector<XMLElement> &elements)
{
	m_parent = parent;
	buildFromElements(elements);
}

XMLNode::~XMLNode()
{
	for (unsigned int c = 0; c < m_children.size(); ++c)
		delete m_children[c];
}

XMLNode& XMLNode::operator=(const XMLNode &other)
{
	// clean up
	for (unsigned int c = 0; c < m_children.size(); ++c)
		delete m_children[c];

	// assign new attributes
	m_name = other.m_name;
	m_value = other.m_value;
	m_parent = other.m_parent;
	m_attributes = other.m_attributes;

	// copy children
	for (unsigned c = 0; c < other.m_children.size(); ++c)
		m_children.push_back(new XMLNode(*other.m_children[c]));

	return *this;
}

wstring XMLNode::getAttribute(const wstring &attribute)
{
	map<wstring, wstring>::iterator i = m_attributes.find(attribute);
	if (i == m_attributes.end())
		return L"";
	else
		return m_attributes[attribute];
}

vector<XMLNode*> XMLNode::getChildren(const wstring &tag, const wstring &value)
{
	if (tag.size() == 0)
		return m_children;
	vector<XMLNode*> output;
	for (unsigned c = 0; c < m_children.size(); ++c)
	{
		if (m_children[c]->getName() == tag && (value.size() == 0 || m_children[c]->getValue() == value))
			output.push_back(m_children[c]);
	}
	return output;
}

vector<XMLNode*> XMLNode::getNodes(const wstring &tag, const wstring &value)
{
	vector<XMLNode*> output;
	for (unsigned int c = 0; c < m_children.size(); ++c)
	{
		if (m_children[c]->getName() == tag && (value.size() == 0 || m_children[c]->getValue() == value))
			output.push_back(m_children[c]);
		vector<XMLNode*> fromChild = m_children[c]->getNodes(tag);
		for (unsigned int d = 0; d < fromChild.size(); ++d)
			output.push_back(fromChild[d]);
	}
	return output;
}

XMLNode *XMLNode::addChild(const wstring &name, const wstring &value)
{
	m_children.push_back(new XMLNode(name, value));
	return m_children[m_children.size() - 1];
}

void XMLNode::deleteChild(XMLNode *node)
{
	for (unsigned int c = 0; c < m_children.size(); ++c)
	{
		if (m_children[c] == node)
		{
			m_children.erase(m_children.begin() + c);
			break;
		}
	}
}

void XMLNode::save(const wstring &fileName)
{
	ofstream file(fileName.c_str());
	save(file, 0);
	file.close();
}



void XMLNode::save(ofstream &file, int depth)
{
	wstring line = L"";
	for (int c = 0; c < depth; ++c)
		line += L"\t";
	line += L"<" + m_name;

	for (map<wstring, wstring>::iterator i = m_attributes.begin(); i != m_attributes.end(); ++i)
		line += L" " + i->first + L"='" + i->second + L"'";
	if (m_children.size() == 0 && m_value.size() == 0)
	{
		line += L" />";
		file << line.c_str() << '\n';
	}
	else if (m_value != L"")
	{
		line += L">";
		if (m_value != L"")
			line += m_value + L"</" + m_name + L">";
		file << line.c_str() << L'\n';
	}
	else if (m_children.size() > 0)
	{
		line += L">";
		file << line.c_str() << L'\n';
		for (unsigned int c = 0; c < m_children.size(); ++c)
			m_children[c]->save(file, depth + 1);
		line = L"";
		for (int c = 0; c < depth; ++c) line += L"\t";
		line += L"</" + m_name + L">";
		file << line.c_str() << L'\n';
	}
}

wstring XMLNode::decryptXML(wstring xml)
{
	int pos = xml.find(L"&lt;");
	while (pos != wstring::npos)
	{
		xml = xml.replace(pos, 4, L"<");
		pos = xml.find(L"&lt;");
	}
	pos = xml.find(L"&gt;");
	while (pos != wstring::npos)
	{
		xml = xml.replace(pos, 4, L">");
		pos = xml.find(L"&gt;");
	}
	pos = xml.find(L"&amp;");
	while (pos != wstring::npos)
	{
		xml = xml.replace(pos, 4, L"&");
		pos = xml.find(L"&amp;");
	}
	pos = xml.find(L"&apos;");
	while (pos != wstring::npos)
	{
		xml = xml.replace(pos, 4, L"'");
		pos = xml.find(L"&apos;");
	}
	pos = xml.find(L"&quot;");
	while (pos != wstring::npos)
	{
		xml = xml.replace(pos, 4, L"\"");
		pos = xml.find(L"&quot;");
	}
	// note - this is not standard xml but is very useful for inserting new line characters without destroying the indentation
	pos = xml.find(L"&nl;");
	while (pos != wstring::npos)
	{
		xml = xml.replace(pos, 4, L"\n");
		pos = xml.find(L"&nl;");
	}

	return xml;
}

void XMLNode::lex(const wstring &xml, vector<XMLElement> &allElements)
{
	// initialize locals
	short currentType = SMBL_WHITE;
	wstring currentValue = L"";

	// tokenize text
	for (unsigned c = 0; c < xml.length(); ++c)
	{
		if (xml[c] == L' ' || xml[c] == L'\n' || xml[c] == L'\t' || xml[c] == L'\r')
		{
			if (currentType != SMBL_WHITE && currentValue != L"")
			{
				allElements.push_back(XMLElement(currentType, currentValue));
				currentType = SMBL_WHITE;
				currentValue = L"";
			}
			currentValue += xml[c];
		}
		else if (xml[c] == L'<' || xml[c] == L'>' || xml[c] == L'/' || xml[c] == L'?' || xml[c] == L'\'' || xml[c] == L'"' || xml[c] == L'=' || xml[c] == L'!' || xml[c] == L'-')
		{
			bool newElement = currentType != SMBL_SPECIAL || currentValue == L"'" || currentValue == L"\"" || currentValue == L"=" || currentValue == L"</" || currentValue == L"<?" || currentValue == L"/>" || currentValue == L"?>" || currentValue == L"<!--" || currentValue == L"-->";
			if (!newElement)
			{
				newElement = true;
				if (xml[c] == L'>' && (currentValue == L"?" || currentValue == L"/" || currentValue == L"--"))
					newElement = false;
				else if ((xml[c] == L'/' || xml[c] == L'?' || xml[c] == L'!') && currentValue == L"<")
					newElement = false;
				else if (xml[c] == '-' && (currentValue == L"<!" || currentValue == L"<!-" || currentValue == L"-"))
					newElement = false;
			}

			if (newElement)
			{
				if (currentValue != L"")
					allElements.push_back(XMLElement(currentType, currentValue));
				currentType = SMBL_SPECIAL;
				currentValue = xml[c];
			}
			else
				currentValue += xml[c];
		}
		else
		{
			if (currentType != SMBL_TEXT && currentValue != L"")
			{
				allElements.push_back(XMLElement(currentType, currentValue));
				currentType = SMBL_TEXT;
				currentValue = L"";
			}
			currentValue += xml[c];
		}
	}
	if (currentValue != L"")
		allElements.push_back(XMLElement(currentType, currentValue));

	// purge irrelevant elements
	for (unsigned int c = 0; c < allElements.size(); ++c)
	{
		if (allElements[c].text == L"<!--")
		{
			int end = findElement(allElements, c, SMBL_SPECIAL, L"-->");
			if (end == -1)
				utils::errorQuit(L"XML Exception. Unclosed comment tags.");
			allElements.erase(allElements.begin() + c, allElements.begin() + end + 1);
			c--;
		}
		else if (allElements[c].text == L"<?")
		{
			int end = findElement(allElements, c, SMBL_SPECIAL, L"?>");
			if (end == -1)
				utils::errorQuit(L"XML Exception. Unclosed meta tags.");
			allElements.erase(allElements.begin() + c, allElements.begin() + end + 1);
			c--;
		}
	}
}

void XMLNode::buildFromElements(const vector<XMLElement> &elements)
{
	// initialize attributes
	m_name = L"";
	m_value = L"";

	int pos = findElement(elements, 0, SMBL_SPECIAL, L"<");
	if (pos > -1)
	{
		pos++; if ((unsigned)pos >= elements.size()) utils::errorQuit(L"XML Exception. Invalid XML file.");
		if (elements[pos].type == SMBL_TEXT)
			m_name = elements[pos].text;
		else
			utils::errorQuit(L"XML Exception. Invalid root node.");

		while (elements[pos].text != L">" && elements[pos].text != L"/>")
		{
			pos++; if ((unsigned)pos >= elements.size()) utils::errorQuit(L"XML Exception. Invalid XML file.");
			if (elements[pos].type == SMBL_TEXT)
			{
				wstring attName = elements[pos].text;
				pos = findElement(elements, pos + 1, SMBL_SPECIAL, L"=");
				if (pos == -1)
					utils::errorQuit(L"XML Exception. Incomplete attribute.");
				while (elements[pos].text != L"'" && elements[pos].text != L"\"")
					pos++; if ((unsigned)pos >= elements.size()) utils::errorQuit(L"XML Exception. Invalid XML file.");
				wstring openQuote = elements[pos].text;
				wstring attValue = L"";
				pos++; if ((unsigned)pos >= elements.size()) utils::errorQuit(L"XML Exception. Invalid XML file.");
				while (elements[pos].text != openQuote)
				{
					attValue += elements[pos].text;
					pos++; if ((unsigned)pos >= elements.size()) utils::errorQuit(L"XML Exception. Invalid XML file.");
				}
				m_attributes[attName] = decryptXML(attValue);
			}
		}

		if (elements[pos].text == L">")
		{
			
			while (!(elements[pos].text == L"</" && elements[pos + 1].text == m_name))
			{
				if (elements[pos].type == SMBL_TEXT)
				{
					while (!(elements[pos].text == L"</" && elements[pos + 1].text == m_name))
					{
						m_value += elements[pos].text;
						pos++; if ((unsigned)pos + 1 >= elements.size()) utils::errorQuit(L"XML Exception. Invalid XML file.");
					}
					pos--;
				}
				else if (elements[pos].text == L"<")
				{
					int open = pos;
					wstring curName = elements[pos + 1].text;
					while (elements[pos].text != L">" && elements[pos].text != L"/>")
						pos++; if ((unsigned)pos + 1 >= elements.size()) utils::errorQuit(L"XML Exception. Invalid XML file.");
					if (elements[pos].text == L"/>")
						m_children.push_back(new XMLNode(this, vector<XMLElement>(&elements[open], &elements[pos + 1])));
					else
					{
						while (!(elements[pos].text == L"</" && elements[pos + 1].text == curName))
							pos++; if ((unsigned)pos + 1 >= elements.size()) utils::errorQuit(L"XML Exception. Invalid XML file.");
						pos = findElement(elements, pos, SMBL_SPECIAL, L">");
						if (pos > -1)
							m_children.push_back(new XMLNode(this, vector<XMLElement>(&elements[open], &elements[pos + 1])));
						else
							utils::errorQuit(L"XML Exception. Unclosed tag.");
					}
				}
				pos++; if ((unsigned)pos >= elements.size()) utils::errorQuit(L"XML Exception. Invalid XML file.");
			}
		}
	}
	else
		utils::errorQuit(L"XML Exception. No root node found.");

	m_value = decryptXML(m_value);
}

inline int XMLNode::findElement(const vector<XMLElement> &elements, int start, short type, const wstring &exact)
{
	for (unsigned c = start; c < elements.size(); c++)
	{
		if (elements[c].type == type)
		{
			if (exact.size() == 0 || elements[c].text == exact)
				return int(c);
		}
	}
	return -1;
}
