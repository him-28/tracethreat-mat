#ifndef _THRIFT_CONCURRENCY_MUTEX_H_
#define _THRIFT_CONCURRENCY_MUTEX_H_ 1

/*
* Copyright 2014 MTSec, Inc.
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
* http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*/

/*  Titles			                                                     Authors	          Date
 *- Thirf concurrency licenses unders Apahce License Version 2.0
 *                                                                 R.chatsiri     24/03/2014
 */



#include <boost/shared_ptr.hpp>
#include <boost/noncopyable.hpp>


namespace controller
{

    class ReadWriteMutex
    {
        public:
            ReadWriteMutex();
            virtual ~ReadWriteMutex() {}

            // these get the lock and block until it is done successfully
            virtual void acquireRead() const;
            virtual void acquireWrite() const;

            // these attempt to get the lock, returning false immediately if they fail
            virtual bool attemptRead() const;
            virtual bool attemptWrite() const;

            // this releases both read and write locks
            virtual void release() const;

        private:

            class impl;
            boost::shared_ptr<impl> impl_;
    };

    /**
     * A ReadWriteMutex that guarantees writers will not be starved by readers:
     * When a writer attempts to acquire the mutex, all new readers will be
     * blocked from acquiring the mutex until the writer has acquired and
     * released it. In some operating systems, this may already be guaranteed
     * by a regular ReadWriteMutex.
     */
    class NoStarveReadWriteMutex : public ReadWriteMutex
    {
        public:
            NoStarveReadWriteMutex();

            virtual void acquireRead() const;
            virtual void acquireWrite() const;

        private:
            Mutex mutex_;
            mutable volatile bool writerWaiting_;
    };

    class Guard : boost::noncopyable
    {
        public:
            Guard(const Mutex& value, int64_t timeout = 0) : mutex_(&value) {
                if (timeout == 0) {
                    value.lock();
                } else if (timeout < 0) {
                    if (!value.trylock()) {
                        mutex_ = NULL;
                    }
                } else {
                    if (!value.timedlock(timeout)) {
                        mutex_ = NULL;
                    }
                }
            }
            ~Guard() {
                if (mutex_) {
                    mutex_->unlock();
                }
            }

            operator bool() const {
                return (mutex_ != NULL);
            }

        private:
            const Mutex *mutex_;
    };

    // Can be used as second argument to RWGuard to make code more readable
    // as to whether we're doing acquireRead() or acquireWrite().
    enum RWGuardType {
        RW_READ = 0,
        RW_WRITE = 1
    };


    class RWGuard : boost::noncopyable
    {
        public:
            RWGuard(const ReadWriteMutex& value, bool write = false)
                : rw_mutex_(value) {
                if (write) {
                    rw_mutex_.acquireWrite();
                } else {
                    rw_mutex_.acquireRead();
                }
            }

            RWGuard(const ReadWriteMutex& value, RWGuardType type)
                : rw_mutex_(value) {
                if (type == RW_WRITE) {
                    rw_mutex_.acquireWrite();
                } else {
                    rw_mutex_.acquireRead();
                }
            }
            ~RWGuard() {
                rw_mutex_.release();
            }
        private:
            const ReadWriteMutex& rw_mutex_;
    };



}


#endif // #ifndef _THRIFT_CONCURRENCY_MUTEX_H_
