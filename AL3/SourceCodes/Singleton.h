#pragma once
template<class T>
class Singleton
{
public:
	static inline T& GetInstance()
	{
		static T instance;
		return &instance;
	}

protected:
	Singleton() {}
	virtual ~Singleton() {}

private:
	Singleton &operator=(const Singleton& obj) {}
	Singleton(const Singleton& obj) {}
};