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


/*						Titles														      Authors										Date
 *  Copyright 2014, Chatsiri Rattana.                                           07/02/2014
 *  -Base register class for init opencl type.        R.Chatsiri                07/02/2014
 *  -Base register call scanning PE, ELF and Process types
 *                                                    R.Chatsiri                12/09/2014
 */

#ifndef CLASS_REGISTER__HPP
#define CLASS_REGISTER__HPP

#include <map>
#include <string>

// The first parameter, register_name, should be unique globally.
// Another approach for this is to define a template for base class. It would
// make the code more readable, but the only issue of using template is that
// each base class could have only one register then. It doesn't sound very
// likely that a user wants to have multiple registers for one base class,
// but we keep it as a possibility.
// We would switch to using template class if necessary.
#define CLASS_REGISTER_DEFINE_REGISTRY(register_name, base_class_name)  \
  class ObjectCreatorRegistry_##register_name {                         \
   public:                                                              \
   typedef base_class_name* (*Creator)();                               \
                                                                        \
   ObjectCreatorRegistry_##register_name()                              \
   : m_default_creator(NULL) {}                                         \
   ~ObjectCreatorRegistry_##register_name() {}                          \
                                                                        \
   void SetDefaultCreator(Creator creator) {                            \
     m_default_creator = creator;                                       \
   }                                                                    \
                                                                        \
   void AddCreator(std::string entry_name, Creator creator) {           \
     m_creator_registry[entry_name] = creator;                          \
   }                                                                    \
                                                                        \
   base_class_name* CreateObject(const std::string& entry_name);        \
                                                                        \
   private:                                                             \
   typedef std::map<std::string, Creator> CreatorRegistry;              \
   Creator m_default_creator;                                           \
   CreatorRegistry m_creator_registry;                                  \
  };                                                                    \
                                                                        \
  inline ObjectCreatorRegistry_##register_name&                         \
  GetRegistry_##register_name() {                                       \
    static ObjectCreatorRegistry_##register_name registry;              \
    return registry;                                                    \
  }                                                                     \
                                                                        \
  class DefaultObjectCreatorRegister_##register_name {                  \
   public:                                                              \
   DefaultObjectCreatorRegister_##register_name(                        \
       ObjectCreatorRegistry_##register_name::Creator creator) {        \
     GetRegistry_##register_name().SetDefaultCreator(creator);          \
   }                                                                    \
   ~DefaultObjectCreatorRegister_##register_name() {}                   \
  };                                                                    \
                                                                        \
  class ObjectCreatorRegister_##register_name {                         \
   public:                                                              \
   ObjectCreatorRegister_##register_name(                               \
       const std::string& entry_name,                                   \
       ObjectCreatorRegistry_##register_name::Creator creator) {        \
     GetRegistry_##register_name().AddCreator(entry_name,               \
                                              creator);                 \
   }                                                                    \
   ~ObjectCreatorRegister_##register_name() {}                          \
  }

#define CLASS_REGISTER_IMPLEMENT_REGISTRY(register_name, base_class_name) \
  base_class_name* ObjectCreatorRegistry_##register_name::CreateObject( \
      const std::string& entry_name) {                                  \
    Creator creator = m_default_creator;                                \
    CreatorRegistry::const_iterator it =                                \
        m_creator_registry.find(entry_name);                            \
    if (it != m_creator_registry.end()) {                               \
      creator = it->second;                                             \
    }                                                                   \
                                                                        \
    if (creator != NULL) {                                              \
      return (*creator)();                                              \
     } else {                                                           \
      return NULL;                                                      \
    }                                                                   \
  }

#define CLASS_REGISTER_DEFAULT_OBJECT_CREATOR(register_name,            \
                                              base_class_name,          \
                                              class_name)               \
  base_class_name* DefaultObjectCreator_##register_name##class_name() { \
    return new class_name;                                              \
  }                                                                     \
  DefaultObjectCreatorRegister_##register_name                          \
  g_default_object_creator_register_##register_name##class_name(        \
      DefaultObjectCreator_##register_name##class_name)

#define CLASS_REGISTER_OBJECT_CREATOR(register_name,                    \
                                      base_class_name,                  \
                                      entry_name_as_string,             \
                                      class_name)                       \
  base_class_name* ObjectCreator_##register_name##class_name() {        \
    return new class_name;                                              \
  }                                                                     \
  ObjectCreatorRegister_##register_name                                 \
  g_object_creator_register_##register_name##class_name(                \
      entry_name_as_string,                                             \
      ObjectCreator_##register_name##class_name)

#define CLASS_REGISTER_CREATE_OBJECT(register_name, entry_name_as_string) \
  GetRegistry_##register_name().CreateObject(entry_name_as_string)

#endif  //CLASS_REGISTER__HPP
