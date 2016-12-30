#include "rm.h"
#include "rm_error.h"
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
  RM_PageHdr page_hdr =
    RM_PageHdr::deserialize(page_data, this->record_num_per_page);
  if (!page_hdr.availableSlotMap->Test(slot_num)) {
    return RM_ERASED_RECORD;
  }
  const char* record = page_data + page_hdr.size() + slot_num * record_size;
  rec.Set(record, record_size, rid);
//  page_data =  new RM_PageHdr() 
  return OK_RC;
}

RC RM_FileHandle::InsertRec(const char* pData, RID & rid) {
  RC ret_value;
  ret_value = this->IsValid();
  if (ret_value != OK_RC) {
    return ret_value;
  }
  PF_PageHandle page_handler;
  handle_.GetThisPage(0, page_handler);
  char* page_data;
  page_handler.GetData(page_data);
  RM_PageHdr page_hdr =
    RM_PageHdr::deserialize(page_data, this->record_num_per_page);
  int index = -1;
  for (int i = 0; i < this->record_num_per_page; i++) {
    if (page_hdr.availableSlotMap->Test(i)) {
      index = i;
      break;
    }
  }
  // Cannot find empty slot.
  if (index < 0) {
    return RM_FULL_PAGE;
  }
 // handle_.
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
