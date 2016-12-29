#include "rm.h"
#include "rm_rid.h"

// Following is the implementation of RM_FileHandle
RM_FileHandle::RM_FileHandle() {
  handle_set = false;
}

RM_FileHandle::~RM_FileHandle() {

}

RC RM_FileHandle::GetRec(const RID &rid, RM_Record &rec) const {
  RC ret_value;
  ret_value = this->IsValid();
  if (ret_value != OK_RC) {
    return ret_value;
  }
  PageNum page_num;
  SlotNum slot_num; 
  rid.GetPageNum(page_num);
  rid.GetSlotNum(slot_num);
  PF_PageHandle page_handler;
  // TODO(yye): check if this page num is valid or not.
  handle_.GetThisPage(page_num, page_handler);
  char* page_data;
  page_handler.GetData(page_data);
//  page_data =  new RM_PageHdr() 
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

RC RM_FileHandle::IsValid() const {
  if (!handle_set) {
    return RM_PAGE_FILE_HANDLE_CLOSED_WARN;
  }
  return OK_RC;
}
