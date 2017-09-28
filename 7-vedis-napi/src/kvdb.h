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

#include <napi.h>
#include <uv.h>
#include "deps/vedis.h"


using namespace Napi;

namespace KVDB {

    class Database : public Napi::ObjectWrap<Database> {
        public: 
            static void Init(Napi::Env env, Napi::Object exports, Napi::Object module);
            static Napi::Value New(const Napi::CallbackInfo& info);
            static Napi::Value GetKey(const Napi::CallbackInfo& info);
            static Napi::Value GetKeyBuffer(const Napi::CallbackInfo& info);
            static Napi::Value GetKeySync(const Napi::CallbackInfo& info);
            static Napi::Value PutKey(const Napi::CallbackInfo& info);
            static Napi::Value PutKeyBuffer(const Napi::CallbackInfo& info);
            static Napi::Value PutKeySync(const Napi::CallbackInfo& info);
            Napi::Value DbName(const Napi::CallbackInfo& info);
        private:
            Database(std::string db_name);
            ~Database();
            std::string db_name;
            vedis *db;
            //static inline Persistent<v8::Function> & constructor();
    };

    static std::string root_path = "./tmp";
    static std::string db_extension = ".db";

}

#endif //KVDB_H