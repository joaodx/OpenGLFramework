#ifndef H_XMLNODE
#define H_XMLNODE

#include <string>
#include <vector>
#include <map>
#include <fstream>
#include "Utilities.h"

using namespace std;

class XMLNode
{
	struct XMLElement
	{
		short type;
		wstring text;
		XMLElement(short newType, wstring newText) : type(newType), text(newText) { }
	};

	map<wstring, wstring> m_attributes;
	vector<XMLNode*> m_children;
	wstring m_name, m_value;
	XMLNode *m_parent;

	enum Symbols { SMBL_TEXT, SMBL_WHITE, SMBL_SPECIAL };

public:
	XMLNode(const string &xmlFile);
	XMLNode(const wstring &name, const wstring &value);
	XMLNode(const XMLNode &other);
	~XMLNode();

	XMLNode &operator=(const XMLNode &other);

	const wstring &getName() { return m_name; }
	wstring getAttribute(const wstring &attribute);
	vector<XMLNode*> getChildren(const wstring &tag = L"", const wstring &value = L"");
	vector<XMLNode*> getNodes(const wstring &tag, const wstring &value = L"");
	const wstring &getValue() { return m_value; }
	XMLNode *getParent() { return m_parent; }

	void setName(const wstring &name) { m_name = name; }
	void setValue(const wstring &value) { m_value = value; }
	void setAttribute(const wstring &name, wstring value) { m_attributes[name] = value; }
	void deleteAttribute(const wstring &name) { m_attributes.erase(name); }
	XMLNode *addChild(const wstring &name, const wstring &value = L"");
	void deleteChild(XMLNode *node);
	void save(const wstring &fileName);

private:
	XMLNode(XMLNode *parent, const vector<XMLElement> &elements);
	void save(ofstream &file, int depth);
	wstring decryptXML(wstring xml);
	void lex(const wstring &xml, vector<XMLElement> &elements);
	void buildFromElements(const vector<XMLElement> &elements);
	int findElement(const vector<XMLElement> &elements, int start, short type, const wstring &exact = L"");
};

#endif
