
#include <iostream>
#include <cstring>

#include  <boost/shared_ptr.hpp>

#include <folly/io/TypedIOBuf.h>

#include "internet/scan_server/security_connection.hpp"
#include "internet/msg/scan_server_client/message_scan.pb.h"

#include "internet/scan_server/packedmessage_scan.hpp"

#include "utils/uuid_generator.hpp"

//Check Queue for IOBuffer  as Folly Test.
void check_consistency(const folly::IOBufQueue& queue)
{

    if(queue.options().cacheChainLength) {
        size_t len = queue.front() ? queue.front()->computeChainDataLength() : 0;

        LOG(INFO)<<"Chain Length : "<< len;

        EXPECT_EQ(len, queue.chainLength());
    }

}

/*
TEST(SecurityConnection, uuid_controller_queue)
{
    //Step initial data
    typedef boost::shared_ptr<message_scan::RequestScan> msg_request_ptr_type;

    std::vector<uint8_t> msg_vec;

    msg_request_ptr_type  msg_req(new message_scan::RequestScan);
    msg_req->set_type(message_scan::RequestScan::CLOSE_CONNECTION);
    msg_req->set_uuid("0:1:01:0");
    msg_req->set_timestamp("0:0:0:0");

    //pack data in msg_vec
    internet::packedmessage_scan<message_scan::RequestScan>  pack_msg_req(msg_req);
    pack_msg_req.pack(msg_vec);


    //LOG(INFO)<<" Message Register value : " << message_scan::RequestScan::SCAN;
    //LOG(INFO)<<" Message Scan     value : " << message_scan::RequestScan::REGISTER;

    //UUID controller initial
  //  internet::security::uuid_controller<msg_request_ptr_type> uuid_con;
    internet::security::uuid_controller<message_scan::RequestScan> uuid_con;

    //IOBuffer_scan
    std::unique_ptr<folly::IOBuf> iobuffer_scan =	uuid_con.initial_iobuf();


    //Wrapper all data to IOBuffer.
    iobuffer_scan = folly::IOBuf::wrapBuffer(&msg_vec[0], msg_vec.size());


    //Check data size match with IOBuffer contains data.
    EXPECT_EQ(iobuffer_scan->data(), msg_vec.data());
    EXPECT_EQ(iobuffer_scan->capacity(), msg_vec.size());



    /*
    //convert from array to vector in uint8_t type.
    std::vector<uint8_t> result_msg_vec(iobuffer_scan->data(),
        iobuffer_scan->data() + iobuffer_scan->length());

    //Test unpack message to vector after message packed.
    pack_msg_req.unpack(result_msg_vec);

    //Read message from unpack class.
    msg_request_ptr_type  result_msg_req(new message_scan::RequestScan);
    result_msg_req = pack_msg_req.get_msg();

    LOG(INFO)<<"Unpack for IO-Buffer,  RequestType : " << result_msg_req->type();
    LOG(INFO)<<"Unpack for IO-Buffer,  UUID        : " << result_msg_req->uuid();
    */
/*
    //push  IOBuffer to UUID Controller.
    uuid_con.push_msg(std::move(iobuffer_scan));


    check_consistency(uuid_con.get_queue());

    std::vector<uint8_t> msg_vec2;

    //State one list
    msg_request_ptr_type  msg_req2(new message_scan::RequestScan);
    msg_req2->set_type(message_scan::RequestScan::SCAN);
    msg_req2->set_uuid("0:1:01:10");
    msg_req2->set_timestamp("0:0:0:1");

    //pack data in msg_vec
    internet::packedmessage_scan<message_scan::RequestScan>  pack_msg_reqs(msg_req2);
    pack_msg_reqs.pack(msg_vec2);


    //IOBuffer_scan
    std::unique_ptr<folly::IOBuf> iobuffer_scan2 =	uuid_con.initial_iobuf();

    //Wrapper all data to IOBuffer.
    iobuffer_scan2 = folly::IOBuf::wrapBuffer(&msg_vec2[0], msg_vec2.size());

    //Check data size match with IOBuffer contains data.
    EXPECT_EQ(iobuffer_scan2->data(), msg_vec2.data());
    EXPECT_EQ(iobuffer_scan2->capacity(), msg_vec2.size());


    check_consistency(uuid_con.get_queue());


    //push  IOBuffer to UUID Controller.
    uuid_con.push_msg(std::move(iobuffer_scan2));



    //Get front to check.
    folly::IOBufQueue& queue = uuid_con.get_queue();

    //const folly::IOBuf* buffer_front = queue.front();

    std::unique_ptr<folly::IOBuf> buffer_front = queue.pop_front();


    //Retest after pop front from queue.
    //convert from array to vector in uint8_t type.
    std::vector<uint8_t> result_vec(buffer_front->data(),
            buffer_front->data() + buffer_front->length());

    //Test unpack message to vector after message packed.
    pack_msg_req.unpack(result_vec);

    //Read message from unpack class.
    msg_request_ptr_type  result_req(new message_scan::RequestScan);
    result_req = pack_msg_req.get_msg();
    LOG(INFO)<<"-------------------POP-1-----------------------";
    LOG(INFO)<<"Retest : Unpack for IO-Buffer,  RequestType : " << result_req->type();
    LOG(INFO)<<"Retest : Unpack for IO-Buffer,  UUID        : " << result_req->uuid();


    folly::IOBufQueue& queue2 = uuid_con.get_queue();

    std::unique_ptr<folly::IOBuf> buffer_front2 = queue2.pop_front();


    //Retest after pop front from queue.
    //convert from array to vector in uint8_t type.
    std::vector<uint8_t> result_vec2(buffer_front2->data(),
            buffer_front2->data() + buffer_front2->length());

    //Test unpack message to vector after message packed.
    pack_msg_req.unpack(result_vec2);

    //Read message from unpack class.
    msg_request_ptr_type  result_req2(new message_scan::RequestScan);
    result_req2 = pack_msg_req.get_msg();
    LOG(INFO)<<"-------------------POP-2----------------------";
    LOG(INFO)<<"Retest : Unpack for IO-Buffer,  RequestType : " << result_req2->type();
    LOG(INFO)<<"Retest : Unpack for IO-Buffer,  UUID        : " << result_req2->uuid();

    check_consistency(uuid_con.get_queue());

}

TEST(SecurityConnection, uuid_controller_iobuffer)
{

    typedef boost::shared_ptr<message_scan::RequestScan> msg_request_ptr_type;
		typedef internet::packedmessage_scan<message_scan::RequestScan> pack_request_type;
		typedef std::vector<pack_request_type>  pack_reqeust_vec;

    std::vector<uint8_t> msg_vec;
    std::vector<uint8_t> msg_vec_sub;

		utils::uuid_generator  uuid_gen;

    msg_request_ptr_type  msg_req(new message_scan::RequestScan);
    msg_req->set_type(message_scan::RequestScan::CLOSE_CONNECTION);
    msg_req->set_uuid(uuid_gen.generate());
    msg_req->set_timestamp("0:0:0:0");

    msg_request_ptr_type  msg_req_sub(new message_scan::RequestScan);
    msg_req_sub->set_type(message_scan::RequestScan::SCAN);
    msg_req_sub->set_uuid(uuid_gen.generate());
    msg_req_sub->set_timestamp("0:0:0:1");

		
    //pack data in msg_vec
    internet::packedmessage_scan<message_scan::RequestScan>  pack_msg_req(msg_req);
    pack_msg_req.pack(msg_vec);

    internet::packedmessage_scan<message_scan::RequestScan>  pack_msg_req_sub(msg_req_sub);
    pack_msg_req_sub.pack(msg_vec_sub);


    //UUID controller initial
    //internet::security::uuid_controller<msg_request_ptr_type> uuid_con;
    internet::security::uuid_controller<message_scan::RequestScan> uuid_con;

    //IOBuffer_scan
    std::unique_ptr<folly::IOBuf> iobuffer_scan =	uuid_con.initial_iobuf();
    //Wrapper all data to IOBuffer.(message-1)
    iobuffer_scan = folly::IOBuf::wrapBuffer(&msg_vec[0], msg_vec.size());

		for(int count_msg = 0; count_msg < 3; count_msg++){

			//Genterate io buffer 
			std::unique_ptr<folly::IOBuf> iobuffer_scan_sub = uuid_con.initial_iobuf();

			//Seconds iobuffer.	
    	iobuffer_scan_sub = folly::IOBuf::wrapBuffer(&msg_vec_sub[0], msg_vec_sub.size());
		
			iobuffer_scan->appendChain(std::move(iobuffer_scan_sub));

		}

    //Check data size match with IOBuffer contains data.
    EXPECT_EQ(iobuffer_scan->data(), msg_vec.data());
    EXPECT_EQ(iobuffer_scan->capacity(), msg_vec.size());

    uuid_con.push_msg(std::move(iobuffer_scan));

    check_consistency(uuid_con.get_queue());


    //Get Queue for invok IOBuffer list.
    folly::IOBufQueue& queue = uuid_con.get_queue();
    //If use pop_front() is pop data in queue (Delete front in queue).
    const folly::IOBuf *msg_iobuffer = queue.front();

    for(int count_list = 0;
            count_list < msg_iobuffer->countChainElements();
            count_list++) {


        std::vector<uint8_t> msg(msg_iobuffer->data(),
                msg_iobuffer->data() + msg_iobuffer->length());

		    msg_request_ptr_type  msg_req(new message_scan::RequestScan);
	
        internet::packedmessage_scan<message_scan::RequestScan>  pack_msg_req(msg_req);

        pack_msg_req.unpack(msg);
	
        msg_req =  pack_msg_req.get_msg();

        LOG(INFO)<<"MSG TYPE : " << msg_req->type();
				LOG(INFO)<<"UUID     : " << msg_req->uuid();
	
				msg_iobuffer = msg_iobuffer->next();

    }

}
*/

TEST(SecurityConnection, uuid_controller_insert_message){

    typedef boost::shared_ptr<message_scan::RequestScan> msg_request_ptr_type;

    std::vector<uint8_t> msg_vec;
    std::vector<uint8_t> msg_vec_sub;

		utils::uuid_generator  uuid_gen;

    msg_request_ptr_type  msg_req(new message_scan::RequestScan);
    msg_req->set_type(message_scan::RequestScan::REGISTER);
    msg_req->set_uuid("007c6b00-0000-0000-3cfb-410000000000");
    msg_req->set_timestamp("0:0:0:0");

    msg_request_ptr_type  msg_req_sub(new message_scan::RequestScan);
    msg_req_sub->set_type(message_scan::RequestScan::SCAN);
    msg_req_sub->set_uuid("007c6b00-0000-0000-3cfb-410000000000");
    msg_req_sub->set_timestamp("0:0:0:1");

    msg_request_ptr_type  msg_req_fraud(new message_scan::RequestScan);
    msg_req_fraud->set_type(message_scan::RequestScan::SCAN);
    //msg_req_fraud->set_uuid(uuid_gen.generate());
    msg_req_fraud->set_uuid("007c6b00-0000-0000-3cfb-410000000000");
    msg_req_fraud->set_timestamp("0:0:0:2");



	internet::security::uuid_controller<message_scan::RequestScan> uuid_conn;

	uuid_conn.get_msg_uuid(msg_req);


	uuid_conn.get_msg_uuid(msg_req_sub);


	uuid_conn.get_msg_uuid(msg_req_fraud);



}

