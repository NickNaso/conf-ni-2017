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
#include "deps/simdb.hpp"

namespace KVDB {

  NAN_MODULE_INIT(Database::Init) {
    Local<FunctionTemplate> tpl = Nan::New<FunctionTemplate>(New);
    tpl->SetClassName(Nan::New("Database").ToLocalChecked());
    tpl->InstanceTemplate()->SetInternalFieldCount(1);
    Nan::SetPrototypeMethod(tpl, "get", Get);
    Nan::SetPrototypeMethod(tpl, "put", Put);
    Nan::SetPrototypeMethod(tpl, "keys", Keys);
    Nan::SetPrototypeMethod(tpl, "size", Size);
    Nan::SetPrototypeMethod(tpl, "empty", Empty);
    // Only if you have accessor method
    Local<ObjectTemplate> itpl = tpl->InstanceTemplate();
    Nan::SetAccessor(itpl, Nan::New("db_name").ToLocalChecked(), DbName);
    Nan::SetAccessor(itpl, Nan::New("blocks").ToLocalChecked(), Blocks);
    Nan::SetAccessor(itpl, Nan::New("block_size").ToLocalChecked(), BlockSize);
    constructor.Reset(tpl);
    Nan::Set(target, Nan::New("Database").ToLocalChecked(), tpl->GetFunction());
  }

  NAN_METHOD(Database::New) {
    // Here we need some control 
    String::Utf8Value tmpDbName(info[0]->ToString());
    std::string dbName(*tmpDbName);
    Nan::Maybe<int> blocks = Nan::To<int>(info[1]);
    Nan::Maybe<int> blockSize = Nan::To<int>(info[2]);
    KVDB::Database *database = new KVDB::Database(dbName, blocks.FromJust(), blockSize.FromJust());
    database->Wrap(info.This());
    info.GetReturnValue().Set(info.This());
  }

  NAN_METHOD(Database::Get) {
    // Here we need some control
    KVDB::Database* database = ObjectWrap::Unwrap<KVDB::Database>(info.This());
    simdb db(database->db_name.c_str(), database->blocks, database->block_size);
  }

  NAN_METHOD(Database::Put) {
    // Here we need some control
    KVDB::Database* database = ObjectWrap::Unwrap<KVDB::Database>(info.This());
    simdb db(database->db_name.c_str(), database->blocks, database->block_size);

  }

  NAN_METHOD(Database::Keys) {
    KVDB::Database* database = ObjectWrap::Unwrap<KVDB::Database>(info.This());
    simdb db(database->db_name.c_str(), database->blocks, database->block_size); 
  }

  NAN_METHOD(Database::Size) {
    KVDB::Database* database = ObjectWrap::Unwrap<KVDB::Database>(info.This());
    simdb db(database->db_name.c_str(), database->blocks, database->block_size);
    
  }

  NAN_METHOD(Database::Empty) {
    KVDB::Database* database = ObjectWrap::Unwrap<KVDB::Database>(info.This());
    simdb db(database->db_name.c_str(), database->blocks, database->block_size);
  }

  NAN_GETTER(Database::DbName) {
    KVDB::Database* database = ObjectWrap::Unwrap<KVDB::Database>(info.This());
    info.GetReturnValue().Set(Nan::New(database->db_name).ToLocalChecked());
  }

  NAN_GETTER(Database::Blocks) {
    KVDB::Database* database = ObjectWrap::Unwrap<KVDB::Database>(info.This());
    info.GetReturnValue().Set(Nan::New(database->blocks));
  }

  NAN_GETTER(Database::BlockSize) {
    KVDB::Database* database = ObjectWrap::Unwrap<KVDB::Database>(info.This());
    info.GetReturnValue().Set(Nan::New(database->block_size));
  }

  Database::Database(std::string db_name, int blocks, int block_size) {
    this->db_name = db_name;
    this->blocks = blocks;
    this->block_size = block_size;
  }

  Database::~Database() {
    // NOOP
  }

}