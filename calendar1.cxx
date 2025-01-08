#include <iostream>
#include <iomanip>
#include <chrono>

#include <ctime>
#include <sstream>
#include <string.h>

/*
cxxdroid uses c++17
raspberry pi gcc uses c++17
*/

#define JAN "January"
#define FEB "February"
#define MAR "March"
#define APR "April"
#define MAY "May"
#define JUN "June"
#define JUL "July"
#define AUG "August"
#define SEP "September"
#define OCT "October"
#define NOV "November"
#define DEC "December"

#define MON "M"
// "Monday"
#define TUE "T"
// "Tuesday"
#define WED "W"
//Wednesday"
#define THU "T"
//Thursday"
#define FRI "F"
//Friday"
#define SAT "S"
//Saturday"
#define SUN "S"
//Sunday" 

using namespace std;

enum class Month
{ Jan=1, Feb, Mar, Apr, May, Jun, Jul, Aug,
Sep, Oct, Nov, Dec };

enum class Day
{ Sun=0, Mon, Tue, Wed, Thu, Fri, Sat };

/*
class Date
includes output to cout
can add days, months, years
and takes leap years into account
*/
	
class Date
{
	public:
	class Invalid{};
	Date();
	Date(int yy, Month mm, int dd):y{yy}, m{mm}, d{dd}
	{
		if(!is_valid(yy,mm,dd)) throw Invalid{};
	}
	
	int day() const { return d; };
	Month month() const { return m; };
	int year() const { return y; };
	int days() const { return days_in_month; };
	
	void add_day();
	void add_month();
	void add_year(int n);
		
	private:
	int y{2001};
	Month m{Month::Jan};
	int d{1};
	int days_in_month{0};
	
	bool is_valid(int y, Month m, int d);
	bool leapyear(int y);
};

const Date& default_date()
{
	static Date dd {2001, Month::Jan, 1};
	return dd;
}

Date::Date() : y{default_date().year()},
	m{default_date().month()},
	d{default_date().day()}
{
	
}

void Date::add_day()
{
	d++;
	
	if(d>days_in_month)
	{
		// d first 31>30
		d = 1;
		add_month();
	}
}

void Date::add_month()
{
	if(m == Month::Dec)
	{
		m = Month::Jan;
		//do not change day
		add_year(1);
	}
	else if(m == Month::Jan && d == 29 && !leapyear(y))
	{
		// enums might be the wrong choice
		m = (Month)((int)m+1);
		d = 28;
	}
	else
	{
		m = (Month)((int)m+1);
	}
	// double check
	if(!is_valid(y,m,d)) throw Invalid{}; 
}

void Date::add_year(int n)
{
	if(m == Month::Feb && d == 29
		&& !leapyear(y+n))
	{
		m = Month::Mar;
		d = 1;
	}
	y += n;
}

/*
change / create a date -> is valid
useful
*/
bool Date::is_valid(int y, Month m, int d)
{
	if(d<0) return false;
	if(m<Month::Jan || Month::Dec < m)
		return false;
		
	days_in_month = 31;
	
	switch(m)
	{
	case Month::Feb:
	{
		days_in_month = (leapyear(y)) ? 29 : 28;
		break;
	}
	case Month::Apr: case Month::Jun:
	case Month::Sep: case Month::Nov:
	{
		days_in_month = 30;
		break;
	}
	}
	
	if(days_in_month<d) return false;
	
	return true;
}

bool Date::leapyear(int y)
{
	return (y%4 == 0 && y%100 != 0) || y%400 == 0;
}

//
// operators
//

ostream& operator<<(ostream& os, const Date&d)
{
	return os << '(' << d.day()
					<< ',' << int(d.month())
					<< ',' << d.year()
					<< ')';
}
// end of Date class


//
// time point
//

/*
check mktime still gives a valid date
make a new Date from puttime and check
would a standard library function go wrong?

if getDayOfWeek returns an int
if tm returns possibly -1
can i link back to a date?
*/

tm makeTM(
const Date date)
{
	// must initialise with {} or it is garbage
	tm get_tm{};
	string sd;
	int d = date.day();
	int m = (int)date.month();
	int y = date.year();
	
	sd += to_string(y)+"-";
	sd += to_string(m)+"-";
	sd += to_string(d);
	
	istringstream ss{sd};
	ss >> get_time(&get_tm, "%y-%m-%d");
	// a crappy check at the moment
	
	
	// works with this
	mktime(&get_tm);
	
	return get_tm;
}

// use chrono to find Mon-Sun
// from Date string
int getDayOfWeek(const Date date)
{
	tm get_day = makeTM(date);
	// we dont check it the return works

	stringstream grab_number;
	grab_number << put_time(&get_day, "%w");
	string sday = grab_number.str();
	int iday = stoi(sday);
	
	return iday;
}

//
// print
//

string printMonth(const Date& d)
{
	switch(d.month())
	{
	case Month::Jan: return JAN;
	case Month::Feb: return FEB;
	case Month::Mar: return MAR;
	case Month::Apr: return APR;
	case Month::May: return MAY;
	case Month::Jun: return JUN;
	case Month::Jul: return JUL;
	case Month::Aug: return AUG;
	case Month::Sep: return SEP;
	case Month::Oct: return OCT;
	case Month::Nov: return NOV;
	case Month::Dec: return DEC;
	default: return "mmm";
	}
}

void printHeader(
const Date date)
{
	// my console wasn't working
	int mfs = 6;
	int tt = 7;
	int ws = 9;
	mfs = 6;
	tt = 6;
	ws = 6;
	
	cout << "==========" << endl;
	tm title = makeTM(date);
	cout << asctime(&title);
	cout << "==========" << endl;
	
	cout << "|";
	cout << setfill('X');
	cout << setw(3)<< SUN << "|";
	cout << setw(3) << MON << "|";
	cout << setw(3)<< TUE << "|";
	cout << setw(3)<< WED << "|";
	cout << setw(3)<< THU << "|";
	cout << setw(3)<< FRI << "|";
	cout << setw(3)<< SAT << "|";
	
	cout << "\n\n";
}

void printWeek(const Date date)
{
	// we want the beginning of the month
	Date begin { date.year(),
		date.month(),
		1 };
	// which enum?
	int first_day = getDayOfWeek(begin);
	Day day = (Day)((int)first_day);
	
	cout << "|";
	
	// fill last month
	for(int i=0; i < first_day; ++i)
	{
		cout << setw(3) << setfill('X') << "X" << "|";
	}
	
	// number days
	for(int i=0; i<begin.days();++i)
	{
		cout << setw(3)<< setfill('X')
			<< begin.day() << "|";
		
		switch(day)
		{
		case Day::Sun: day = Day::Mon; break;
		case Day::Mon: day = Day::Tue; break;
		case Day::Tue: day = Day::Wed; break;
		case Day::Wed: day = Day::Thu; break;
		case Day::Thu: day = Day::Fri; break;
		case Day::Fri: day = Day::Sat; break;
		case Day::Sat:
		{
			day = Day::Sun;
			cout << endl << "|";
		}
		}
		
		begin.add_day();
		if(begin.day() > begin.days()) break;
	}
	
	Date finish {date.year(),
		date.month(),
		date.days()};
		
	int zero_sunday = 6;
	int end = zero_sunday - getDayOfWeek(finish);

	for(int i = 0; i < end; ++i)
	{
		cout << setw(3)<< setfill('X') << "X" << "|";
	}
}


//
// main
//

int main(int argc, char *argv[])
{
	chrono::time_point then{
			chrono::system_clock::now()};	
	time_t t =
		chrono::system_clock::to_time_t(then);
		
	time_t t2 = 0;
		
	Date date{2025, Month::Jan, 8};
	
	//
	// make the calendar
	//
	
	printHeader(date);
	printWeek(date);
	
	// fast forward stuff
	
	int count = 10;
	while(count>11)
	{
		chrono::time_point now{
			chrono::system_clock::now()};
		t2 = chrono::system_clock::to_time_t(now);
		
		if(t2>t)
		{
			t = t2;
			date.add_day();
			cout << date;
			
			cout << endl; // endl for ostream
		}
		
		// no --count
	}
	
	cout << "\n\n";
	
	return 0;
}