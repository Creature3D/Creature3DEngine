#ifndef CRE_EXCEPTION
#define CRE_EXCEPTION 1

#include <string>

namespace cre{

class crException{
public:
	crException(std::string error);
	~crException();
	std::string getError(){return m_error;};
private:
	std::string m_error;
};

}

#endif
