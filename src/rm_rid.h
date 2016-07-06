//
// rm_rid.h
//
//   The Record Id interface
//

#ifndef RM_RID_H
#define RM_RID_H

// We separate the interface of RID from the rest of RM because some
// components will require the use of RID but not the rest of RM.

#include "redbase.h"

//
// PageNum: uniquely identifies a page in a file
//
typedef long PageNum;

//
// SlotNum: uniquely identifies a record in a page
//
typedef int SlotNum;

const PageNum kInvalidPageNum = -1;

const SlotNum kInvalidSlotNum = -1;

#define RM_INVALIDPAGE      (START_RM_ERR - 1) //  page number is invalid
#define RM_INVALIDSLOT      (START_RM_ERR - 2) //  slot number is invalid

//
// RID: Record id interface
//
class RID {
 public:
  RID();                                         // Default constructor
  RID(PageNum pageNum, SlotNum slotNum);
  ~RID();                                        // Destructor

  RC GetPageNum(PageNum &pageNum) const;         // Return page number
  RC GetSlotNum(SlotNum &slotNum) const;         // Return slot number
  // Copy constructor of RID.
  RID(const RID& rid);
  // Override = operator.
  RID& operator=(const RID &rid);
 private:
  PageNum page_num_;
  SlotNum slot_num_;
};

#endif
