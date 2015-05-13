//
//  Error.hh
//  CBForest
//
//  Created by Jens Alfke on 6/15/14.
//  Copyright (c) 2014 Couchbase. All rights reserved.
//
//  Licensed under the Apache License, Version 2.0 (the "License"); you may not use this file
//  except in compliance with the License. You may obtain a copy of the License at
//    http://www.apache.org/licenses/LICENSE-2.0
//  Unless required by applicable law or agreed to in writing, software distributed under the
//  License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND,
//  either express or implied. See the License for the specific language governing permissions
//  and limitations under the License.

#ifndef CBForest_Error_h
#define CBForest_Error_h

#include <libforestdb/forestdb.h>

namespace forestdb {

    /** Most API calls can throw this. */
    struct error {
        // Extra status codes not defined by fdb_errors.h
        enum CBForestError {
            BadRevisionID = -1000,
            CorruptRevisionData = -1001,
        };

        /** Either an fdb_status code, as defined in fdb_errors.h; or a CBForestError. */
        int status;

        error (fdb_status s)        :status(s) {}
        error (CBForestError e)     :status(e) {}
    };

}

#endif
