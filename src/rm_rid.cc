#include "rm_rid.h"

RID::RID() {
  this->page_num_ = kInvalidPageNum;
  this->slot_num_ = kInvalidSlotNum;
}

RID::RID(PageNum pageNum, SlotNum slotNum) {
  this->page_num_ = pageNum;
  this->slot_num_ = slotNum;
}

RID::~RID() {
}

RC RID::GetPageNum(PageNum &pageNum) const {
  if (this->page_num_ == kInvalidPageNum) {
    return RM_INVALIDPAGE;
  }
  pageNum = this->page_num_;
  return OK_RC;
}

RC RID::GetSlotNum(SlotNum &slotNum) const {
  if (this->slot_num_ == kInvalidSlotNum) {
    return RM_INVALIDSLOT;
  }
  slotNum = this->slot_num_;
  return OK_RC;
}
