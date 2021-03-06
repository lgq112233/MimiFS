#include "stdafx.h"
#include "MiniFolder.h"


MiniFolder::MiniFolder()
{
	fileHead.type = FS_FILETYPE_FOLDER;
	loaded = false;
}


MiniFolder::~MiniFolder()
{
	clear();
}

void MiniFolder::show() {

	using namespace std;

	for (auto i : childs) {

		if (i.second) {
			if (i.second->isFolder()) {
				cout << "/" << i.first << endl;
			}
			else {
				cout << i.first << "     " << i.second->fileHead.size << "byte" << endl;
			}
		}

	}

}

void MiniFolder::clear() {

	for (auto i : childs) {
		delete i.second;
	}

	childs.clear();

}

std::string MiniFolder::getAbsolutePath() {

	if (parent) {
		return  parent->getAbsolutePath() + "/" + fileHead.filename;
	}

	return  "root:/";

}

MiniFile* MiniFolder::createChildFile(std::string& filename) {
	auto f = new MiniFile();
	f->setParent(this);
	childs[filename] = f;
	f->fileHead.filename = filename;
	f->fileHead.blockId = op.requestEmptyBlock();
	return f;
}

MiniFolder* MiniFolder::createChildFolder(std::string& filename) {
	auto f = new MiniFolder();
	f->setParent(this);
	childs[filename] = f;
	f->loaded = true;
	f->fileHead.filename = filename;
	f->fileHead.blockId = op.requestEmptyBlock();
	return f;
}

MiniFile*& MiniFolder::atChild(std::string&filename) {

	return childs.at(filename);

}

MiniFile*& MiniFolder::getChild(std::string&fileName) {

	return childs[fileName];

}

void MiniFolder::updateDir() {

	int remainSize = 0;

	BlockHead blockHead;

	op.seekBlock(fileHead.blockId);
	op.read(blockHead);
	op.flush();
	blockHead.size = 0;
	remainSize = op.getBlockSize() - sizeof(BlockHead);

	for (auto& i : childs) {

		if (i.second == nullptr)
			continue;

		auto size = i.second->computeHeadSize();

		if (size > remainSize) {

			if (!blockHead.nextBlockId)
				blockHead.nextBlockId = op.requestEmptyBlock();

			op.reseekCurBlock();
			op.write(blockHead);

			op.seekBlock(blockHead.nextBlockId);
			op.read(blockHead);
			op.flush();

			blockHead.size = 0;
			remainSize = op.getBlockSize() - sizeof(BlockHead);
		}

		remainSize -= size;

		op.write<MiniFileHead&>(i.second->fileHead);
		blockHead.size++;

	}

	if (blockHead.nextBlockId)
		op.releaseBlock(blockHead.nextBlockId);

	blockHead.nextBlockId = 0;
	op.reseekCurBlock();
	op.write(blockHead);
	op.flush();

}

void MiniFolder::deleteForever() {

	if (!loaded) {
		load();
	}

	for (auto i : childs) {

		if (i.second) {
			i.second->deleteForever();
			delete i.second;
		}

	}

	op.releaseBlock(fileHead.blockId);

}


void MiniFolder::load() {

	BlockHead blockHead;
	blockHead.nextBlockId = fileHead.blockId;

	do {
		op.seekBlock(blockHead.nextBlockId);
		op.read(blockHead);

		for (int i = 0; i < blockHead.size; i++) {
			MiniFileHead filehead;
			op.read(filehead);
			auto file = MiniFile::fromFileHead(filehead);
			file->setParent(this);
			childs[filehead.filename] = file;

		}
	} while (blockHead.nextBlockId);

}
