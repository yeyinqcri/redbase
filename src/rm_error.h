#ifndef RM_ERROR_H
#define RM_ERROR_H

#include "redbase.h"

#define RM_SIZE_TOO_BUG (START_RM_ERR - 0)
#define RM_NULLRECORD (START_RM_ERR - 1)
#define RM_ERASED_RECORD (START_RM_ERR - 2)
#define RM_FULL_PAGE (START_RM_ERR - 3)

#define RM_FILE_SCAN_OPENED (START_RM_ERR - 4)
#define RM_FILE_SCAN_NOT_OPENED (START_RM_ERR - 5)
#define RM_UNKNOWN_TYPE (START_RM_ERR - 6)

#define RM_EOF (START_RM_WARN + 1)

#endif  // RM_ERROR_H
