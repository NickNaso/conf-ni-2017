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

#include "kvdb.h"

using namespace v8;

namespace KVDB {

  NAN_MODULE_INIT(Database::Init) {
    Local<FunctionTemplate> tpl = Nan::New<FunctionTemplate>(New);
    constructor.Reset(tpl);
    tpl->SetClassName(Nan::New("Database").ToLocalChecked());
    tpl->InstanceTemplate()->SetInternalFieldCount(1);

    Nan::SetPrototypeMethod(tpl, "get", Get);
    Nan::SetPrototypeMethod(tpl, "put", Put);
    Nan::SetPrototypeMethod(tpl, "keys", Keys);
    Nan::SetPrototypeMethod(tpl, "size", Size);
    Nan::SetPrototypeMethod(tpl, "close", Close);

    // Only if you have accessor method
    //Local<ObjectTemplate> itpl = tpl->InstanceTemplate();

    Nan::Set(target, Nan::New("Database").ToLocalChecked(), tpl->GetFunction());
  }

  NAN_METHOD(Database::New) {
    KVDB::Database *database;

  }

  NAN_METHOD(Database::Get) {

  }

  NAN_METHOD(Database::Put) {
    
  }

  NAN_METHOD(Database::Keys) {
    
  }

  NAN_METHOD(Database::Size) {
    
  }

  NAN_METHOD(Database::Close) {
    
  }

}