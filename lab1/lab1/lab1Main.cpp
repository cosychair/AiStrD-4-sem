//Вариант 8, фигура 9(прямоугольный треугольник), расположение 1 10(отражена влево) 11(отражена вправо) 12

#include "shape.h"
// Пример добавки: дополнительный фрагмент - полуокружность
class h_circle : public rectangle, public reflectable {
	bool reflected;
public:
	h_circle(point a, point b, bool r = true) : rectangle(a, b), reflected(r) { }
	void draw();
	void flip_horisontally() { };	// Отразить горизонтально
	void flip_vertically() { reflected = !reflected; };	// Отразить вертикально
};

void h_circle::draw() //Алгоритм Брезенхэма для окружностей
{
	int x0 = (sw.x + ne.x) / 2;
	int y0 = reflected ? sw.y : ne.y;
	int radius = (ne.x - sw.x) / 2;
	int x = 0;
	int y = radius;
	int delta = 2 - 2 * radius;
	int error = 0;
	while (y >= 0) {
		if (reflected) { put_point(x0 + x, y0 + y * 0.7); put_point(x0 - x, y0 + y * 0.7); }
		else { put_point(x0 + x, y0 - y * 0.7); put_point(x0 - x, y0 - y * 0.7); }
		error = 2 * (delta + y) - 1;
		if (delta < 0 && error <= 0) { ++x; delta += 2 * x + 1; continue; }
		error = 2 * (delta - x) - 1;
		if (delta > 0 && error > 0) { --y; delta += 1 - 2 * y; continue; }
		++x; delta += 2 * (x - y);  --y;
	}
}

// Пример добавки: дополнительная функция присоединения…
void down(shape &p, const shape &q)
{
	point n = q.south();
	point s = p.north();
	p.move(n.x - s.x, n.y - s.y - 1);
}


// Прямоугольный треугольник
class RightTriangle : public rectangle, public reflectable {
protected:
	enum position {upLeft, upRight, downRight, downLeft} pos;
public:
	//ne - up, sw - down 
	RightTriangle(point sw, point ne) :pos(upLeft),rectangle(sw, ne){}
	void draw();
	void flip_horisontally() {
		ne.y += sw.y;
		sw.y = ne.y - sw.y;
		ne.y = ne.y - sw.y;
		pos = position((pos + 2) % 4);
	}	// Отразить горизонтально
	void flip_vertically() {
		ne.x += sw.x;
		sw.x = ne.x - sw.x;
		ne.x = ne.x - sw.x;
		pos = position((pos + 2) % 4);
	}	// Отразить вертикально
	void rotate_right() // Поворот вправо относительно se
	{
		rectangle::rotate_right();
		pos = position((pos + 1) % 4);
	}
	void rotate_left() // Поворот влево относительно sw
	{
		rectangle::rotate_left();
		pos = position((pos + 3) % 4);
	}
};
void RightTriangle::draw() {
	switch (pos)
	{
	case RightTriangle::upLeft: 
		put_line(nwest(), ne);
		put_line(sw, nwest());
		put_line(ne, sw);
		break;
	case RightTriangle::upRight:
		put_line(nwest(), ne);
		put_line(ne, seast());
		put_line(nwest(), seast());
		break;
	case RightTriangle::downRight:
		put_line(ne, seast());
		put_line(sw, seast());
		put_line(ne, sw);
		break;
	case RightTriangle::downLeft:
		put_line(nwest(), sw);
		put_line(sw, seast());
		put_line(nwest(), seast());
		break;
	default:
		break;
	}
}
// Cборная фигура - физиономия
class myshape : public rectangle {
	//Моя фигура ЯВЛЯЕТСЯ прямоугольником
	int w, h;
	line l_eye; // левый глаз – моя фигура СОДЕРЖИТ линию
	line r_eye; // правый глаз
	line mouth; // рот
	RightTriangle tr1, tr2;
public:
	myshape(point, point);
	void draw();
	void move(int, int);
	void resize(int) { }
};
myshape::myshape(point a, point b)
	: rectangle(a, b),
	w(neast().x - swest().x + 1),
	h(neast().y - swest().y + 1),
	l_eye(point(swest().x + 2, swest().y + h * 3 / 4), 2),
	r_eye(point(swest().x + w - 4, swest().y + h * 3 / 4), 2),
	mouth(point(swest().x + 2, swest().y + h / 4), w - 4),
	tr1(point(0,0), point(5, 5)),
	tr2(point(0, 0), point(5, 5))
{
	tr2.rotate_left();
	tr2.rotate_left();
	down(tr1, l_eye);
	down(tr2, r_eye);
}//предложили оригинальное решение напиши в выводазх


void myshape::draw()
{
	rectangle::draw();
	int a = (swest().x + neast().x) / 2;
	int b = (swest().y + neast().y) / 2;
	put_point(point(a, b));
}
void myshape::move(int a, int b)
{
	rectangle::move(a, b);
	l_eye.move(a, b);
	r_eye.move(a, b);
	mouth.move(a, b);
}

int main() {
	screen_init();
	//== 1.Объявление набора фигур ==
	RightTriangle tr(point(18, 0), point(25, 5));
	RightTriangle tr1(point(0, 0), point(8, 5));

	rectangle hat(point(0, 0), point(28, 10));
	line brim(point(0, 30), 34);
	myshape face(point(30, 20), point(54, 36));
	//h_circle beard(point(40, 10), point(50, 20));
	shape_refresh();
	std::cout << "=== Generated... ===\n";
	std::cin.get(); //Смотреть исходный набор
//== 2.Ориентация ==
	hat.rotate_right();
	brim.resize(2);
	face.resize(2);
	//beard.flip_vertically();
	shape_refresh();
	std::cout << "=== Prepared... ===\n";
	std::cin.get(); //Смотреть ориентацию
//== 3.Сборка изображения ==
	up(brim, face);
	up(hat, brim);
	down(tr, face);
	up(tr1, brim);
	shape_refresh();
	std::cout << "=== Ready! ===\n";
	std::cin.get(); //Смотреть результат
	screen_destroy();
}