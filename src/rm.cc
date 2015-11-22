#include "rm.h"

// Following is the implementation of RM_Record
// TODO (yye@) add more logic for the construction
RM_Record::RM_Record() {

}

RM_Record::~RM_Record() {
}

RC RM_Record::GetData(char *& pData) const {
  return OK_RC;
}

RC RM_Record::GetRid(RID &rid) const {
  return OK_RC;
}

// Following is the implementation of RM_Manager
RM_Manager::RM_Manager(PF_Manager &pfm) {
  this->pf_manager_ = &pfm;
}

RM_Manager::~RM_Manager() {
}

RC RM_Manager::CreateFile(const char *fileName, int recordSize) {
  if (recordSize > PF_PAGE_SIZE || recordSize <= 0) {
    return RM_PAGE_FILE_SIZE_EXCEED;
  }
  return pf_manager_->CreateFile(fileName);
}

RC RM_Manager::DestroyFile(const char *fileName) {
  return pf_manager_->DestroyFile(fileName);
}

RC RM_Manager::OpenFile(const char *fileName, RM_FileHandle &fileHandler) {
  PF_FileHandle pf_fileHandler;
  RC rc = pf_manager_->OpenFile(fileName, pf_fileHandler);
  return rc;
}

RC RM_Manager::CloseFile(RM_FileHandle &fileHandle) {
  if (fileHandle.GetHandleSet()) {
    //    return manager_.CloseFile(fileHandle.GetPFHandle());
  }
  return END_RM_ERR;
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
  handle_ = &handle;
  handle_set = true;
}

bool RM_FileHandle::GetHandleSet() {
  return handle_set;
}

RC RM_FileHandle::GetPFHandle(PF_FileHandle& handle) {
  if (!handle_set) {
    return RM_PAGE_FILE_HANDLE_CLOSED_WARN;
  }
  handle_ = &handle;
  return OK_RC;
}
