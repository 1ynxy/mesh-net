#ifndef object_h
#define object_h

#include <../src/shared.h>

class Sharable {
private:
	// Member Variables

	std::vector<void**> pointers = std::vector<void**>();

public:
	// Member Variables

	unsigned int references = 0;
	
	// Constructors && Destructors

	virtual ~Sharable() {
		for (void**& pointer : pointers) *pointer = nullptr;
	}

	// Member Functions

	virtual void add_reference(void** pointer) {
		typename std::vector<void**>::iterator iter = std::find(pointers.begin(), pointers.end(), pointer);

		if (iter == pointers.end()) {
			*pointer = (void*) this;

			pointers.push_back(pointer);

			references++;
		}
	}

	virtual void remove_reference(void** pointer) {
		typename std::vector<void**>::iterator iter = std::find(pointers.begin(), pointers.end(), pointer);

		if (iter != pointers.end()) {
			*pointer = nullptr;

			pointers.erase(iter);

			references--;

			if (!references) Destroy();
		}
	}

	void Destroy() {
		delete this;
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
		if (src) ((Sharable*) src)->add_reference((void**) &dest);
	}

	template <typename O>
	Shared(O* src) {
		T* cast = dynamic_cast<T*>(src);

		if (cast) ((Sharable*) cast)->add_reference((void**) &dest);
	}

	Shared(const Shared<T>& src) {
		if (src.dest) ((Sharable*) src.dest)->add_reference((void**) &dest);
	}

	template <typename O>
	Shared(const Shared<O>& src) {
		T* cast = dynamic_cast<T*>(src.dest);
		
		if (cast) ((Sharable*) cast)->add_reference((void**) &dest);
	}

	~Shared() {
		if (dest) ((Sharable*) dest)->remove_reference((void**) &dest);
	}

	// Operator Overloading
	
	T* operator->() const {
		return dest;
	}

	operator bool() const {
		return dest;
	}

	Shared<T>& operator=(T* src) {
		if (dest == src) return *this;

		if (dest) ((Sharable*) dest)->remove_reference((void**) &dest);
		
		if (src) ((Sharable*) src)->add_reference((void**) &dest);

		return *this;
	}

	template <typename O>
	Shared<T>& operator=(O* src) {
		T* cast = dynamic_cast<T*>(src);

		if (dest == cast) return *this;

		if (dest) ((Sharable*) dest)->remove_reference((void**) &dest);
		
		if (cast) ((Sharable*) cast)->add_reference((void**) &dest);

		return *this;
	}

	Shared<T>& operator=(const Shared<T>& src) {
		if (dest == src.dest) return *this;

		if (dest) ((Sharable*) dest)->remove_reference((void**) &dest);
		
		if (src.dest) ((Sharable*) src.dest)->add_reference((void**) &dest);
		
		return *this;
	}

	template <typename O>
	Shared<T>& operator=(const Shared<O>& src) {
		T* cast = dynamic_cast<T*>(src.dest);

		if (dest == cast) return *this;

		if (dest) ((Sharable*) dest)->remove_reference((void**) &dest);
		
		if (cast) ((Sharable*) cast)->add_reference((void**) &dest);
		
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
