/*						Titles														      Authors										Date
 *  Copyright 2014, MTSec Inc.                                                  07/02/2014
 *  -Test register class for init opencl module.        
 *                                                    R.Chatsiri                26/02/2014
 */

#include <boost/scoped_ptr.hpp>

#include "src/base/class_register.h"
#include "src/base/class_register_test_helper.h"


TEST(ClassRegister, CreateMapper) {

/*
  scoped_ptr<Mapper> mapper;
  mapper.reset(CREATE_MAPPER(""));
  EXPECT_TRUE(mapper.get() == NULL);

  mapper.reset(CREATE_MAPPER("HelloMapper "));
  EXPECT_TRUE(mapper.get() == NULL);
*/
 
}

TEST(ClassRegister, CreateSecondaryMapper) {
/*
  scoped_ptr<Mapper> mapper;
  mapper.reset(CREATE_SECONDARY_MAPPER(""));
  EXPECT_TRUE(mapper.get() == NULL);

  mapper.reset(CREATE_SECONDARY_MAPPER("SecondaryMapper "));
  EXPECT_TRUE(mapper.get() == NULL);
*/
  
}

TEST(ClassRegister, CreateReducer) {
/*
  scoped_ptr<Reducer> reducer;
  reducer.reset(CREATE_REDUCER(""));
  EXPECT_TRUE(reducer.get() == NULL);

  reducer.reset(CREATE_REDUCER("HelloReducer "));
  EXPECT_TRUE(reducer.get() == NULL);
*/
  
}

