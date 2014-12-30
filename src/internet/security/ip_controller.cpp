#include "internet/security/ip_controller.hpp"

#include "utils/convert.hpp"

#include "rocksdb/iterator.h"

namespace internet
{

    namespace security
    {

				//Plan implement message send to Tracethreat system.
				template<typename MessageType>
				bool ip_controller<MessageType>::ping_service(){

				}

        //Load object from Instance load in MACRO
        template<typename MessageType>
        bool ip_controller<MessageType>::load_ip()
        {

            options.IncreaseParallelism();
            options.OptimizeLevelStyleCompaction();
            options.create_if_missing = true;

            rocksdb::Status s = rocksdb::DB::Open(options, db_path, &db);

            if(s.ok() && ip_map.empty()) {
                return false;
            }

            //loop get all fraud IP in table.
            rocksdb::Iterator *iter_ip = db->NewIterator(rocksdb::ReadOptions());

            for(iter_ip->SeekToFirst(); iter_ip->Valid(); iter_ip->Next()) {
                //convert key from char to int64_t
                int64_t key = utils::convert::char_to_int64bit(iter_ip->key().ToString().c_str());
                std::string serialize_obj = iter_ip->value().ToString();

                ip_map.insert(std::make_pair(key, serialize_obj));
            }

            //load fruad  IP from table.
            return true;

        }

        //Use lock with mutex
        template<typename MessageType>
        bool ip_controller<MessageType>::
        filter_ip(const char *ip, std::unique_ptr<MessageType>& msg_ptr)
        {
            wait_sync();
						int64_t key = utils::convert::char_to_int64bit(ip);
            if(ip_map.find(key) == ip_map.end()) {
                LOG(INFO)<<"IP in blacklist try to access";
                LOG(INFO)<<"IP Address : "<< msg_ptr->ip();
                LOG(INFO)<<"UUID       : "<<  msg_ptr->uuid();
                sync();
                //TO-DO: Send message to Tracethreat mailbox.
                return true;
            }

            sync();
            return false;
        }

        //Use lock with mutex
        //Lock and return : http://goo.gl/Cr0KJg
        //Return protected data : http://goo.gl/hwFhnL
        template<typename MessageType>
        bool ip_controller<MessageType>::
        store_ip(const  char   *ip, std::unique_ptr<MessageType>& msg_ptr)
        {
            wait_sync();
            int64_t key = utils::convert::char_to_int64bit(ip);
            std::string serialize_obj;
            msg_ptr.SerializeToString(serialize_obj);
            ip_map.insert(std::make_pair(key, serialize_obj));
            sync();
            return true;
        }

        template<typename MessageType>
        void ip_controller<MessageType>::wait_sync()
        {
            boost::unique_lock<boost::mutex> lock_(mutex_);
            sync_send = false;

            while(!sync_send) {
                cond_.wait(lock_);
            }//while
        }

        template<typename MessageType>
        void ip_controller<MessageType>::sync()
        {
            {
                boost::lock_guard<boost::mutex> lock_(mutex_);
                sync_send = true;
            }
            cond_.notify_all();
        }


    }


}
