#ifndef HASHMAP_OCL__HPP
#define HASHMAP_OCL__HPP

/*                       Titles                                          Authors                        Date
 *- Hash method for sending to OCL                                       R.Chatsiri                     15/01/2014
 */
#include <iostream>

template<typename T, typename T2>
class ihash
{
    public:
        virtual bool push(T *key, T2 *value) = 0;
        virtual T&   get_value(T *key) = 0;
        virtual bool remove(T *key) = 0;
};


template<typename T, typename T2>
class hash_generate_sample : public ihash<T,T2>
{
		public:


		private:

};


template<typename T, typename T2>
struct MethodHashType {
    typename class hash_generate_sample<T,T2>;
    typename class hash_jarray<T,T2>;
}



template<typename T, typename T2 
typename MethodHashType = struct MethodHashType::hash_generate_sample<T, T2> >
class hashmap_ocl
{

    public:


    private:

        MethodHashType  hash_gtype;

};

#endif
