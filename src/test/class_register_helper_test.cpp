/*
This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or (at
your option) any later version.

This program is distributed in the hope that it will be useful, but
WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
02110-1301, USA.
*/

// Defines several base class and registers for testing. We intentionally
// define them in a separate file as some compilers don't correctly support to
// define static variable in inline function, they create a separate copy each
// time it's included. We want to make sure it doesn't happen to us.

/*  Authors	                                          Titles			              Date
 *  Copyright 2010, Yi Wang (yiwang@tencent.com), Hangjun Ye (hansye@tencent.com)
 *  Copyright 2014, MTSec Inc.                                                  07/02/2014
 *  -Test register class for init opencl module.        
 *                                                    R.Chatsiri                26/02/2014
 */



#include "utils/base/class_register_test_helper.h"

/*
CLASS_REGISTER_IMPLEMENT_REGISTRY(mapper_register, Mapper);
CLASS_REGISTER_IMPLEMENT_REGISTRY(second_mapper_register, Mapper);
CLASS_REGISTER_IMPLEMENT_REGISTRY(reducer_register, Reducer);
CLASS_REGISTER_IMPLEMENT_REGISTRY(file_impl_register, FileImpl);
*/

/*
class HelloMapper : public Mapper {
  virtual std::string GetMapperName() const {
    return "HelloMapper";
  }
};
REGISTER_MAPPER(HelloMapper);

class WorldMapper : public Mapper {
  virtual std::string GetMapperName() const {
    return "WorldMapper";
  }
};
REGISTER_MAPPER(WorldMapper);

class SecondaryMapper : public Mapper {
  virtual std::string GetMapperName() const {
    return "SecondaryMapper";
  }
};
REGISTER_SECONDARY_MAPPER(SecondaryMapper);


class HelloReducer : public Reducer {
  virtual std::string GetReducerName() const {
    return "HelloReducer";
  }
};
REGISTER_REDUCER(HelloReducer);

class WorldReducer : public Reducer {
  virtual std::string GetReducerName() const {
    return "WorldReducer";
  }
};
REGISTER_REDUCER(WorldReducer);
*/

