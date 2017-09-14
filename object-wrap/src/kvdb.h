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
//#include "deps/simdb.hpp"

using namespace v8;

namespace KVDB {

    class Database : public Nan::ObjectWrap {
        public: 
            static NAN_MODULE_INIT(Init);
            static NAN_METHOD(New);
            static NAN_METHOD(Get);
            static NAN_METHOD(Put);
            static NAN_METHOD(Keys);
            static NAN_METHOD(Size);
            static NAN_METHOD(Empty);
            static NAN_GETTER(DbName);
            static NAN_GETTER(Blocks);
            static NAN_GETTER(BlockSize);
        private:
            Database(std::string db_name, int blocks, int block_size);
            ~Database();
            std::string db_name;
            int blocks;
            int block_size;         
    };

    static Nan::Persistent<v8::FunctionTemplate> constructor; 

    

}

#endif //KVDB_H