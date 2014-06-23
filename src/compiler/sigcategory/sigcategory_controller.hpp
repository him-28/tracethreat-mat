#ifndef COMPILER_SIGCATEGORY_CONTROLLER_HPP
#define COMPILER_SIGCATEGORY_CONTROLLER_HPP

#define MALWARE 1;
#define ROOTKIT 2;


template<typename SigDescription>
class sigcategory_controller
{

    public:
        bool& sig_classification(SigDescription *file_descp);
        bool set_sigdescp(SigDescription *sig_descp);
        bool set_sig_malware_shm(std::vector<SigDescription *>    *sig_mal);
        bool set_sig_rootkit_shm(std::vector<SigDescription *>    *sig_rk);
    private:
        std::vector<SigDescription *> *sig_descp_ptr;
        sigcategory *sigcategory_ptr;
};


template<typename SigDescription>
bool& sigcategory_controller<SigDescription>::
sig_classification(std::vector<sigcategory *>   *sigcate)
{
    //make currentcy
    sig_descp_ptr = sigcategory_ptr->detected_type_sig(sig_descp_ptr);
    std::vector<SigDescription *>::iterator iter_sig_vec;

    for(iter_size_vec = sig_descp_ptr->begin();
            iter_size_vec != sig_descp_ptr->end();
            ++iter_size_vec) {
        SigDescription *sig = *iter_size_vec;

        switch(sig->type) {
        case MALWARE :
            set_sig_malware_shm(sig_descp_ptr);
            break;

        case ROOTKIT :
            set_sig_rootkit_shm(sig_descp_ptr);
            break;

        default :
            return false;
            break;
        }

    }

    return true;
};

#endif /* COMPILER_SIGCATEGORY_CONTROLLER_HPP */
