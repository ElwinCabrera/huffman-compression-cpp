#ifndef EXCEPTION_H
#include EXCEPTION_H


class GenericException : public std::exception {
private:
    const char *message;
    int status_code;

public:
    //GenericException(int status_code=-1) {}
    GenericException(const char *msg="", int status_code=-1) : message(msg), status_code(status_code){}
    
    virtual const char* what () {
        std::cout << "Exception occured" << std::endl;
        std::cout << "Last errno saved: " << errno << " -> " << std::strerror(errno) << std::endl;
        return message;
    }
};



#endif