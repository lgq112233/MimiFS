#pragma once

class ConsoleApp;

class MiniFolder:public MiniFile
{
	friend ConsoleApp;
	friend MiniFile;
protected:
	MiniFolder();
	void load();
public:
	~MiniFolder();

	void show()override;

	std::string getAbsolutePath();

	void deleteForever()override;

	MiniFile* createChildFile(std::string& filename);
	MiniFolder* createChildFolder(std::string& filename);

	void clear();

	void updateDir();
	bool isLoaded() { return loaded;}
	MiniFile*& atChild(std::string&filename);
	MiniFile*& getChild(std::string&filename);

	MiniFolder* getFolderByRelativePath(std::vector<std::string>&nameList,bool queryCreateNew=true);

private:
	bool loaded;
	std::unordered_map<std::string, MiniFile*>childs;
};

