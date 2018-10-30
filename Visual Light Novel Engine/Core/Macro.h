#pragma once

#define SINGLETON(class_name) \
private: \
	class_name(); \
	virtual ~class_name(); \
public: \
	static class_name* GetInstance() \
	{ \
		static class_name instance; \
		return &instance; \
	} \

#define SINGLETON_H(class_name) \
private: \
	class_name() {} \
	virtual ~class_name() {} \
public: \
	static class_name* GetInstance() \
	{ \
		static class_name instance; \
		return &instance; \
	} \

#define SYNTHESIZE(varType, varName, funcName)\
protected: varType varName;\
public: varType Get##funcName(void) { return varName; }\
public: void Set##funcName(const varType var) { varName = var; }

#define SYNTHESIZE_REF(varType, varName, funName)\
protected: varType varName;\
public: varType& Get##funName(void) { return varName; }\
public: void Set##funName(const varType& var) { varName = var; }

#define SYNTHESIZE_PT(varType, varName, funName)\
protected: varType varName;\
public: varType* Get##funName(void) { return &varName; }\
public: void Set##funName(const varType var) { varName = var; }

#define SAFE_DELETE(p) { if(p) delete p; p = nullptr; }
#define SAFE_DELETE_ARRAY(p) { if(p) delete[] p; p = nullptr; }
#define SAFE_RELEASE(p) { if(p) p->Release(); p = nullptr; }