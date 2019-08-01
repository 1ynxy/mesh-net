#ifndef shared_h
#define shared_h

#include <vector>
#include <algorithm>

class Sharable {
private:
	// Member Variables

	unsigned int references = 0;

	std::vector<void**> pointers = std::vector<void**>();

public:
	// Constructors && Destructors

	virtual ~Sharable() {
		destroy();
	}

	// Member Functions

	virtual void add_ref(void** pointer) {
		typename std::vector<void**>::iterator iter = std::find(pointers.begin(), pointers.end(), pointer);

		if (iter == pointers.end()) {
			*pointer = (void*) this;

			pointers.push_back(pointer);

			references++;
		}
	}

	virtual void clr_ref(void** pointer) {
		typename std::vector<void**>::iterator iter = std::find(pointers.begin(), pointers.end(), pointer);

		if (iter != pointers.end()) {
			*pointer = nullptr;

			pointers.erase(iter);

			references--;

			if (!references) destroy();
		}
	}

	unsigned int count() const {
		return references;
	}

	void replace(void** newPointer) {
		for (void**& pointer : pointers) *pointer = newPointer;
	}

	void destroy() {
		for (void**& pointer : pointers) *pointer = nullptr;
	}
};

template <typename T>
struct Shared {
	// Member Variables
	
	T* dest = nullptr;

	// Constructors & Destructors

	Shared() {

	}

	Shared(T* src) {
		if (src) ((Sharable*) src)->add_ref((void**) &dest);
	}

	template <typename O>
	Shared(O* src) {
		T* cast = dynamic_cast<T*>(src);

		if (cast) ((Sharable*) cast)->add_ref((void**) &dest);
	}

	Shared(const Shared<T>& src) {
		if (src.dest) ((Sharable*) src.dest)->add_ref((void**) &dest);
	}

	template <typename O>
	Shared(const Shared<O>& src) {
		T* cast = dynamic_cast<T*>(src.dest);
		
		if (cast) ((Sharable*) cast)->add_ref((void**) &dest);
	}

	~Shared() {
		if (dest) ((Sharable*) dest)->clr_ref((void**) &dest);
	}

	// Operator Overloading
	
	T* operator->() const {
		return dest;
	}

	operator bool() const {
		return dest;
	}

	Shared<T>& operator=(T* src) {
		if (dest != src) {
			if (dest) ((Sharable*) dest)->clr_ref((void**) &dest);
		
			if (src) ((Sharable*) src)->add_ref((void**) &dest);
		}

		return *this;
	}

	template <typename O>
	Shared<T>& operator=(O* src) {
		T* cast = dynamic_cast<T*>(src);

		if (dest != cast) {
			if (dest) ((Sharable*) dest)->clr_ref((void**) &dest);
		
			if (cast) ((Sharable*) cast)->add_ref((void**) &dest);
		}

		return *this;
	}

	Shared<T>& operator=(const Shared<T>& src) {
		if (dest != src.dest) {
			if (dest) ((Sharable*) dest)->clr_ref((void**) &dest);
		
			if (src.dest) ((Sharable*) src.dest)->add_ref((void**) &dest);
		}
		
		return *this;
	}

	template <typename O>
	Shared<T>& operator=(const Shared<O>& src) {
		T* cast = dynamic_cast<T*>(src.dest);

		if (dest != cast) {
			if (dest) ((Sharable*) dest)->clr_ref((void**) &dest);
		
			if (cast) ((Sharable*) cast)->add_ref((void**) &dest);
		}
		
		return *this;
	}

	bool operator==(const Shared<T>& other) {
		return dest == other.dest;
	}

	template <typename O>
	bool operator==(const Shared<O>& other) {
		T* cast = dynamic_cast<T*>(other.dest);

		return dest == cast;
	}

	bool operator!=(const Shared<T>& other) {
		return dest != other.dest;
	}

	template <typename O>
	bool operator!=(const Shared<O>& other) {
		T* cast = dynamic_cast<T*>(other.dest);

		return dest != cast;
	}
};

#endif
