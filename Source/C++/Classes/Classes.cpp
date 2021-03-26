

#include "Classes.h"



std::vector<i32> identitys;
std::vector<void*> ptrs;

extern sur::Map_Analyses _Amap;
//
//	Master
//
void sur::Master::Move(sur::Vec2 direction, bool detect)
{
	if (detect) {
		i32 CurMove = 0;
		if (direction.y > 0) {	//Up
			for (i32 a = 1; a <= direction.y; a++) {
				for (i32 i = position.x; i < position.x + size.x; i++) {
					for (i32 j = 0; j < identitys.size(); j++) {				
						if (_debug)
							_Amap.Render(i, position.y - a - 1, Color(0, 255, 0));
						if (_Amap.Collider(i, position.y - a - 1) == identitys[j] && this->id != identitys[j]) {
							if (callback != nullptr)
								callback(static_cast<Master*>(ptrs[j]));
							if (static_cast<Master*>(ptrs[j])->callback != nullptr)
								static_cast<Master*>(ptrs[j])->callback(this);
							goto dir1;
						}	
					}
				}
				CurMove = a;
			}
		dir1:
			MoveInject(1, CurMove);
		}
		if (direction.x > 0) {	//Right
			for (i32 a = 1; a <= direction.x; a++) {
				for (i32 i = position.y; i < position.y + size.y; i++) {
					for (i32 j = 0; j < identitys.size(); j++) {
						if (_debug)
							_Amap.Render(position.x + size.x + a - 1, i, Color(0, 255, 0));
						if (_Amap.Collider(position.x + size.x + a - 1, i) == identitys[j] && this->id != identitys[j]) {
							if (callback != nullptr)
								callback(static_cast<Master*>(ptrs[j]));
							if (static_cast<Master*>(ptrs[j])->callback != nullptr)
								static_cast<Master*>(ptrs[j])->callback(this);
							goto dir2;
						}			
					}
				}
				CurMove = a;
			}
		dir2:
			MoveInject(2, CurMove);
		}
		if (direction.y < 0) {	//Down
			direction.y *= -1;
			for (i32 a = 0; a <= direction.y; a++) {
				for (i32 i = position.x; i < position.x + size.x; i++) {
					for (i32 j = 0; j < identitys.size(); j++) {			
						if (_debug)
							_Amap.Render(i, position.y + size.y + a, Color(0, 255, 0));
						if (_Amap.Collider(i, position.y + size.y + a) == identitys[j] && this->id != identitys[j]) {
							if (callback != nullptr)
								callback(static_cast<Master*>(ptrs[j]));
							if (static_cast<Master*>(ptrs[j])->callback != nullptr)
								static_cast<Master*>(ptrs[j])->callback(this);
							goto dir3;
						}
					}
				}
				CurMove = a;
			}
		dir3:
			MoveInject(3, CurMove);
		}
		if (direction.x < 0) {	//Left
			direction.x *= -1;
			for (i32 a = 1; a <= direction.x; a++) {
				for (i32 i = position.y; i < position.y + size.y; i++) {
					for (i32 j = 0; j < identitys.size(); j++) {
						if (_debug)
							_Amap.Render(position.x - a - 1, i, Color(0, 255, 0));
						if (_Amap.Collider(position.x - a - 1, i) == identitys[j] && this->id != identitys[j]) {
							if (callback != nullptr)
								callback(static_cast<Master*>(ptrs[j]));
							if (static_cast<Master*>(ptrs[j])->callback != nullptr)
								static_cast<Master*>(ptrs[j])->callback(this);
							goto dir4;
						}
					}
				}
				CurMove = a;
			}
		dir4:
			MoveInject(4, CurMove);
		}
	}
	else {
		if (direction.y > 0)
			position.y -= direction.y;
		if (direction.x > 0)
			position.x += direction.x;
		if (direction.y < 0)
			position.y -= direction.y;
		if (direction.x < 0)
			position.x += direction.x;
	}
}

void sur::Master::MoveInject(i32 index, i32 CurMove)
{
	switch (index)
	{
	case 1:
		position.y -= CurMove;
		break;
	case 2:
		position.x += CurMove;
		break;
	case 3:
		position.y += CurMove;
		break;
	case 4:
		position.x -= CurMove;
		break;
	}
}
//
//	Render
//
void sur::Render::ClearScreenBuffer()
{
	std::fill(_map.RenderMap, _map.RenderMap + _window_size.x * _window_size.y, bg.ToCOLORREF());
	std::fill(_map.ColliderMap, _map.ColliderMap + _window_size.x * _window_size.y, 0);
	std::fill(_map.TriggerMap, _map.TriggerMap + _window_size.x * _window_size.y, 0);
}

void sur::Render::RenderScreenBuffer()
{
	BITMAPINFO bi = { 0 };
	bi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	bi.bmiHeader.biWidth = _window_size.x;
	bi.bmiHeader.biHeight = _window_size.y;
	bi.bmiHeader.biPlanes = 1;
	bi.bmiHeader.biBitCount = 32;
	bi.bmiHeader.biCompression = BI_RGB;
	sleep_for(milliseconds(Wait));
	SetDIBitsToDevice(dc, 0, 0,_window_size.x, _window_size.y, 0, 0, 0, _window_size.x, _map.RenderMap, &bi, 0);
	frameCounter++;
}

void sur::Render::FPS()
{
	if (!thread) {
		thread = true;
		std::thread t(&sur::Render::FPS, this);
		t.detach();
		return;
	}
	std::string Buffer;
	char* title = new char[100];
	GetWindowTextA(_hwnd, title, 100);
	while (true) {		
		sleep_for(seconds(1));
		Buffer += title;
		Buffer += " FPS: ";
		Buffer += std::to_string(frameCounter);
		SetWindowTextA(_hwnd, Buffer.c_str());
		Buffer = "";
		frameCounter = 0;
	}
}

void sur::Render::DebugConsole(bool Show)
{
	if (!Show)
		ShowWindow(GetConsoleWindow(), SW_HIDE);
}
//
//	Camera
//
void sur::Camera::Move(sur::Vec2 direction)
{
	for (i32 i = 0; i < sur::Instancer::GetCount(sur::Instancer::Types::Rectangle); i++) {
		sur::Instancer::GetRect("", i)->Move(direction.invert(), false);
	}
	for (i32 i = 0; i < sur::Instancer::GetCount(sur::Instancer::Types::Line); i++) {
		sur::Instancer::GetLine("", i)->Move(direction.invert(), false);
	}
	for (i32 i = 0; i < sur::Instancer::GetCount(sur::Instancer::Types::Obj); i++) {
		sur::Instancer::GetObj("", i)->Move(direction.invert(), false);
	}
}
//
//	Rectangle
//
sur::Rectangle::Rectangle(sur::Vec2 position, sur::Vec2 size, Color color, const std::string& name, i32 id, cb_ptr<Master*> callback)
	:color(color), Master(name,id, position, size, callback)
{
	identitys.push_back(id);
	ptrs.push_back(this);
}


void sur::Rectangle::Bind(bool Collider)
{
	for (i32 i = position.y; i < position.y + size.y; i++)
		for (i32 j = position.x; j < position.x + size.x; j++) {
			_Amap.Render(j, i, color.ToCOLORREF());
			if (Collider)
				_Amap.Collider(j - CO, i - CO, id);
		}
}
//
//	Line
//
sur::Line::Line(sur::Vec2 start, sur::Vec2 end, Color color, const std::string& name, i32 id, cb_ptr<Master*> callback)
	: start(start), end(end), color(color), Master(name, id, callback)
{
	identitys.push_back(id);
	ptrs.push_back(this);
}

void sur::Line::Bind(bool Collider)
{
	i32 lcounter = 0;
	sur::Vec2 end = this->end;
	sur::Vec2 start = this-> start;
	if (start.x == end.x) start.x--;
	if (this->start.y > this->end.y && this->start.x > this->end.x) {
		std::swap(start.y, end.y);
		std::swap(start.x, end.x);
	}
	i32 Dx, Dy;
	float RunsThrough;
	Dx = end.x - start.x;
	Dy = end.y - start.y;
	RunsThrough = (float)Dy / (float)Dx;
	if (RunsThrough >= 0) {
		i32 tempy = start.y;
		float counter = 0.0f;
		i32 countcounter = 1;
		for (i32 i = start.x; i <= end.x; i++) {
			_Amap.Render(i, tempy, color.ToCOLORREF());
			if(Collider)
				_Amap.Collider(i - CO, tempy - CO, id);	
			while (counter >= countcounter) {
				tempy++;
				_Amap.Render(i, tempy, color.ToCOLORREF());
				if (Collider)
					_Amap.Collider(i - CO, tempy - CO, id);
				countcounter++;
			}
			counter += RunsThrough;
		}
	}
	else {
		RunsThrough *= -1;
		i32 tempy = start.y;
		float counter = 0.0f;
		i32 countcounter = 1;
		bool runned = false;
		for (i32 i = start.x; i <= end.x; i++) {
			_Amap.Render(i, tempy, color.ToCOLORREF());
			if (Collider)
				_Amap.Collider(i - CO, tempy - CO, id);
			while (counter >= countcounter) {
				tempy--;
				_Amap.Render(i, tempy, color.ToCOLORREF());
				if (Collider)
					_Amap.Collider(i - CO, tempy - CO, id);
				countcounter++;
			}
			runned = true;	
			counter += RunsThrough;
		}
		if (!runned) {
			RunsThrough = (float)Dx / (float)Dy;
			RunsThrough *= -1;
			i32 tempx = start.x;
			for (i32 i = start.y; i <= end.y; i++) {
				_Amap.Render(tempx, i, color.ToCOLORREF());
				if(Collider)
					_Amap.Collider(tempx - CO, i - CO, id);
				while (counter >= countcounter) {
					tempx--;
					_Amap.Render(tempx, i, color.ToCOLORREF());
					if (Collider)
						_Amap.Collider(tempx - CO, i - CO, id);
					countcounter++;
				}
				counter += RunsThrough;
			}
		}
	}
}
//
//	Custom wire shape
//
void sur::Shape::Gen()
{
	for (i32 i = 0; i < vec->size() - 1; ++i) {
		lines->push_back(new Line(vec->at(i), vec->at(i + 1), color, "ShapeLine" + std::to_string(i), id));
	}
}

void sur::Shape::Bind(bool Collider)
{
	for (auto&& it : *lines)
		it->Bind(Collider);
}
//
//	Instancer
//
void sur::Instancer::Add(void* object, Types type)
{
	switch (type) {
	case Types::Rectangle:
		restricted::rectangles->push_back(static_cast<Rectangle*>(object));
		break;
	case Types::Line:
		restricted::lines->push_back(static_cast<Line*>(object));
		break;
	case Types::Obj:
		restricted::objects->push_back(static_cast<Object*>(object));
		break;
	case Types::Triangle:
		restricted::triangles->push_back(static_cast<Triangle*>(object));
		break;
	}
}

sur::Rectangle* sur::Instancer::GetRect(const std::string& name, i32 index)
{
	if (name != "") {
		for (auto&& it : *restricted::rectangles)
			if (it->GetName() == name && it->State()) return it;
	}
	else if (index >= 0 && index < restricted::rectangles->size() && restricted::rectangles->at(index)->State()) 
		return restricted::rectangles->at(index);
	return restricted::Rdefault;
}

sur::Line* sur::Instancer::GetLine(const std::string& name, i32 index)
{
	if (name != "") {
		for (auto&& it : *restricted::lines)
			if (it->GetName() == name && it->State()) return it;
	}
	else if (index >= 0 && index < restricted::lines->size() && restricted::lines->at(index)->State()) 
		return restricted::lines->at(index);
	return restricted::Ldefault;
}

sur::Object* sur::Instancer::GetObj(const std::string& name, i32 index)
{
	if (name != "") {
		for (auto&& it : *restricted::objects)
			if (it->GetName() == name && it->State()) return it;
	}
	else if (index >= 0 && index < restricted::objects->size() && restricted::objects->at(index)->State())
		return restricted::objects->at(index);
	return restricted::Odefault;
}

sur::Triangle* sur::Instancer::GetTri(const std::string& name, i32 index)
{
	if (name != "") {
		for (auto&& it : *restricted::triangles)
			if (it->GetName() == name && it->State()) return it;
	}
	else if (index >= 0 && index < restricted::triangles->size() && restricted::triangles->at(index)->State()) 
		return restricted::triangles->at(index);
	return restricted::Tdefault;
}

i32 sur::Instancer::GetCount(Types type) {
	switch (type) {
	case Types::Rectangle:
		return (i32)restricted::rectangles->size();
	case Types::Line:
		return (i32)restricted::lines->size();
	case Types::Obj:
		return (i32)restricted::objects->size();
	default:
		return -1;
	}
}

void sur::Instancer::State(Types type,bool active, const std::string& name, i32 index)
{
	if (type == Types::Rectangle)
		if (name != "") {
			for (i32 j = 0; j < restricted::rectangles->size(); ++j)
				if (restricted::rectangles->at(j)->GetName() == name)
					restricted::rectangles->at(j)->active = active;
		}
		else if (index >= 0)
			restricted::rectangles->at(index)->active = active;
	if (type == Types::Line)
		if (name != "") {
			for (i32 j = 0; j < restricted::lines->size(); ++j)
				if (restricted::lines->at(j)->GetName() == name)
					restricted::lines->at(j)->active = active;
		}
		else if (index >= 0)
			restricted::lines->at(index)->active = active;
	if (type == Types::Obj)
		if (name != "") {
			for (i32 j = 0; j < restricted::objects->size(); ++j)
				if (restricted::objects->at(j)->GetName() == name)
					restricted::objects->at(j)->active = active;
		}
		else if (index >= 0)
			restricted::objects->at(index)->active = active;
	if (type == Types::Triangle)
		if (name != "") {
			for (i32 j = 0; j < restricted::triangles->size(); ++j)
				if (restricted::triangles->at(j)->GetName() == name)
					restricted::triangles->at(j)->active = active;
		}
		else if (index >= 0)
			restricted::triangles->at(index)->active = active;
}


void sur::Instancer::Delete(Types type, const std::string& name, i32 index)
{
	if (type == Types::Rectangle)
		if (name != "") {
			for (i32 j = 0; j < restricted::rectangles->size(); ++j)
				if (restricted::rectangles->at(j)->GetName() == name)
					restricted::rectangles->erase(restricted::rectangles->begin() + j);
		}
		else if (index >= 0)
			restricted::rectangles->erase(restricted::rectangles->begin() + index);
	if (type == Types::Line)
		if (name != "") {
			for (i32 j = 0; j < restricted::lines->size(); ++j)
				if (restricted::lines->at(j)->GetName() == name)
					restricted::lines->erase(restricted::lines->begin() + j);
		}
		else if (index >= 0)
			restricted::lines->erase(restricted::lines->begin() + index);
	if (type == Types::Obj)
		if (name != "") {
			for (i32 j = 0; j < restricted::objects->size(); ++j)
				if (restricted::objects->at(j)->GetName() == name)
					restricted::objects->erase(restricted::objects->begin() + j);
		}
		else if (index >= 0)
			restricted::objects->erase(restricted::objects->begin() + index);
	if (type == Types::Triangle)
		if (name != "") {
			for (i32 j = 0; j < restricted::triangles->size(); ++j)
				if (restricted::triangles->at(j)->GetName() == name)
					restricted::triangles->erase(restricted::triangles->begin() + j);
		}
		else if (index >= 0)
			restricted::triangles->erase(restricted::triangles->begin() + index);
}
//
//	Input
//

//	Mouse
sur::Vec2 sur::Input::Mouse::Position() const
{
	POINT point;
	GetCursorPos(&point);
	ScreenToClient(_hwnd, &point);
	if (point.x < 0 || point.y < 0)
		return { 0,0 };
	if (point.x > _window_size.x - 1 || point.y > _window_size.y - 1)
		return { _window_size.x - 1, _window_size.y - 1};
	return{ point.x, point.y };
}

bool sur::Input::Mouse::LClick() const
{
	if (GetAsyncKeyState(VK_LBUTTON))
		return true;
	return false;
}

bool sur::Input::Mouse::RClick() const
{
	if (GetAsyncKeyState(VK_RBUTTON))
		return true;
	return false;
}

// Keyboard
bool sur::Input::Keyboard::Key(Keys key) const
{
	if (GetAsyncKeyState(key))
		return true;
	return false;
}
//
//	Map_Analyses
//
void sur::Map_Analyses::operator()(sur::Maps map, sur::Vec2 size)
{
	Collider(map.ColliderMap, size);
	Trigger(map.ColliderMap, size);
	Render(map.RenderMap, size);
}

void sur::Map_Analyses::operator()(i32* cptr, i32* tptr, DWORD* rptr, sur::Vec2 size)
{
	Collider(cptr, size);
	Trigger(tptr, size);
	Render(rptr, size);
}

