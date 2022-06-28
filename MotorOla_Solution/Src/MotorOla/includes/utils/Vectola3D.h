#pragma once
#ifdef MOTOR_EXPORTS
#define MOTOR_API __declspec(dllexport)
#else
#define MOTOR_API __declspec(dllimport)
#endif

#include <iostream>
#include <math.h>
#include <Ogre.h>

class MOTOR_API Vectola3D
{
	double _x;
	double _y;
	double _z;

public:
	// Constructores
	Vectola3D();
	Vectola3D(const Vectola3D&v);
	Vectola3D(double x, double y, double z);

	virtual ~Vectola3D();

	inline double getX() const { return _x; }
	inline double getY() const { return _y; }
	inline double getZ() const { return _z; }

	inline void setX(double x) { _x = x; }
	inline void setY(double y) { _y = y; }
	inline void setZ(double z) { _z = z; }

	inline void set(const Vectola3D& v) { _x = v._x; _y = v._y; _z = v._z; }
	inline void set(double x, double y, double z) { _x = x;	_y = y;	_z = z; }
	
	inline double magnitude() const { return sqrt(pow(_x, 2) + pow(_y, 2) + pow(_z, 2)); }

	// Funciones sin refactorizar a 3d
	Vectola3D rotate(double degrees) const;
	double angle(const Vectola3D& v) const;
	Vectola3D normalize() const;

	double dotProduct(const Vectola3D& other) const { return _x * other._x + _y * other._y + _z * other._z; }

	Vectola3D crossProduct(const Vectola3D& other) const {
		float nx = _y * other._z - _z * other._y;
		float ny = _z * other._x - _x * other._z;
		float nz = _x * other._y - _y * other._x;
		return Vectola3D(nx, ny, nz);
	}

	bool operator==(const Vectola3D& quat) const;
	inline Vectola3D operator-(const Vectola3D& v) const { return Vectola3D(_x - v._x, _y - v._y, _z - v._z); }
	inline Vectola3D operator+(const Vectola3D& v) const { return Vectola3D(_x + v._x, _y + v._y, _z + v._z); }
	inline Vectola3D operator*(double d) const { return Vectola3D(_x * d, _y * d, _z * d); }
	inline double operator *(const Vectola3D& d) const { return d._x * _x + d._y * _y + d._z * _z; }
	Vectola3D operator/(double d) const { return Vectola3D(_x / d, _y / d, _z / d); }

	friend std::ostream& operator<<(std::ostream& os, const Vectola3D& v);

	Ogre::Vector3 convertOgreVector(const Vectola3D& v)const;
};

