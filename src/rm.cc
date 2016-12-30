#include "rm.h"
#include "rm_error.h"
#include <cstring>
#include <math.h>

using namespace std;

namespace {
  const int kBitsInByte = 8;
}

Bitmap::Bitmap(int numBits) {
  CHECK(numBits > 0);
  size = numBits;
  bitArray = new char[size/sizeof(char)+1];
  memset(bitArray, 0, sizeof(char) * (size/sizeof(char)+1));
}

Bitmap::~Bitmap() {
  delete[] bitArray;
}

bool Bitmap::Test(int bitNumber) {
  return ( (bitArray[bitNumber/sizeof(char)] & (1 << (bitNumber%sizeof(char)) )) != 0 );
}

void Bitmap::Set(int bitNumber, bool value) {
  if (value) {
    bitArray[bitNumber/sizeof(char)] |= 1 << (bitNumber%sizeof(char));
  } else {
    bitArray[bitNumber/sizeof(char)] &= ~(1 << (bitNumber%sizeof(char)));
  }
}

int Bitmap::GetSize() {
  return size;
}

int Bitmap::GetSizeInBytes() {
  return static_cast<int>(ceil(static_cast<float>(size)/kBitsInByte));
}

char*& Bitmap::GetBitArray() {
  return this->bitArray;
}

void RM_FileHdr::serialize(char* data) {
  int cursor = 0;
  memcpy(data + cursor, &record_num_per_page, sizeof(record_num_per_page));
  cursor += sizeof(record_num_per_page);
  memcpy(data + cursor, &record_size, sizeof(record_size));
  cursor += sizeof(record_size);
  memcpy(data + cursor, &first_free_page_num, sizeof(first_free_page_num));
}

RM_FileHdr RM_FileHdr::deserialize(char* data) {
  RM_FileHdr hdr;
  int cursor = 0;
  memcpy(&hdr.record_num_per_page, data + cursor, sizeof(hdr.record_num_per_page));
  cursor += sizeof(hdr.record_num_per_page);
  memcpy(&hdr.record_size, data + cursor, sizeof(hdr.record_size));
  cursor += sizeof(hdr.record_size);
  memcpy(&hdr.first_free_page_num, data + cursor, sizeof(hdr.first_free_page_num));
  return hdr;
}

RM_PageHdr::RM_PageHdr(int numBits) {
  numSlots = numBits;
  numFreeSlots = numBits;
  availableSlotMap = new Bitmap(numSlots);
  nextFree = 0;
}

RM_PageHdr::~RM_PageHdr() {
    delete availableSlotMap;
}

int RM_PageHdr::size() {
  int total_size =
  // numslots size.
  sizeof(int) + 
  // numFreeslots size.
  sizeof(int) +
  // nextFree size.
  sizeof(int) +
  // bitmap size.
  this->availableSlotMap->GetSizeInBytes();
  return total_size;
}

void RM_PageHdr::serialize(char* data) {
  int cursor = 0;
  memcpy(data + cursor, &numSlots, sizeof(numSlots));
  cursor += sizeof(numSlots);
  memcpy(data + cursor, &numFreeSlots, sizeof(numFreeSlots));
  cursor += sizeof(numFreeSlots);
  memcpy(data + cursor, &nextFree, sizeof(nextFree));
  cursor += sizeof(nextFree);
  memcpy(data + cursor,
         this->availableSlotMap->GetBitArray(),
         this->availableSlotMap->GetSizeInBytes());
}

RM_PageHdr RM_PageHdr::deserialize(char* data, int numSlots) {
  RM_PageHdr hdr(numSlots);
  int cursor = 0;
  memcpy(&hdr.numSlots, data, sizeof(hdr.numSlots));
  cursor += sizeof(hdr.numSlots);
  memcpy(&hdr.numFreeSlots, data + cursor, sizeof(hdr.numFreeSlots));
  cursor += sizeof(hdr.numFreeSlots);
  memcpy(&hdr.nextFree, data + cursor, sizeof(hdr.nextFree));
  cursor += sizeof(hdr.nextFree);
  memcpy(hdr.availableSlotMap->GetBitArray(), data + cursor,
         hdr.availableSlotMap->GetSizeInBytes());
  return hdr;
}

RM_Record::RM_Record() {
}

void RM_Record::Set(const char* data, const int record_size, const RID rid) {
  this->rid = rid;
  data_ = new char[record_size];
  memcpy(data_, data, record_size);
  this->length = record_size;
}

RM_Record::~RM_Record() {
  delete[] data_;
}

RC RM_Record::GetData(char *& pData) const {
  if (data_ == NULL || length <= 0) {
      return RM_NULLRECORD;
  }
  memcpy(pData, data_, length);
  return OK_RC;
}

RC RM_Record::GetRid(RID &rid) const {
  rid = this->rid;
  return OK_RC;
}

// Following is the implementation of RM_Manager
RM_Manager::RM_Manager(PF_Manager &pfm) {
  this->pf_manager_ = &pfm;
}

RM_Manager::~RM_Manager() {
}

// TODO(yye): more logic in CreateFile method.
RC RM_Manager::CreateFile(const char *fileName, int recordSize) {
  if (recordSize >= PF_PAGE_SIZE - RM_HEADER_SIZE || recordSize <= 0) {
    return RM_PAGE_FILE_SIZE_EXCEED;
  }
  RC rc = pf_manager_->CreateFile(fileName);
  if (rc != OK_RC) {
    PF_PrintError(rc);
    return rc;
  }
  PF_FileHandle fileHandle;
  rc = pf_manager_->OpenFile(fileName, fileHandle);
  if (rc != OK_RC) {
    PF_PrintError(rc);
    return rc;
  }
  PF_PageHandle headerPageHandler;
  rc = fileHandle.AllocatePage(headerPageHandler);
  return rc;
}

RC RM_Manager::DestroyFile(const char *fileName) {
  return pf_manager_->DestroyFile(fileName);
}

RC RM_Manager::OpenFile(const char *fileName, RM_FileHandle &fileHandler) {
  PF_FileHandle pf_fileHandler;
  RC rc = pf_manager_->OpenFile(fileName, pf_fileHandler);
  fileHandler.SetPFFileHandle(pf_fileHandler);
  return rc;
}

RC RM_Manager::CloseFile(RM_FileHandle &fileHandle) {
  if (fileHandle.GetHandleSet()) {
    PF_FileHandle pfh;
    if (OK_RC == fileHandle.GetPFHandle(pfh)) {
      return pf_manager_->CloseFile(pfh);
    }
  }
  return RM_FAILED_CLOSE_FILE;
}


