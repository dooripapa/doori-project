#include <iostream>
#include <ostream>
#include <ios>

using namespace std;


class mylog
{
	public:

	enum class LEVEL{
		F,
		W,
		I,
		D
	};

	mylog() : level{LEVEL::D}, userLevel{LEVEL::D},os{cout} {};

	template<typename T>
	mylog& operator<<(T args);

	template<typename T>
	mylog& operator[](T);

	mylog& operator<<(ostream& (*manip)(ostream&));

	void setLevel(LEVEL level) {
		this->userLevel = level;
	}

	private:
	LEVEL level;
	LEVEL userLevel;
	ostream& os;

};

template<typename T>
mylog& mylog::operator<<(T args)
{
	if( level <= userLevel)
	{
		this->os<<args;
	}
	return *this;
}

template<typename T>
mylog& mylog::operator[](T t) {
	level = t;
	return *this;
}

mylog& mylog::operator<<(ostream& (*manip)(ostream&)) {
	if( level <= userLevel)
	{
		this->os << manip;
	}
    return *this;
}

