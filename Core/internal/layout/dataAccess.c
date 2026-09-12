
#include "OCT_Core_eng.h"
#include <string.h>

#include "ECS/entityContext_int.h"




// static void* iOCT_field_read(eOCT_pool sourcePool, eOCT_fieldTicket fieldDetails, OCT_index entryIndex) {
//     void* fieldLoc = eOCT_pool_access(&sourcePool, entryIndex, fieldDetails.offsetFromStruct);
//     return fieldLoc;
// }








// eOCT_dataUnion* eOCT_single_upload(eOCT_singleKey singleKey, void* source, OCT_handle contextHandle) {
//     eOCT_pool* sourcePool;
//     if (singleKey.global && singleKey.globalPool) {
//         sourcePool = singleKey.globalPool;
//     }
//     else {
//         sourcePool = &iOCT_entityContext_get(contextHandle.objectID)->singles;
//     }
//
//     eOCT_dataUnion* dataLoc = eOCT_pool_access(sourcePool, singleKey.singleTypeIndex, 0);
//
//     if (source) {
//         memcpy(dataLoc, source, sizeof())
//     }
// }