#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <string.h>



struct Point {
	char* title;
	double x, y;

	/* 1 */ Point() : title{ NULL }, x{ 0 }, y{ 0 } {}
	/* 2 */ Point(Point&& other) : x{ other.x }, y{ other.y }, title{ other.title }  {
		other.title = NULL;
		//printf("Move constructor\n");
	}
	/* 3 */ Point(const Point& other) : x{ other.x }, y{ other.y }, title{ NULL } {
		this->setTitle(other.title);
	}
	/* 4 */ Point(double x, double y, char* title = NULL) : x{ x }, y{ y }, title{ NULL } {
		if (title) setTitle(title);
	}
	/* 5 */ ~Point() {
		if (title) delete[] title;
		//printf("Destructor\n");
	}
	/* 6 */ Point& operator = (const Point& other) {
		this->x = other.x; // OK
		this->y = other.y; // OK
		// this->title = other.title; // NO
		this->setTitle(other.title);
		//printf("Copy =\n");
		return *this;
	}
	/* 7 */ Point& operator = (Point&& other)  {
		this->x = other.x; // OK
		this->y = other.y; // OK
		this->title = other.title; // OK
		// this->setTitle( other.title ) ; // NO
		other.title = NULL;
		//printf("Move =\n");
		return *this;
	}

	void setTitle(const char* title) {
		if (this->title) delete[] this->title;
		this->title = new char[strlen(title) + 1];
		strcpy(this->title, title);
	}

	Point& operator ++ () {
		this->x++;
		this->y++;
		return *this;
	}

	Point operator - () const{

		return Point(-x, -y);
	}

	Point operator + () const {

		return Point(x, y);
	}

	friend const Point operator ++ (Point& tmp)  {
		tmp.x++;
		tmp.y++;
		return Point(tmp.x, tmp.y);
	}
	friend const Point operator ++ (Point& tmp, int) {
		Point oldValue(tmp.x, tmp.y);
		tmp.x++;
		tmp.y++;
		return oldValue;
	}

	friend const Point& operator -- (Point& tmp) {
		tmp.x--;
		tmp.y--;
		return Point(tmp.x, tmp.y);
	}

	friend const Point operator -- (Point& tmp, int) {
		Point oldValue(tmp.x, tmp.y);
		tmp.x--;
		tmp.y--;
		return Point(tmp.x, tmp.y);
	}

	friend const Point operator+(const Point& left, const Point& right) {
		return Point(left.x + right.x, left.y+right.y);
	}

	friend const Point operator-(const Point& left, const Point& right) {
		return Point(left.x - right.x, left.y - right.y);
	}


	friend Point& operator+=(Point& left, const Point& right) {
		left.x += right.x;
		left.y += right.y;
		return left;
	}

	friend Point& operator+=(Point& left, const Point& right) {
		left.x -= right.x;
		left.y -= right.y;
		return left;
	}

	friend Point operator==(const Point& left, const Point& right) {
		left.x == right.x;
		left.y == right.y;
		return left;
	}
};





int main() {

	Point p1, p2;
	p1.setTitle("point A");
	Point p3 = p1; // copy constructor
	p2 = p1 = p3; // copy =

	p1.x = 2;
	p1.y = 2;
	/*p1.x++;
	p1.y--;
	p1.x-=3;*/
	p2.x = 5;
	p2.y = 5;
	Point p4 = p2 + p1;
	std::cout << p4.x << ' ' << p4.y << std::endl;
	p2++;
	std::cout << std::endl;
	std::cout << p3.x << ' ' << p3.y << std::endl;
	std::cout << -p1.x << ' ' << -p1.y << std::endl;
	std::cout << p1.x << ' ' << p1.y << std::endl;

	/* Point(без &) operator = (...) ->
	p1 = p3 р3 - объект не-временный, выбираетс€ Copy=
	{ ... return *this; --> создаетс€ копи€ (1) в область возврата

	p2 = ... <-- (1) (1)-временный объект, поэтому выбираетс€ Move=
	{ return *this; --> создаетс€ копи€ (2) в область возврата:
	(2) <-- p2 = p1 = p3
	конец операции - удал€ютс€ временные объекты (1) и (2) - два деструктора
	*/
	/* Point& operator = (...) -> возврат по ссылке
	p1 = p3 р3 - объект не-временный, выбираетс€ Copy=
	{ ... return *this; --> т.к. возврат по ссылке, происходит "св€зывание"
	возвращаемого значени€ и точки возврата (без создани€ временного объекта)
	operator = (const Point& other <-- *this
	поскольку временного объекта нет, выбираетс€ Copy=, а также
	нет вызова дополнительных деструкторов

	p2 = p1 = p3 --> =(p2, =(p1, p3))

	=(p1, p3)
	|
	=(p2, V )

	!! по ссылке нельз€ передавать временные объекты:
	Point operator + (Point p1, Point p2) { | в такой форме - Point& operator... нельз€
	Point res;
	res.set...
	return res; // возврат временного объекта | если передавать по ссылке - выбираетс€ Copy=
	} // здесь включитс€ его деструктор | но деструктор все равно сработает

	вызов без ссылки приведет к выбору Move=, позвол€€ более эффективно забрать ресурсы
	*/


	return 0;
}