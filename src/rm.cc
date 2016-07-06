#include "rm.h"
#include "rm_error.h"
#include <cstring>

using namespace std;

Bitmap::Bitmap(int numBits) {
  CHECK(numBits > 0);
  size = numBits;
  bitArray = new char[size/sizeof(char)+1];
  memset(bitArray, 0, sizeof(char) * (size/sizeof(char)+1));
}

Bitmap::~Bitmap() {
  delete[] bitArray;
}

bool Bitmap::test(int bitNumber) {
  return ( (bitArray[bitNumber/sizeof(char)] & (1 << (bitNumber%sizeof(char)) )) != 0 );
}

void Bitmap::set(int bitNumber, bool value) {
  if (value) {
    bitArray[bitNumber/sizeof(char)] |= 1 << (bitNumber%sizeof(char));
  } else {
    bitArray[bitNumber/sizeof(char)] &= ~(1 << (bitNumber%sizeof(char)));
  }
}

int Bitmap::getSize() {
  return size;
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

RM_Record::RM_Record(const char* data,
                     int length,
                     const RID rid) {
  this->rid = rid;
  data_ = new char[length + 1];
  memcpy(data_, data, length);
  this->length = length;
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

// Following is the implementation of RM_FileHandle
RM_FileHandle::RM_FileHandle() {
  handle_set = false;
}

RM_FileHandle::~RM_FileHandle() {

}

RC RM_FileHandle::GetRec(const RID &rid, RM_Record &rec) const {
  if (!handle_set) {
    return RM_PAGE_FILE_HANDLE_CLOSED_WARN;
  }
  return OK_RC;
}

RC RM_FileHandle::InsertRec(const char* pData, RID & rid) {
  if (!handle_set) {
    return RM_PAGE_FILE_HANDLE_CLOSED_WARN;
  }

  return OK_RC;
}

RC RM_FileHandle::DeleteRec(const RID &rid) {
  if (!handle_set) {
    return RM_PAGE_FILE_HANDLE_CLOSED_WARN;
  }

  return OK_RC;
}

RC RM_FileHandle::UpdateRec(const RM_Record &rec) {
  if (!handle_set) {
    return RM_PAGE_FILE_HANDLE_CLOSED_WARN;
  }
  return OK_RC;
}

RC RM_FileHandle::ForcePages(PageNum pageNum) {
  if (!handle_set) {
    return RM_PAGE_FILE_HANDLE_CLOSED_WARN;
  }
  return OK_RC;
}

void RM_FileHandle::SetPFFileHandle(PF_FileHandle& handle) {
  handle_ = handle;
  handle_set = true;
}

bool RM_FileHandle::GetHandleSet() {
  return handle_set;
}

RC RM_FileHandle::GetPFHandle(PF_FileHandle& handle) {
  if (!handle_set) {
    return RM_PAGE_FILE_HANDLE_CLOSED_WARN;
  }
  handle = handle;
  return OK_RC;
}
