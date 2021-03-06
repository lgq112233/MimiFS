#include "stdafx.h"
#include "MiniFileWriter.h"

MiniFileWriter::MiniFileWriter(MiniFile*file)
{
	head.size = 0;
	curBlock = 0;
	head.nextBlockId = file->fileHead.blockId;
}


MiniFileWriter::~MiniFileWriter()
{
	if (curBlock) {

		if (head.nextBlockId) {
			MiniFile::op.releaseBlock(head.nextBlockId);
			head.nextBlockId = 0;
		}

		MiniFile::op.seekBlock(curBlock);
		MiniFile::op.write(head);
	}
}

int MiniFileWriter::getBlockMaxWriteSize() {
	return MiniFile::op.getBlockSize() - sizeof(BlockHead);
}

int MiniFileWriter::writeToBlock(char*bytes, int bytecount) {

	if (bytecount > getBlockMaxWriteSize()) {
		return 0;
	}

	if (!head.nextBlockId) {
		head.nextBlockId= MiniFile::op.requestEmptyBlock();
	}

	if (curBlock) {
		MiniFile::op.seekBlock(curBlock);
		MiniFile::op.write(head);
	}

	curBlock = head.nextBlockId;
	MiniFile::op.seekBlock(curBlock);
	MiniFile::op.read(head);
	MiniFile::op.flush();

	fwrite(bytes, 1, bytecount, MiniFile::op.file);
	head.size = bytecount;

	return bytecount;

}