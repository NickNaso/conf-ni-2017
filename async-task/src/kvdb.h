/*******************************************************************************
 * Copyright (c) Nicola Del Gobbo
 * Licensed under the Apache License, Version 2.0 (the "License"); you may not
 * use this file except in compliance with the License. You may obtain a copy of
 * the license at http://www.apache.org/licenses/LICENSE-2.0
 *
 * THIS CODE IS PROVIDED ON AN *AS IS* BASIS, WITHOUT WARRANTIES OR CONDITIONS
 * OF ANY KIND, EITHER EXPRESS OR IMPLIED, INCLUDING WITHOUT LIMITATION ANY
 * IMPLIED WARRANTIES OR CONDITIONS OF TITLE, FITNESS FOR A PARTICULAR PURPOSE,
 * MERCHANTABLITY OR NON-INFRINGEMENT.
 *
 * See the Apache Version 2.0 License for specific language governing
 * permissions and limitations under the License.
 *
 * Contributors - initial API implementation:
 * Nicola Del Gobbo <nicoladelgobbo@gmail.com>
 ******************************************************************************/

#ifndef KVDB_H
#define KVDB_H

#include <nan.h>
#include "deps/vedis.h"


using namespace v8;

namespace KVDB {

    class Database : public Nan::ObjectWrap {
        public: 
            static NAN_MODULE_INIT(Init);
            static NAN_METHOD(New);
            static NAN_METHOD(GetKeySync);
            static NAN_METHOD(PutKeySync);
            static NAN_GETTER(DbName);
        private:
            Database(std::string db_name);
            ~Database();
            std::string db_name;
            vedis *db;    
            //static Nan::Persistent<v8::FunctionTemplate> constructor; 
            //static inline Persistent<v8::Function> & constructor();
            static inline Persistent<v8::Function> & constructor();
    };

    static std::string root_path = "./tmp";
    static std::string db_extension = ".db";
    //static Nan::Persistent<v8::FunctionTemplate> constructor; 
    

}

#endif //KVDB_H