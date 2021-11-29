/** author-developer: Roman Ermakov */

#ifndef _SINGLETON_H_
#define _SINGLETON_H_

namespace ern{
template <typename T>
class  SingletonDestroyer
{
  private:
    // Singleton* p_instance;
	T* p_instance;
  public:   
    ~SingletonDestroyer()
	{
		delete p_instance;
	}
    void initialize( T* p )
	{
		p_instance = p;
	}
};
//--------------------------------
template <typename T>
class  Singleton
{
  /*private:
    static SingletonDestroyer destroyer;*/
  protected:
	static T* singletonInstance;
    Singleton() { }
    Singleton( const Singleton& );
    Singleton& operator=( Singleton& );
    virtual ~Singleton() { }
    friend class SingletonsDestroyer;
  public:
    static T& getInstance()
	{
		if(!singletonInstance)  
		{
			singletonInstance = new T();
			// destroyer.initialize(singletonInstance);
			
		}
		return *singletonInstance;
	}
};
// template <typename T> T* Singleton<T>::singletonInstance;
}
#endif /* _SINGLETON_H_ */