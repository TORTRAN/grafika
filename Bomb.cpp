class Bomb{
public:
	Point asalDuar;
	vector<Point> arrPoint;
	vector<Point> arrPoint2;
	
	Bomb(int x, int y){
		asalDuar.x = x;
		asalDuar.y = y;
		arrPoint.push_back(Point(x-30, y-26));
		arrPoint.push_back(Point(x-83, y-87));
		arrPoint.push_back(Point(x-16, y-51));
		arrPoint.push_back(Point(x+38, y-100));
		arrPoint.push_back(Point(x+24, y-57));
		arrPoint.push_back(Point(x+70, y-36));
		arrPoint.push_back(Point(x+44, y-27));
		arrPoint.push_back(Point(x+105, y+38));
		arrPoint.push_back(Point(x+29, y+20));
		arrPoint.push_back(Point(x-1, y+101));
		arrPoint.push_back(Point(x-9, y+45));
		arrPoint.push_back(Point(x-93, y+75));
		arrPoint.push_back(Point(x-42, y+27));
		arrPoint.push_back(Point(x-88, y+21));
		// for(int i = 0; i<arrPoint.size()-1; i++){
			arrPoint2 = arrPoint;

		// }
	}
	void drawBomb(){
		for(int i = 0; i < arrPoint.size()-1; i++){
			drawLine(arrPoint[i].x, arrPoint[i].y, arrPoint[i+1].x, arrPoint[i+1].y);
		}
		drawLine(arrPoint[arrPoint.size()-1].x, arrPoint[arrPoint.size()-1].y, arrPoint[0].x, arrPoint[0].y);
	}
	void reset(){
		arrPoint = arrPoint2;
	}
	void zoomIn(){
		for(int i = 0; i < arrPoint.size(); i++){
			arrPoint[i].x = ((arrPoint[i].x - asalDuar.x) * 8 / 5)+asalDuar.x;
			arrPoint[i].y = ((arrPoint[i].y - asalDuar.y)* 8 / 5)+asalDuar.y;
		}
	}
};