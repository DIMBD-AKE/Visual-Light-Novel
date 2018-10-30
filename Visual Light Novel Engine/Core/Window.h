#pragma once
class Window
{
public:
	static function<void(UINT, UINT)> OnResize;

public:
	Window();
	virtual ~Window();
	virtual void Init(Setting * setting);
	
	WPARAM Run();

private:
	vector<string> nameList;
};

