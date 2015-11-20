#include "rm.h"

RM_FileHandle::RM_FileHandle() {
  handle_set = false;
}

RM_FileHandle::~RM_FileHandle() {

}

RC RM_FileHandle::GetRec(const RID &rid, RM_Record &rec) const {
  if (!handle_set) {
    return RM_PAGE_FILE_HANDLE_CLOSED_WARN;
  }
handle_.
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

RC RM_FileHandle::ForcePages(PageNum pageNum = ALL_PAGES) {
if (!handle_set) {
  return RM_PAGE_FILE_HANDLE_CLOSED_WARN;
}
return OK_RC;
}

void RM_FileHandle::SetPFFileHandle(const PF_FileHandle& handle) {
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
handle = handle_;
return OK_RC;
}
