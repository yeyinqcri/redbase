#include "rm.h"
#include "rm_error.h"
#include "rm_rid.h"

#include <cstring>

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
    RM_PageHdr::deserialize(page_data, file_header_.record_num_per_page);
  if (!page_hdr.availableSlotMap->Test(slot_num)) {
    return RM_ERASED_RECORD;
  }
  const char* record = page_data + page_hdr.size() +
    slot_num * file_header_.record_size;
  rec.Set(record, file_header_.record_size, rid);
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
  bool is_head_dirty = false;
  // If there is no free page in the file, create a new page for the insertion.
  if (file_header_.first_free_page_num == kInvalidPageNum) {
    handle_.AllocatePage(page_handler);
    page_handler.GetPageNum(file_header_.first_free_page_num);
    is_head_dirty = true;
  }
  PageNum curr_page_num = file_header_.first_free_page_num;
  handle_.GetThisPage(file_header_.first_free_page_num, page_handler);
  char* page_data;
  page_handler.GetData(page_data);
  RM_PageHdr page_hdr =
    RM_PageHdr::deserialize(page_data, file_header_.record_num_per_page);
  int slot_num = -1;
  for (int i = 0; i < file_header_.record_num_per_page; i++) {
    if (page_hdr.availableSlotMap->Test(i)) {
      slot_num = i;
      break;
    }
  }
  // Cannot find empty slot, this is an exception.
  if (slot_num < 0) {
    return RM_FULL_PAGE;
  }
  char* record = page_data + page_hdr.size() +
    slot_num * file_header_.record_size;
  // Copy all the data into the slot.
  memcpy(record, pData, this->file_header_.record_size);
  // Update the metadata of the page.
  page_hdr.availableSlotMap->Set(slot_num, 1);
  page_hdr.numFreeSlots = page_hdr.numFreeSlots - 1;
  page_hdr.serialize(page_data);
  handle_.MarkDirty(curr_page_num);
  // Update the returned value.
  rid = RID(curr_page_num, slot_num);
  if (page_hdr.numFreeSlots == 0) {
    file_header_.first_free_page_num = page_hdr.nextFree;
    is_head_dirty = true;
  }
  // Update the first free page number is needed.
  if (is_head_dirty) {
    PF_PageHandle first_page_handler;
    handle_.GetThisPage(0, first_page_handler);
    char* first_page_data;
    first_page_handler.GetData(first_page_data);
    file_header_.serialize(first_page_data);
    handle_.MarkDirty(HEADER_PAGE_NUM);
  }
  return OK_RC;
}

RC RM_FileHandle::UpdateNextFreeSlot(const PageNum page_num) {
  PageNum previous_first_page = file_header_.first_free_page_num;
  PF_PageHandle page_handler;
  // Updates the header of the file.
  handle_.GetThisPage(0, page_handler);
  char* header_data;
  page_handler.GetData(header_data);
  file_header_.first_free_page_num = page_num;
  file_header_.serialize(header_data);
  handle_.MarkDirty(HEADER_PAGE_NUM);
  // Updates the header of the page;
  char* page_data;
  handle_.GetThisPage(page_num, page_handler);
  page_handler.GetData(page_data);
  RM_PageHdr page_hdr =
    RM_PageHdr::deserialize(page_data, file_header_.record_num_per_page);
  page_hdr.nextFree = previous_first_page;
  page_hdr.serialize(page_data);
  handle_.MarkDirty(page_num);
  return OK_RC;
}

RC RM_FileHandle::DeleteRec(const RID &rid) {
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
    RM_PageHdr::deserialize(page_data, file_header_.record_num_per_page);
  if (!page_hdr.availableSlotMap->Test(slot_num)) {
    return RM_ERASED_RECORD;
  }
  page_hdr.availableSlotMap->Set(slot_num, 1);
  // Updates the number of free slot.
  if (page_hdr.numFreeSlots == 0) {
    if (ret_value = UpdateNextFreeSlot(page_num)) {
      return ret_value;
    }
  }
  // Updates the metadata of the page. (page header)
  page_hdr.numFreeSlots += 1;
  page_hdr.serialize(page_data);
  handle_.MarkDirty(page_num);
  return OK_RC;
}

RC RM_FileHandle::UpdateRec(const RM_Record &rec) {
  RC ret_value;
  ret_value = this->IsValid();
  if (ret_value != OK_RC) {
    return ret_value;
  }
  RID rid;
  rec.GetRid(rid);
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
    RM_PageHdr::deserialize(page_data, file_header_.record_num_per_page);
  char* record = page_data + page_hdr.size() +
    slot_num * this->file_header_.record_size;
  // Copy all the data into the slot.
  char* updated_data;
  rec.GetData(updated_data);
  memcpy(record, updated_data, file_header_.record_size);
  handle_.MarkDirty(page_num);
  return OK_RC;
}

RC RM_FileHandle::ForcePages(PageNum pageNum) {
  RC ret_value;
  ret_value = this->IsValid();
  if (ret_value != OK_RC) {
    return ret_value;
  }
  handle_.ForcePages(pageNum);
  return OK_RC;
}

void RM_FileHandle::SetPFFileHandle(PF_FileHandle& handle) {
  handle_ = handle;
  handle_set = true;
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
